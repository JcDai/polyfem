"""
Generate miso C++ solver classes for polyfem Jacobian validity checks.

Besides the classes below, DegenerateChecks() generates the P3 classes used
when elements are allowed a known zero of det J at one corner (see its
docstring):
  P3{Tri,Tet}DegVal, P3{Tri,Tet}DegCGV, JacEval_P3{Tri,Tet}Bernstein.

Produces two classes per (D, P) pair:
  P{P}{Simplex}Val  -- SOLVE: static injectivity check (det J > 0 everywhere)
  P{P}{Simplex}CGV  -- MINIMIZE: continuous geometric validity (max safe time step)
  JacEval_P{P}{Simplex}  -- evaluator for det J at a point (used for P=1 static check
                             and robust_evaluate_jacobian)

Run:
    python generate.py                 # everything
    python generate.py --degenerate    # only the degenerate classes

Set POLYFEM_PYMISO_DIR to use an existing pymiso checkout instead of cloning.
"""
import os
import re
import sys
import shutil
import argparse
import subprocess

from sympy import Matrix, symbols

# Clone pymiso from GitLab into ignore/pymiso if not already present, on the
# branch with degenerate inclusions, the cone subdivision and sampling at
# given points (needed by DegenerateChecks; the other classes are unchanged
# by it).
_HERE = os.path.dirname(os.path.abspath(__file__))
_PYMISO_URL = 'https://gitlab.com/minimize-solve/pymiso.git'
_PYMISO_BRANCH = 'polyfem-degenerate'
_PYMISO_DIR = os.environ.get('POLYFEM_PYMISO_DIR', os.path.join(_HERE, 'ignore', 'pymiso'))

if not os.path.isdir(_PYMISO_DIR):
    os.makedirs(os.path.dirname(_PYMISO_DIR), exist_ok=True)
    subprocess.run(['git', 'clone', '--branch', _PYMISO_BRANCH, _PYMISO_URL, _PYMISO_DIR], check=True)

sys.path.insert(0, _PYMISO_DIR)
from miso import Domain, Basis, make_poly, generate, generate_evaluator
from miso.subdivision import Subdivision, cone_subdivision, combine_subdiv, identity_subdivision

SRC_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'generated')

SIMPLEX = {1: 'Lin', 2: 'Tri', 3: 'Tet'}


def SimplexChecks(D, P):
    """
    Generate both static (Val) and continuous (CGV) validity solvers for a
    degree-P map on a D-simplex. Two generate() calls share the symbolic setup.

    Control points are rest position + displacement, taken as separate inputs
    (r, u) so the sum is formed in interval arithmetic: the displacement is the
    solver's own vector, which the continuous check widens to cover the step
    the solver actually takes (fix.pdf), and nothing rounds it on the way in.
    """
    simplex = SIMPLEX[D]
    X = Domain(' '.join(f'x{i}' for i in range(D)))
    T = Domain('t')
    domain = X * T
    t = domain.variables[-1]  # T[0] may differ by identity from domain.variables

    def rest_plus(name):
        r = X.poly_map(P, Basis.LAGRANGE, codomain_dim=D, name='r', output_miso_poly=False)
        u = X.poly_map(P, Basis.LAGRANGE, codomain_dim=D, name=name, output_miso_poly=False)
        return r + u

    # Compute the static Jacobian determinant (shared symbolic work)
    x = rest_plus('u')
    Jx = x.jacobian(X.variables).applyfunc(make_poly)
    jd_val = make_poly(Jx.det())

    # Evaluator: standalone function computing det J at a point (all free symbols
    # become parameters). Used for P=1 static validity check (constant det) and
    # for robust_evaluate_jacobian (pointwise evaluation for P>1).
    generate_evaluator(SRC_DIR, f'JacEval_P{P}{simplex}', jd_val)
    print(f'JacEval_P{P}{simplex} done')

    # Call 1: static validity (SOLVE, no objective)
    # P=1: Jacobian of a linear map is constant in domain variables (degree 0);
    # miso's Bernstein converter requires degree >= 1.  Handled directly in C++.
    if P > 1:
        generate(SRC_DIR, f'P{P}{simplex}Val', X, [jd_val], parallel=True)
        print(f'P{P}{simplex}Val done')

    # CGV: blended map (1-t)*p0 + t*p1 with p0 = r + u0, p1 = r + u1
    p0 = rest_plus('u0')
    p1 = rest_plus('u1')

    if P == 1:
        # The Jacobian of a linear map is constant in space, so validity
        # along the step is a one-variable problem in t.
        tt = T.variables[0]
        Jt = p0.jacobian(X.variables) * (1 - tt) + p1.jacobian(X.variables) * tt
        jd_t = make_poly(Jt.det())
        generate(SRC_DIR, f'P{P}{simplex}CGV', T, [jd_t], objective=tt, parallel=True)
        print(f'P{P}{simplex}CGV done')
        return

    p = [make_poly(a * (1 - t) + b * t) for a, b in zip(p0, p1)]
    p_ext = p + [t]  # plain symbol: avoids sympy Function canonicalization swapping t identity
    jd_cgv = make_poly(Matrix(p_ext).jacobian(domain.variables).applyfunc(make_poly).det())
    sd_all = domain.default_subdivision
    sd_t = domain.make_subdivision(T)

    # Call 2: continuous geometric validity (MINIMIZE, objective=t)
    generate(SRC_DIR, f'P{P}{simplex}CGV', domain, [jd_cgv],
             objective=t, subdivisions=[sd_all, sd_t], parallel=True)
    print(f'P{P}{simplex}CGV done')



