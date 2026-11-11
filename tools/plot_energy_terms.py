#!/usr/bin/env python3
"""Plot the energy terms and the elastic weight per Newton iteration from energy traces.

    plot_energy_terms.py OUT.png TRACE_DIR [TRACE_DIR ...] [--title TITLE]

Each TRACE_DIR is a directory written with "experiments": {"energy_trace": DIR}
(it holds iterations.csv). One panel per directory, labelled by the directory
name (or its parent's, if it is called "trace"): every form's weighted energy
on a log scale, and below it the elastic form's weight (kappa of the adaptive
elastic stiffness; constant otherwise). Solves are concatenated, with a dashed
line where a new one starts (e.g. the augmented Lagrangian's).
"""
import argparse
import csv
import os
import textwrap

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.ticker

INK, MUTED, GRID, AXIS = "#0b0b0b", "#52514e", "#e4e3df", "#c9c8c3"
# fixed color per form, in order of first appearance after these
COLORS = {"elastic": "#2a78d6", "quadratic-penalty": "#eb6834"}
EXTRA = ["#1f9e89", "#8e5cc4", "#c4a000", "#d14a8b", "#5d6b7a"]
SKIP = {"total", "grad_norm", "grad_norm_inf", "elastic_weight"}


def label_of(d):
    d = os.path.normpath(d)
    name = os.path.basename(d)
    return os.path.basename(os.path.dirname(d)) if name == "trace" else name


def read(d):
    """Series per form, x = running state index over all solves; solve starts."""
    series, index, starts = {}, {}, []
    with open(os.path.join(d, "iterations.csv")) as f:
        for r in csv.DictReader(f):
            key = (int(r["solve"]), int(r["state"]))
            if key not in index:
                if index and key[0] != list(index)[-1][0]:
                    starts.append(len(index))
                index[key] = len(index)
            series.setdefault(r["form"], []).append((index[key], float(r["value"])))
    return series, starts


def style(ax):
    ax.grid(True, which="major", color=GRID, lw=0.6)
    ax.tick_params(colors=MUTED, labelsize=8)
    for s in ("top", "right"):
        ax.spines[s].set_visible(False)
    for s in ("left", "bottom"):
        ax.spines[s].set_color(AXIS)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("out")
    ap.add_argument("traces", nargs="+")
    ap.add_argument("--title", default="Energy terms and elastic weight per Newton iteration")
    a = ap.parse_args()

    traces = [d for d in a.traces if os.path.exists(os.path.join(d, "iterations.csv"))]
    if not traces:
        raise SystemExit("no iterations.csv in the given directories")

    ncol = min(5, len(traces))
    nrow = (len(traces) + ncol - 1) // ncol
    ratios = []
    for r in range(nrow):
        ratios += ([0.55] if r else []) + [3, 1]
    fig = plt.figure(figsize=(3.6 * ncol + 0.6, 5.0 * nrow + 0.8))
    gs = fig.add_gridspec(len(ratios), ncol, height_ratios=ratios, hspace=0.12, wspace=0.28)

    colors = dict(COLORS)
    handles = {}
    for k, d in enumerate(traces):
        series, starts = read(d)
        r0, c = 3 * (k // ncol), k % ncol
        ax = fig.add_subplot(gs[r0, c])
        axk = fig.add_subplot(gs[r0 + 1, c], sharex=ax)
        for form, pts in series.items():
            if form in SKIP:
                continue
            pts = sorted(p for p in pts if p[1] > 0)  # log scale
            if not pts:
                continue
            if form not in colors:
                colors[form] = EXTRA[(len(colors) - len(COLORS)) % len(EXTRA)]
            (h,) = ax.plot([p[0] for p in pts], [p[1] for p in pts], color=colors[form], lw=2, label=form)
            handles.setdefault(form, h)
        kap = sorted(series.get("elastic_weight", []))
        for x in starts:
            for axis in (ax, axk):
                axis.axvline(x, color=AXIS, lw=1, ls="--")
        if starts:
            ax.text(starts[0], 0.02, " next solve", transform=ax.get_xaxis_transform(), fontsize=8, color=MUTED)
        ax.set_yscale("log")
        ax.set_title(label_of(d), fontsize=11, color=INK)
        plt.setp(ax.get_xticklabels(), visible=False)
        style(ax)
        if kap:
            axk.step([p[0] for p in kap], [p[1] for p in kap], where="post", color=MUTED, lw=2)
            axk.text(0.02, 0.9, f"final weight {kap[-1][1]:g}", transform=axk.transAxes, ha="left", va="top", fontsize=8, color=MUTED)
            axk.set_yscale("log")
            axk.set_ylim(bottom=0.5 * min(p[1] for p in kap), top=2.5 * max(p[1] for p in kap))
            axk.yaxis.set_major_formatter(matplotlib.ticker.FuncFormatter(lambda v, _: f"{v:g}"))
            axk.yaxis.set_minor_formatter(matplotlib.ticker.NullFormatter())
        style(axk)
        if c == 0:
            ax.set_ylabel("energy (weighted)", color=MUTED, fontsize=9)
            axk.set_ylabel("elastic weight", color=MUTED, fontsize=9)
        if k // ncol == nrow - 1:
            axk.set_xlabel("Newton iteration", color=MUTED, fontsize=9)

    title = textwrap.fill(a.title, width=int(9 * fig.get_figwidth()))
    nt = title.count("\n") + 1
    fig.legend(list(handles.values()), list(handles.keys()), loc="upper center", ncol=min(len(handles), 4), frameon=False,
               fontsize=10, bbox_to_anchor=(0.5, 1 - (0.25 + 0.25 * nt) / fig.get_figheight()))
    fig.suptitle(title, y=1 - 0.05 / fig.get_figheight(), fontsize=12, color=INK)
    fig.subplots_adjust(left=0.6 / fig.get_figwidth(), right=0.99,
                        top=1 - (0.8 + 0.25 * nt) / fig.get_figheight(), bottom=0.5 / fig.get_figheight())
    fig.savefig(a.out, dpi=130, facecolor="#fcfcfb")
    print(a.out)


if __name__ == "__main__":
    main()