# Quadrature order of the elastic energy for P3 simplices
# (AssemblerUtils::quadrature_order: 2 * (3 - 1)), whose polyfem rule the
# degenerate classes sample at.
DEGENERATE_QUADRATURE_ORDER = 4


def polyfem_rule(D, order):
    """
    Points and weights of polyfem's (non-corner) quadrature rule, read from
    the autogen tables TriQuadrature/TetQuadrature are compiled from. The
    decimal literals there are exact expansions of doubles, so float() gives
    the very numbers polyfem uses.
    """
    name = 'triangle' if D == 2 else 'tetrahedron'
    path = os.path.join(_HERE, '..', 'autogen', f'auto_{name}.ipp')
    text = open(path).read()
    m = re.search(rf'case {order}:.*?points << (.*?);.*?weights << (.*?);', text, re.S)
    if m is None:
        raise RuntimeError(f'order {order} not found in {path}')
    pts = [float(x) for x in m.group(1).replace('\n', ' ').split(',')]
    wts = [float(x) for x in m.group(2).replace('\n', ' ').split(',')]
    points = [tuple(pts[i:i + D]) for i in range(0, len(pts), D)]
    assert len(points) == len(wts)
    return points, wts


def DegenerateChecks(D, P=3):
    """
    P{P}{Simplex}DegVal / P{P}{Simplex}DegCGV: static and continuous checks for
    elements allowed a zero of det J at one corner, where edge control points
    collapse onto the corner's (one for triangles; one or two for
    tetrahedra, det J then vanishes there to that order). The collapse holds
    for the whole step, so the continuous check only needs the corner times
    the time interval as degenerate face.

    Differences from SimplexChecks():
      - the map is given by Bernstein coefficients p (rest + displacement,
        formed in interval arithmetic by the caller);
      - degenerate states for the corners (allow_degenerate), selected at
        runtime through degenerateState;
      - det J is sampled at the points of polyfem's quadrature rule of order
        DEGENERATE_QUADRATURE_ORDER, which has no corners and positive
        weights, instead of at the box vertices: the energy then integrates
        over the same points the checks report, and never at the corner
        where det J is allowed to vanish;
      - tetrahedra: an order-2 corner box is split alternately with the
        uniform subdivision and with a cone split (the facet opposite the
        corner split uniformly, each piece coned to the corner), so that its
        angles at the corner go to zero. In the continuous check every
        other box alternates between the space-time and the space-only
        uniform subdivision, so the time width of a box becomes large
        relative to its size and interior sample points can show an
        inversion starting on the element boundary.

    Scheme (child index) layout, which polyfem's Tree mirrors:
      Val, tet:  0 uniform (8), 1+v cone at polyfem vertex v (4 each)
      Val, tri:  0 uniform (4)
      CGV, tet:  0 space-time uniform (16), 1 time only (2),
                 2+v cone at polyfem vertex v (4 each), 6 space-only uniform (8)
      CGV, tri:  0 space-time uniform (8), 1 time only (2), 2 space-only uniform (4)
    """
    simplex = SIMPLEX[D]
    order = 2 if D == 3 else 1
    X = Domain(' '.join(f'x{i}' for i in range(D)))
    T = Domain('t')
    rule, _ = polyfem_rule(D, DEGENERATE_QUADRATURE_ORDER)
    margin = min(min(min(p), 1 - sum(p)) for p in rule)
    samples = {'factor': 0, 'points': rule, 'margin': margin}

    # miso's domain vertices and polyfem's reference vertices are the same
    # points: polyfem vertex 0 is the origin, vertex i the i-th unit vector.
    xverts = X.simplices[0].vertices
    polyfem_vertex = [0 if sum(v) == 0 else 1 + list(v).index(1) for v in xverts]
    domain_vertex = [polyfem_vertex.index(pv) for pv in range(D + 1)]

    x = X.poly_map(P, Basis.BERNSTEIN, codomain_dim=D, name='p', output_miso_poly=False)
    jd_val = make_poly(x.jacobian(X.variables).applyfunc(make_poly).det())
    generate_evaluator(SRC_DIR, f'JacEval_P{P}{simplex}Bernstein', jd_val)
    print(f'JacEval_P{P}{simplex}Bernstein done')

    cones = [cone_subdivision(D, pv) for pv in range(D + 1)] if order >= 2 else []
    val_faces = [{domain_vertex[pv]} for pv in range(D + 1)]
    generate(SRC_DIR, f'P{P}{simplex}DegVal', X, [jd_val],
             subdivisions=[X.default_subdivision] + cones,
             allow_degenerate=order, degenerate_faces=val_faces,
             alternate_schemes={frozenset(f): 1 + pv for pv, f in enumerate(val_faces)} if cones else None,
             sample_points=samples, parallel=True)
    print(f'P{P}{simplex}DegVal done')

    dom = X * T
    t = dom.variables[-1]
    p0 = X.poly_map(P, Basis.BERNSTEIN, codomain_dim=D, name='p0')
    p1 = X.poly_map(P, Basis.BERNSTEIN, codomain_dim=D, name='p1')
    p = [make_poly(a * (1 - t) + b * t) for a, b in zip(p0, p1)]
    jd_cgv = make_poly(Matrix(p + [t]).jacobian(dom.variables).applyfunc(make_poly).det())

    nT = len(T.simplices[0].vertices)
    ident_t = identity_subdivision(1)[0]
    cones_st = [Subdivision(*(combine_subdiv(m, ident_t) for m in c)) for c in cones]
    subdivisions = [dom.default_subdivision, dom.make_subdivision(T)] + cones_st + [dom.make_subdivision(X)]
    space_only = len(subdivisions) - 1
    cgv_faces = [{domain_vertex[pv] * nT + k for k in range(nT)} for pv in range(D + 1)]
    generate(SRC_DIR, f'P{P}{simplex}DegCGV', dom, [jd_cgv], objective=t,
             subdivisions=subdivisions,
             allow_degenerate=order, degenerate_faces=cgv_faces,
             alternate_schemes={frozenset(f): 2 + pv for pv, f in enumerate(cgv_faces)} if cones else {},
             alternate_default=space_only,
             parity_schemes=[i for i in range(len(subdivisions)) if i != 1],
             sample_points=samples, parallel=(2 if D == 3 else True))
    print(f'P{P}{simplex}DegCGV done')


def write_degenerate_rules():
    """The rule points the degenerate classes sample at, for polyfem to check
    that its quadrature still matches them."""
    with open(os.path.join(SRC_DIR, 'DegenerateRules.hpp'), 'w') as f:
        f.write('#pragma once\n// Generated by generate.py: the quadrature points the P3 degenerate\n')
        f.write('// classes sample at (polyfem rule of order %d), as exact doubles.\n' % DEGENERATE_QUADRATURE_ORDER)
        f.write('namespace miso {\n')
        f.write(f'constexpr int degenerate_quadrature_order = {DEGENERATE_QUADRATURE_ORDER};\n')
        for D in (2, 3):
            pts, _ = polyfem_rule(D, DEGENERATE_QUADRATURE_ORDER)
            name = SIMPLEX[D].lower()
            f.write(f'constexpr int degenerate_{name}_rule_size = {len(pts)};\n')
            f.write(f'constexpr double degenerate_{name}_rule[{len(pts)}][{D}] = {{\n')
            for pt in pts:
                f.write('\t{' + ', '.join(float.hex(c) for c in pt) + '},\n')
            f.write('};\n')
        f.write('}\n')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--degenerate', action='store_true', help='only (re)generate the degenerate classes')
    parser.add_argument('--dims', type=int, nargs='+', default=[2, 3], help='dimensions of the degenerate classes')
    cli = parser.parse_args()
    os.makedirs(SRC_DIR, exist_ok=True)

    if not cli.degenerate:
        # Clear existing class subdirectories; keep top-level CMakeLists.txt / include.hpp
        for entry in os.scandir(SRC_DIR):
            if entry.is_dir():
                shutil.rmtree(entry.path)
        for D, P in [(2, 1), (2, 2), (2, 3), (2, 4), (3, 1), (3, 2), (3, 3)]:
            SimplexChecks(D, P)

    for D in cli.dims:
        DegenerateChecks(D)
    write_degenerate_rules()

    print('All done.')
