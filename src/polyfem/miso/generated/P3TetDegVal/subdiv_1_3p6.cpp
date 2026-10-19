#include "../P3TetDegVal.hpp"
namespace miso {
	std::array<RealVector<84>, 4> P3TetDegVal::subdiv_1_3p6(const RealVector<84> &_b) {
		#pragma region Temporaries
		const Real _t0 = (0.5)*_b[1];
		const Real _t1 = (0.5)*_b[28];
		const Real _t2 = _t0 + _t1;
		const Real _t3 = (0.5)*_b[29];
		const Real _t4 = (0.25)*_b[2];
		const Real _t5 = (0.25)*_b[49];
		const Real _t6 = _t3 + _t4 + _t5;
		const Real _t7 = (0.125)*_b[3];
		const Real _t8 = (0.125)*_b[64];
		const Real _t9 = (0.375)*_b[30] + (0.375)*_b[50] + _t7 + _t8;
		const Real _t10 = (0.25)*_b[31];
		const Real _t11 = (0.0625)*_b[4];
		const Real _t12 = (0.375)*_b[51];
		const Real _t13 = (0.25)*_b[65];
		const Real _t14 = (0.0625)*_b[74];
		const Real _t15 = _t10 + _t11 + _t12 + _t13 + _t14;
		const Real _t16 = (0.03125)*_b[5];
		const Real _t17 = (0.03125)*_b[80];
		const Real _t18 = (0.15625)*_b[32] + (0.3125)*_b[52] + (0.3125)*_b[66] + (0.15625)*_b[75] + _t16 + _t17;
		const Real _t19 = (0.09375)*_b[33];
		const Real _t20 = (0.3125)*_b[67];
		const Real _t21 = (0.015625)*_b[6];
		const Real _t22 = (0.09375)*_b[81];
		const Real _t23 = (0.015625)*_b[83];
		const Real _t24 = (0.234375)*_b[53] + (0.234375)*_b[76] + _t19 + _t20 + _t21 + _t22 + _t23;
		const Real _t25 = (0.5)*_b[7];
		const Real _t26 = _t1 + _t25;
		const Real _t27 = (0.25)*_b[29] + (0.25)*_b[34] + (0.25)*_b[8];
		const Real _t28 = _t27 + _t5;
		const Real _t29 = (0.25)*_b[50];
		const Real _t30 = (0.25)*_b[35];
		const Real _t31 = _t29 + _t30;
		const Real _t32 = (0.125)*_b[54];
		const Real _t33 = (0.125)*_b[30];
		const Real _t34 = (0.125)*_b[9];
		const Real _t35 = _t33 + _t34;
		const Real _t36 = _t32 + _t35;
		const Real _t37 = _t31 + _t36 + _t8;
		const Real _t38 = (0.1875)*_b[55];
		const Real _t39 = _t14 + _t38;
		const Real _t40 = (0.0625)*_b[10];
		const Real _t41 = (0.1875)*_b[36];
		const Real _t42 = (0.0625)*_b[31] + _t41;
		const Real _t43 = _t40 + _t42;
		const Real _t44 = (0.0625)*_b[68];
		const Real _t45 = (0.1875)*_b[51] + _t44;
		const Real _t46 = (0.1875)*_b[65] + _t39 + _t43 + _t45;
		const Real _t47 = (0.125)*_b[52];
		const Real _t48 = (0.125)*_b[75];
		const Real _t49 = (0.1875)*_b[56];
		const Real _t50 = (0.1875)*_b[66] + _t49;
		const Real _t51 = (0.125)*_b[69];
		const Real _t52 = _t17 + _t51;
		const Real _t53 = (0.03125)*_b[77];
		const Real _t54 = (0.03125)*_b[11];
		const Real _t55 = (0.125)*_b[37];
		const Real _t56 = (0.03125)*_b[32] + _t55;
		const Real _t57 = _t54 + _t56;
		const Real _t58 = _t53 + _t57;
		const Real _t59 = _t47 + _t48 + _t50 + _t52 + _t58;
		const Real _t60 = (0.15625)*_b[76];
		const Real _t61 = (0.078125)*_b[78];
		const Real _t62 = _t23 + _t61;
		const Real _t63 = (0.015625)*_b[12];
		const Real _t64 = (0.078125)*_b[38];
		const Real _t65 = (0.015625)*_b[33] + _t64;
		const Real _t66 = _t63 + _t65;
		const Real _t67 = (0.015625)*_b[82];
		const Real _t68 = (0.15625)*_b[57];
		const Real _t69 = (0.15625)*_b[70];
		const Real _t70 = (0.15625)*_b[67] + _t67 + _t68 + _t69;
		const Real _t71 = (0.078125)*_b[53] + (0.078125)*_b[81] + _t60 + _t62 + _t66 + _t70;
		const Real _t72 = (0.25)*_b[13];
		const Real _t73 = (0.5)*_b[34];
		const Real _t74 = _t5 + _t72 + _t73;
		const Real _t75 = (0.25)*_b[54];
		const Real _t76 = (0.125)*_b[14];
		const Real _t77 = (0.125)*_b[39];
		const Real _t78 = _t76 + _t77;
		const Real _t79 = (0.125)*_b[50] + _t30;
		const Real _t80 = _t78 + _t79;
		const Real _t81 = _t75 + _t8 + _t80;
		const Real _t82 = (0.125)*_b[68];
		const Real _t83 = (0.125)*_b[36];
		const Real _t84 = _t82 + _t83;
		const Real _t85 = (0.25)*_b[55];
		const Real _t86 = (0.125)*_b[65];
		const Real _t87 = (0.125)*_b[40];
		const Real _t88 = _t85 + _t86 + _t87;
		const Real _t89 = (0.0625)*_b[58];
		const Real _t90 = (0.0625)*_b[15];
		const Real _t91 = (0.0625)*_b[51];
		const Real _t92 = _t90 + _t91;
		const Real _t93 = _t89 + _t92;
		const Real _t94 = _t14 + _t84 + _t88 + _t93;
		const Real _t95 = (0.0625)*_b[77];
		const Real _t96 = (0.0625)*_b[37];
		const Real _t97 = _t95 + _t96;
		const Real _t98 = (0.1875)*_b[69];
		const Real _t99 = _t17 + _t98;
		const Real _t100 = (0.03125)*_b[16];
		const Real _t101 = (0.03125)*_b[52];
		const Real _t102 = (0.09375)*_b[59];
		const Real _t103 = _t101 + _t102;
		const Real _t104 = _t100 + _t103;
		const Real _t105 = (0.03125)*_b[71];
		const Real _t106 = (0.09375)*_b[41];
		const Real _t107 = _t105 + _t106;
		const Real _t108 = _t107 + _t49;
		const Real _t109 = (0.09375)*_b[66] + _t108;
		const Real _t110 = (0.09375)*_b[75] + _t104 + _t109 + _t97 + _t99;
		const Real _t111 = (0.0625)*_b[81];
		const Real _t112 = (0.03125)*_b[82];
		const Real _t113 = (0.125)*_b[57];
		const Real _t114 = (0.03125)*_b[38];
		const Real _t115 = _t112 + _t113 + _t114;
		const Real _t116 = (0.1875)*_b[70];
		const Real _t117 = (0.09375)*_b[76] + _t116;
		const Real _t118 = (0.015625)*_b[17];
		const Real _t119 = (0.015625)*_b[53];
		const Real _t120 = _t118 + _t119;
		const Real _t121 = (0.125)*_b[78];
		const Real _t122 = _t121 + _t23;
		const Real _t123 = (0.0625)*_b[67];
		const Real _t124 = (0.0625)*_b[42];
		const Real _t125 = (0.0625)*_b[72];
		const Real _t126 = (0.09375)*_b[60];
		const Real _t127 = (0.015625)*_b[79];
		const Real _t128 = _t126 + _t127;
		const Real _t129 = _t124 + _t125 + _t128;
		const Real _t130 = _t123 + _t129;
		const Real _t131 = _t111 + _t115 + _t117 + _t120 + _t122 + _t130;
		const Real _t132 = (0.125)*_b[18];
		const Real _t133 = (0.375)*_b[39] + (0.375)*_b[54] + _t132 + _t8;
		const Real _t134 = (0.0625)*_b[19];
		const Real _t135 = (0.1875)*_b[40];
		const Real _t136 = (0.0625)*_b[43] + _t135;
		const Real _t137 = _t134 + _t136;
		const Real _t138 = (0.0625)*_b[65];
		const Real _t139 = (0.1875)*_b[58] + _t138;
		const Real _t140 = (0.1875)*_b[68] + _t137 + _t139 + _t39;
		const Real _t141 = (0.0625)*_b[75];
		const Real _t142 = (0.0625)*_b[44];
		const Real _t143 = _t141 + _t142;
		const Real _t144 = (0.03125)*_b[20];
		const Real _t145 = (0.03125)*_b[61];
		const Real _t146 = (0.09375)*_b[56];
		const Real _t147 = _t145 + _t146;
		const Real _t148 = _t144 + _t147;
		const Real _t149 = (0.1875)*_b[59];
		const Real _t150 = (0.03125)*_b[66];
		const Real _t151 = _t149 + _t150;
		const Real _t152 = (0.09375)*_b[71] + _t106 + _t151;
		const Real _t153 = (0.09375)*_b[77] + _t143 + _t148 + _t152 + _t99;
		const Real _t154 = (0.015625)*_b[67];
		const Real _t155 = (0.015625)*_b[73];
		const Real _t156 = (0.140625)*_b[60];
		const Real _t157 = _t154 + _t155 + _t156;
		const Real _t158 = (0.015625)*_b[21];
		const Real _t159 = (0.046875)*_b[42] + (0.046875)*_b[57] + _t158;
		const Real _t160 = _t157 + _t159;
		const Real _t161 = (0.140625)*_b[72] + (0.046875)*_b[79];
		const Real _t162 = (0.046875)*_b[45] + (0.046875)*_b[62];
		const Real _t163 = (0.140625)*_b[70] + (0.046875)*_b[76] + _t162;
		const Real _t164 = (0.140625)*_b[78] + (0.046875)*_b[81] + (0.046875)*_b[82] + _t160 + _t161 + _t163 + _t23;
		const Real _t165 = (0.0625)*_b[22];
		const Real _t166 = (0.25)*_b[43];
		const Real _t167 = (0.375)*_b[58];
		const Real _t168 = (0.25)*_b[68];
		const Real _t169 = _t14 + _t165 + _t166 + _t167 + _t168;
		const Real _t170 = (0.125)*_b[61];
		const Real _t171 = (0.125)*_b[77];
		const Real _t172 = (0.1875)*_b[71] + _t149;
		const Real _t173 = (0.125)*_b[44];
		const Real _t174 = (0.03125)*_b[46] + _t173;
		const Real _t175 = (0.03125)*_b[23];
		const Real _t176 = (0.03125)*_b[75];
		const Real _t177 = _t175 + _t176;
		const Real _t178 = _t174 + _t177;
		const Real _t179 = _t170 + _t171 + _t172 + _t178 + _t52;
		const Real _t180 = (0.0625)*_b[82];
		const Real _t181 = (0.125)*_b[62];
		const Real _t182 = (0.03125)*_b[81];
		const Real _t183 = (0.03125)*_b[47];
		const Real _t184 = _t181 + _t182 + _t183;
		const Real _t185 = (0.1875)*_b[72];
		const Real _t186 = (0.09375)*_b[79] + _t185;
		const Real _t187 = (0.015625)*_b[24];
		const Real _t188 = (0.015625)*_b[63];
		const Real _t189 = _t126 + _t188;
		const Real _t190 = _t187 + _t189;
		const Real _t191 = (0.0625)*_b[73];
		const Real _t192 = (0.0625)*_b[45];
		const Real _t193 = (0.0625)*_b[70];
		const Real _t194 = (0.015625)*_b[76];
		const Real _t195 = _t191 + _t192 + _t193 + _t194;
		const Real _t196 = _t122 + _t180 + _t184 + _t186 + _t190 + _t195;
		const Real _t197 = (0.03125)*_b[25];
		const Real _t198 = (0.15625)*_b[46] + (0.3125)*_b[61] + (0.3125)*_b[71] + (0.15625)*_b[77] + _t17 + _t197;
		const Real _t199 = (0.15625)*_b[79];
		const Real _t200 = (0.015625)*_b[26];
		const Real _t201 = (0.078125)*_b[47];
		const Real _t202 = (0.015625)*_b[48] + _t201;
		const Real _t203 = _t200 + _t202;
		const Real _t204 = (0.015625)*_b[81];
		const Real _t205 = (0.15625)*_b[62];
		const Real _t206 = (0.15625)*_b[72];
		const Real _t207 = (0.15625)*_b[73] + _t204 + _t205 + _t206;
		const Real _t208 = (0.078125)*_b[63] + (0.078125)*_b[82] + _t199 + _t203 + _t207 + _t62;
		const Real _t209 = (0.015625)*_b[27];
		const Real _t210 = (0.09375)*_b[48];
		const Real _t211 = (0.3125)*_b[73];
		const Real _t212 = (0.09375)*_b[82];
		const Real _t213 = (0.234375)*_b[63] + (0.234375)*_b[79] + _t209 + _t210 + _t211 + _t212 + _t23;
		const Real _t214 = (0.5)*_b[49];
		const Real _t215 = (0.25)*_b[30];
		const Real _t216 = (0.5)*_b[50];
		const Real _t217 = (0.25)*_b[64];
		const Real _t218 = (0.125)*_b[31];
		const Real _t219 = (0.125)*_b[74];
		const Real _t220 = (0.0625)*_b[32];
		const Real _t221 = (0.25)*_b[52];
		const Real _t222 = (0.375)*_b[66];
		const Real _t223 = (0.25)*_b[75];
		const Real _t224 = (0.0625)*_b[80];
		const Real _t225 = (0.03125)*_b[33];
		const Real _t226 = (0.15625)*_b[53];
		const Real _t227 = (0.03125)*_b[83];
		const Real _t228 = (0.125)*_b[51];
		const Real _t229 = _t13 + _t85;
		const Real _t230 = (0.0625)*_b[52];
		const Real _t231 = _t224 + _t98;
		const Real _t232 = (0.125)*_b[67];
		const Real _t233 = (0.125)*_b[81];
		const Real _t234 = (0.03125)*_b[53];
		const Real _t235 = (0.1875)*_b[76] + _t116;
		const Real _t236 = _t121 + _t227;
		const Real _t237 = (0.25)*_b[39];
		const Real _t238 = (0.5)*_b[54];
		const Real _t239 = (0.125)*_b[58];
		const Real _t240 = (0.125)*_b[56];
		const Real _t241 = _t171 + _t240;
		const Real _t242 = (0.25)*_b[69];
		const Real _t243 = (0.125)*_b[59];
		const Real _t244 = _t242 + _t243 + _t48;
		const Real _t245 = (0.0625)*_b[71];
		const Real _t246 = (0.0625)*_b[66];
		const Real _t247 = _t245 + _t246;
		const Real _t248 = (0.03125)*_b[79];
		const Real _t249 = (0.09375)*_b[72];
		const Real _t250 = (0.0625)*_b[57];
		const Real _t251 = _t180 + _t250;
		const Real _t252 = (0.03125)*_b[67];
		const Real _t253 = _t126 + _t252;
		const Real _t254 = (0.1875)*_b[78];
		const Real _t255 = _t227 + _t254;
		const Real _t256 = (0.125)*_b[43];
		const Real _t257 = (0.0625)*_b[61];
		const Real _t258 = (0.03125)*_b[76];
		const Real _t259 = (0.09375)*_b[70];
		const Real _t260 = (0.0625)*_b[62];
		const Real _t261 = _t111 + _t260;
		const Real _t262 = (0.03125)*_b[73];
		const Real _t263 = _t126 + _t262;
		const Real _t264 = (0.0625)*_b[46];
		const Real _t265 = (0.25)*_b[61];
		const Real _t266 = (0.375)*_b[71];
		const Real _t267 = (0.25)*_b[77];
		const Real _t268 = (0.125)*_b[73];
		const Real _t269 = (0.125)*_b[82];
		const Real _t270 = (0.03125)*_b[63];
		const Real _t271 = (0.1875)*_b[79] + _t185;
		const Real _t272 = (0.03125)*_b[48];
		const Real _t273 = (0.15625)*_b[63];
		const Real _t274 = (0.5)*_b[64];
		const Real _t275 = (0.25)*_b[51];
		const Real _t276 = (0.5)*_b[65];
		const Real _t277 = (0.25)*_b[74];
		const Real _t278 = (0.125)*_b[80];
		const Real _t279 = (0.0625)*_b[53];
		const Real _t280 = (0.25)*_b[67];
		const Real _t281 = (0.375)*_b[76];
		const Real _t282 = (0.25)*_b[81];
		const Real _t283 = (0.0625)*_b[83];
		const Real _t284 = (0.125)*_b[66];
		const Real _t285 = _t223 + _t242;
		const Real _t286 = _t254 + _t283;
		const Real _t287 = (0.25)*_b[58];
		const Real _t288 = (0.5)*_b[68];
		const Real _t289 = (0.125)*_b[71];
		const Real _t290 = (0.0625)*_b[79];
		const Real _t291 = (0.0625)*_b[76];
		const Real _t292 = (0.0625)*_b[60];
		const Real _t293 = (0.125)*_b[70];
		const Real _t294 = _t269 + _t293;
		const Real _t295 = (0.25)*_b[78];
		const Real _t296 = (0.125)*_b[72];
		const Real _t297 = _t233 + _t295 + _t296;
		const Real _t298 = (0.0625)*_b[63];
		const Real _t299 = (0.25)*_b[73];
		const Real _t300 = (0.375)*_b[79];
		const Real _t301 = (0.25)*_b[82];
		const Real _t302 = (0.5)*_b[74];
		const Real _t303 = (0.25)*_b[66];
		const Real _t304 = (0.5)*_b[75];
		const Real _t305 = (0.25)*_b[80];
		const Real _t306 = (0.125)*_b[83];
		const Real _t307 = _t282 + _t295;
		const Real _t308 = (0.25)*_b[71];
		const Real _t309 = (0.5)*_b[77];
		const Real _t310 = (0.5)*_b[80];
		const Real _t311 = (0.5)*_b[81];
		const Real _t312 = (0.25)*_b[83];
		const Real _t313 = (0.5)*_b[82];
		const Real _t314 = (0.5)*_b[83];
		const Real _t315 = _t0 + _t25;
		const Real _t316 = (0.5)*_b[8];
		const Real _t317 = _t316 + _t4 + _t72;
		const Real _t318 = (0.375)*_b[14] + (0.375)*_b[9] + _t132 + _t7;
		const Real _t319 = (0.25)*_b[10];
		const Real _t320 = (0.375)*_b[15];
		const Real _t321 = (0.25)*_b[19];
		const Real _t322 = _t11 + _t165 + _t319 + _t320 + _t321;
		const Real _t323 = (0.15625)*_b[11] + (0.3125)*_b[16] + (0.3125)*_b[20] + (0.15625)*_b[23] + _t16 + _t197;
		const Real _t324 = (0.09375)*_b[12];
		const Real _t325 = (0.3125)*_b[21];
		const Real _t326 = (0.09375)*_b[26];
		const Real _t327 = (0.234375)*_b[17] + (0.234375)*_b[24] + _t209 + _t21 + _t324 + _t325 + _t326;
		const Real _t328 = (0.5)*_b[13];
		const Real _t329 = (0.5)*_b[14];
		const Real _t330 = (0.25)*_b[18];
		const Real _t331 = (0.25)*_b[9];
		const Real _t332 = (0.125)*_b[10];
		const Real _t333 = (0.125)*_b[22];
		const Real _t334 = (0.0625)*_b[11];
		const Real _t335 = (0.25)*_b[16];
		const Real _t336 = (0.375)*_b[20];
		const Real _t337 = (0.25)*_b[23];
		const Real _t338 = (0.0625)*_b[25];
		const Real _t339 = (0.03125)*_b[12];
		const Real _t340 = (0.15625)*_b[17];
		const Real _t341 = (0.03125)*_b[27];
		const Real _t342 = (0.5)*_b[18];
		const Real _t343 = (0.25)*_b[15];
		const Real _t344 = (0.5)*_b[19];
		const Real _t345 = (0.25)*_b[22];
		const Real _t346 = (0.125)*_b[16];
		const Real _t347 = (0.125)*_b[25];
		const Real _t348 = (0.0625)*_b[17];
		const Real _t349 = (0.25)*_b[21];
		const Real _t350 = (0.375)*_b[24];
		const Real _t351 = (0.25)*_b[26];
		const Real _t352 = (0.0625)*_b[27];
		const Real _t353 = (0.5)*_b[22];
		const Real _t354 = (0.25)*_b[20];
		const Real _t355 = (0.5)*_b[23];
		const Real _t356 = (0.25)*_b[25];
		const Real _t357 = (0.125)*_b[21];
		const Real _t358 = (0.125)*_b[27];
		const Real _t359 = (0.5)*_b[25];
		const Real _t360 = (0.5)*_b[26];
		const Real _t361 = (0.25)*_b[27];
		const Real _t362 = (0.5)*_b[27];
		const Real _t363 = _t27 + _t72;
		const Real _t364 = (0.25)*_b[14];
		const Real _t365 = _t30 + _t364;
		const Real _t366 = _t132 + _t35 + _t365 + _t77;
		const Real _t367 = (0.1875)*_b[15];
		const Real _t368 = (0.1875)*_b[19] + _t136 + _t165 + _t367 + _t43;
		const Real _t369 = (0.125)*_b[23];
		const Real _t370 = (0.1875)*_b[41];
		const Real _t371 = (0.1875)*_b[20] + _t370;
		const Real _t372 = _t174 + _t57;
		const Real _t373 = _t197 + _t346 + _t369 + _t371 + _t372;
		const Real _t374 = (0.15625)*_b[24];
		const Real _t375 = (0.15625)*_b[42];
		const Real _t376 = (0.15625)*_b[45];
		const Real _t377 = (0.15625)*_b[21] + _t375 + _t376;
		const Real _t378 = (0.078125)*_b[17] + (0.078125)*_b[26] + _t202 + _t209 + _t374 + _t377 + _t66;
		const Real _t379 = (0.125)*_b[15];
		const Real _t380 = (0.25)*_b[40];
		const Real _t381 = _t321 + _t380;
		const Real _t382 = _t256 + _t83;
		const Real _t383 = (0.0625)*_b[16];
		const Real _t384 = (0.1875)*_b[44];
		const Real _t385 = _t338 + _t384;
		const Real _t386 = _t264 + _t96;
		const Real _t387 = (0.125)*_b[26];
		const Real _t388 = (0.03125)*_b[17];
		const Real _t389 = (0.1875)*_b[45];
		const Real _t390 = (0.1875)*_b[24] + _t389;
		const Real _t391 = (0.125)*_b[47];
		const Real _t392 = _t341 + _t391;
		const Real _t393 = (0.125)*_b[42];
		const Real _t394 = _t114 + _t272 + _t393;
		const Real _t395 = (0.5)*_b[39];
		const Real _t396 = (0.125)*_b[20];
		const Real _t397 = (0.25)*_b[44];
		const Real _t398 = _t337 + _t397;
		const Real _t399 = (0.125)*_b[41];
		const Real _t400 = (0.125)*_b[46];
		const Real _t401 = _t399 + _t400;
		const Real _t402 = (0.0625)*_b[21];
		const Real _t403 = (0.1875)*_b[47];
		const Real _t404 = _t352 + _t403;
		const Real _t405 = (0.0625)*_b[48];
		const Real _t406 = _t124 + _t405;
		const Real _t407 = (0.5)*_b[43];
		const Real _t408 = (0.25)*_b[46];
		const Real _t409 = (0.25)*_b[47];
		const Real _t410 = _t351 + _t409;
		const Real _t411 = (0.125)*_b[45];
		const Real _t412 = (0.125)*_b[48];
		const Real _t413 = _t411 + _t412;
		const Real _t414 = (0.5)*_b[46];
		const Real _t415 = (0.25)*_b[48];
		const Real _t416 = (0.5)*_b[48];
		const Real _t417 = _t32 + _t79;
		const Real _t418 = _t132 + _t237 + _t417 + _t76;
		const Real _t419 = (0.125)*_b[19];
		const Real _t420 = (0.125)*_b[55];
		const Real _t421 = _t380 + _t419 + _t420;
		const Real _t422 = _t165 + _t382 + _t421 + _t93;
		const Real _t423 = _t197 + _t384;
		const Real _t424 = (0.09375)*_b[20] + _t147 + _t370;
		const Real _t425 = (0.09375)*_b[23] + _t104 + _t386 + _t423 + _t424;
		const Real _t426 = (0.0625)*_b[26];
		const Real _t427 = (0.09375)*_b[24] + _t389;
		const Real _t428 = _t209 + _t391;
		const Real _t429 = _t250 + _t260 + _t402;
		const Real _t430 = _t120 + _t189 + _t394 + _t426 + _t427 + _t428 + _t429;
		const Real _t431 = _t243 + _t369 + _t397;
		const Real _t432 = (0.0625)*_b[20];
		const Real _t433 = _t257 + _t432;
		const Real _t434 = (0.09375)*_b[62];
		const Real _t435 = _t341 + _t403;
		const Real _t436 = (0.03125)*_b[21];
		const Real _t437 = _t126 + _t436;
		const Real _t438 = (0.0625)*_b[24];
		const Real _t439 = _t181 + _t387 + _t409;
		const Real _t440 = _t38 + _t44;
		const Real _t441 = (0.1875)*_b[43] + _t134 + _t135 + _t139 + _t165 + _t440;
		const Real _t442 = (0.0625)*_b[23];
		const Real _t443 = (0.0625)*_b[69];
		const Real _t444 = _t442 + _t443;
		const Real _t445 = (0.09375)*_b[61] + _t144 + _t146;
		const Real _t446 = (0.09375)*_b[46] + _t107 + _t151 + _t423 + _t444 + _t445;
		const Real _t447 = (0.140625)*_b[62] + (0.046875)*_b[63];
		const Real _t448 = (0.046875)*_b[70] + (0.046875)*_b[72];
		const Real _t449 = (0.046875)*_b[24] + (0.140625)*_b[45] + _t448;
		const Real _t450 = (0.046875)*_b[26] + (0.140625)*_b[47] + (0.046875)*_b[48] + _t160 + _t209 + _t447 + _t449;
		const Real _t451 = (0.1875)*_b[61] + _t149;
		const Real _t452 = (0.03125)*_b[24];
		const Real _t453 = (0.09375)*_b[45];
		const Real _t454 = _t125 + _t426;
		const Real _t455 = (0.1875)*_b[62];
		const Real _t456 = (0.09375)*_b[63] + _t455;
		const Real _t457 = (0.375)*_b[61];
		const Real _t458 = (0.1875)*_b[63] + _t455;
		const Real _t459 = (0.375)*_b[63];
		const Real _t460 = _t51 + _t53;
		const Real _t461 = _t173 + _t177 + _t197 + _t289 + _t400 + _t451 + _t460;
		const Real _t462 = (0.03125)*_b[26];
		const Real _t463 = (0.03125)*_b[78];
		const Real _t464 = _t296 + _t462 + _t463;
		const Real _t465 = _t128 + _t187 + _t195 + _t405 + _t428 + _t456 + _t464;
		const Real _t466 = _t61 + _t67;
		const Real _t467 = (0.078125)*_b[48] + (0.078125)*_b[79] + _t200 + _t201 + _t207 + _t209 + _t273 + _t466;
		const Real _t468 = (0.5)*_b[2];
		const Real _t469 = (0.5)*_b[3];
		const Real _t470 = (0.5)*_b[9];
		const Real _t471 = (0.5)*_b[10];
		const Real _t472 = (0.5)*_b[4];
		const Real _t473 = (0.5)*_b[11];
		const Real _t474 = (0.5)*_b[5];
		const Real _t475 = (0.5)*_b[12];
		const Real _t476 = (0.5)*_b[6];
		const Real _t477 = (0.25)*_b[3];
		const Real _t478 = (0.25)*_b[4];
		const Real _t479 = (0.25)*_b[5];
		const Real _t480 = (0.25)*_b[6];
		const Real _t481 = (0.125)*_b[4];
		const Real _t482 = (0.375)*_b[16];
		const Real _t483 = (0.125)*_b[5];
		const Real _t484 = (0.375)*_b[17];
		const Real _t485 = (0.125)*_b[6];
		const Real _t486 = (0.25)*_b[11];
		const Real _t487 = (0.0625)*_b[5];
		const Real _t488 = (0.25)*_b[12];
		const Real _t489 = (0.0625)*_b[6];
		const Real _t490 = (0.03125)*_b[6];
		const Real _t491 = (0.5)*_b[30];
		const Real _t492 = (0.5)*_b[31];
		const Real _t493 = (0.5)*_b[32];
		const Real _t494 = (0.5)*_b[33];
		const Real _t495 = _t27 + _t4;
		const Real _t496 = _t30 + _t331;
		const Real _t497 = (0.25)*_b[36];
		const Real _t498 = _t319 + _t497;
		const Real _t499 = (0.25)*_b[32];
		const Real _t500 = (0.25)*_b[37];
		const Real _t501 = _t486 + _t500;
		const Real _t502 = (0.25)*_b[33];
		const Real _t503 = (0.25)*_b[38];
		const Real _t504 = _t488 + _t503;
		const Real _t505 = _t33 + _t496 + _t7 + _t78;
		const Real _t506 = _t218 + _t87;
		const Real _t507 = (0.125)*_b[32];
		const Real _t508 = _t399 + _t507;
		const Real _t509 = (0.125)*_b[33];
		const Real _t510 = _t393 + _t509;
		const Real _t511 = (0.1875)*_b[10] + _t11 + _t137 + _t367 + _t42;
		const Real _t512 = (0.1875)*_b[37];
		const Real _t513 = _t487 + _t512;
		const Real _t514 = (0.1875)*_b[16] + _t370;
		const Real _t515 = _t142 + _t220;
		const Real _t516 = (0.1875)*_b[38];
		const Real _t517 = _t489 + _t516;
		const Real _t518 = (0.1875)*_b[42];
		const Real _t519 = (0.1875)*_b[17] + _t518;
		const Real _t520 = (0.0625)*_b[33];
		const Real _t521 = _t192 + _t520;
		const Real _t522 = (0.125)*_b[11];
		const Real _t523 = _t174 + _t175;
		const Real _t524 = _t16 + _t396 + _t514 + _t522 + _t523 + _t56;
		const Real _t525 = (0.125)*_b[12];
		const Real _t526 = (0.125)*_b[38];
		const Real _t527 = _t490 + _t526;
		const Real _t528 = _t183 + _t225 + _t411;
		const Real _t529 = (0.078125)*_b[12] + (0.078125)*_b[24] + _t203 + _t21 + _t340 + _t377 + _t65;
		const Real _t530 = _t215 + _t34 + _t417 + _t7;
		const Real _t531 = _t332 + _t420 + _t497;
		const Real _t532 = _t240 + _t500 + _t522;
		const Real _t533 = _t113 + _t503 + _t525;
		const Real _t534 = _t11 + _t506 + _t531 + _t93;
		const Real _t535 = _t230 + _t383;
		const Real _t536 = _t16 + _t512;
		const Real _t537 = (0.09375)*_b[16] + _t103;
		const Real _t538 = (0.09375)*_b[11] + _t148 + _t370 + _t515 + _t536 + _t537;
		const Real _t539 = (0.09375)*_b[57];
		const Real _t540 = _t490 + _t516;
		const Real _t541 = (0.09375)*_b[17] + _t518;
		const Real _t542 = (0.0625)*_b[12];
		const Real _t543 = _t21 + _t526;
		const Real _t544 = _t119 + _t190 + _t429 + _t528 + _t541 + _t542 + _t543;
		const Real _t545 = (0.375)*_b[52];
		const Real _t546 = (0.375)*_b[53];
		const Real _t547 = (0.1875)*_b[31] + _t11 + _t138 + _t38 + _t40 + _t41 + _t45;
		const Real _t548 = (0.1875)*_b[52] + _t49;
		const Real _t549 = _t334 + _t443;
		const Real _t550 = (0.1875)*_b[57];
		const Real _t551 = (0.1875)*_b[53] + _t550;
		const Real _t552 = _t193 + _t542;
		const Real _t553 = _t100 + _t150;
		const Real _t554 = (0.09375)*_b[52] + _t102;
		const Real _t555 = (0.09375)*_b[32] + _t108 + _t536 + _t549 + _t553 + _t554;
		const Real _t556 = (0.09375)*_b[42];
		const Real _t557 = (0.09375)*_b[53] + _t550;
		const Real _t558 = (0.046875)*_b[53] + (0.140625)*_b[57] + _t158 + _t162;
		const Real _t559 = (0.046875)*_b[17] + (0.140625)*_b[42] + _t157 + _t448;
		const Real _t560 = (0.046875)*_b[12] + (0.046875)*_b[33] + (0.140625)*_b[38] + _t21 + _t558 + _t559;
		const Real _t561 = _t176 + _t460;
		const Real _t562 = _t16 + _t284 + _t507 + _t54 + _t548 + _t55 + _t561;
		const Real _t563 = _t293 + _t339 + _t463;
		const Real _t564 = _t118 + _t130 + _t194 + _t520 + _t543 + _t557 + _t563;
		const Real _t565 = (0.078125)*_b[33] + (0.078125)*_b[76] + _t204 + _t21 + _t226 + _t61 + _t63 + _t64 + _t70;
		const Real _t566 = _t138 + _t440;
		const Real _t567 = _t194 + _t203;
		const Real _t568 = _t204 + _t466;
		const Real _t569 = _t187 + _t568;
		const Real _t570 = _t43 + _t89;
		const Real _t571 = (0.15625)*_b[59] + _t101 + _t96;
		const Real _t572 = (0.15625)*_b[56] + _t142 + _t144 + _t145;
		const Real _t573 = (0.109375)*_b[62];
		const Real _t574 = (0.109375)*_b[70] + _t156;
		const Real _t575 = _t183 + _t188;
		const Real _t576 = (0.046875)*_b[73] + _t154 + _t159;
		const Real _t577 = (0.109375)*_b[57];
		const Real _t578 = (0.109375)*_b[72] + _t156;
		const Real _t579 = (0.046875)*_b[67] + _t155;
		const Real _t580 = _t114 + _t120;
		const Real _t581 = (0.15625)*_b[41] + _t105 + _t443;
		const Real _t582 = (0.109375)*_b[45];
		const Real _t583 = _t127 + _t463;
		const Real _t584 = _t194 + _t583;
		const Real _t585 = _t187 + _t575;
		const Real _t586 = (0.109375)*_b[42];
		const Real _t587 = (0.046875)*_b[21];
		#pragma endregion
		#pragma region Expressions
		return {{
			{
				_b[0],
				_t2,
				_t6,
				_t9,
				_t15,
				_t18,
				_t24,
				_t26,
				_t28,
				_t37,
				_t46,
				_t59,
				_t71,
				_t74,
				_t81,
				_t94,
				_t110,
				_t131,
				_t133,
				_t140,
				_t153,
				_t164,
				_t169,
				_t179,
				_t196,
				_t198,
				_t208,
				_t213,
				_b[28],
				_t214 + _t3,
				_t215 + _t216 + _t217,
				(0.375)*_b[65] + _t12 + _t218 + _t219,
				_t220 + _t221 + _t222 + _t223 + _t224,
				(0.3125)*_b[76] + (0.15625)*_b[81] + _t20 + _t225 + _t226 + _t227,
				_t214 + _t73,
				_t217 + _t31 + _t75,
				_t219 + _t228 + _t229 + _t84,
				(0.1875)*_b[75] + _t230 + _t231 + _t50 + _t97,
				_t115 + _t232 + _t233 + _t234 + _t235 + _t236,
				_t217 + _t237 + _t238,
				_t168 + _t219 + _t239 + _t88,
				(0.0625)*_b[41] + _t224 + _t241 + _t244 + _t247,
				(0.03125)*_b[42] + _t117 + _t22 + _t248 + _t249 + _t251 + _t253 + _t255,
				(0.375)*_b[68] + _t167 + _t219 + _t256,
				(0.1875)*_b[77] + _t143 + _t172 + _t231 + _t257,
				(0.03125)*_b[45] + _t186 + _t212 + _t255 + _t258 + _t259 + _t261 + _t263,
				_t224 + _t264 + _t265 + _t266 + _t267,
				_t184 + _t236 + _t268 + _t269 + _t270 + _t271,
				(0.3125)*_b[79] + (0.15625)*_b[82] + _t211 + _t227 + _t272 + _t273,
				_b[49],
				_t216 + _t274,
				_t275 + _t276 + _t277,
				(0.375)*_b[75] + _t222 + _t278 + _t47,
				_t279 + _t280 + _t281 + _t282 + _t283,
				_t238 + _t274,
				_t168 + _t229 + _t277,
				_t241 + _t278 + _t284 + _t285,
				(0.1875)*_b[81] + _t123 + _t235 + _t251 + _t286,
				_t277 + _t287 + _t288,
				_t244 + _t267 + _t278 + _t289,
				_t283 + _t290 + _t291 + _t292 + _t294 + _t297,
				(0.375)*_b[77] + _t170 + _t266 + _t278,
				(0.1875)*_b[82] + _t191 + _t261 + _t271 + _t286,
				_t283 + _t298 + _t299 + _t300 + _t301,
				_b[64],
				_t276 + _t302,
				_t303 + _t304 + _t305,
				(0.375)*_b[81] + _t232 + _t281 + _t306,
				_t288 + _t302,
				_t267 + _t285 + _t305,
				(0.125)*_b[76] + _t294 + _t306 + _t307,
				_t305 + _t308 + _t309,
				(0.125)*_b[79] + _t297 + _t301 + _t306,
				(0.375)*_b[82] + _t268 + _t300 + _t306,
				_b[74],
				_t304 + _t310,
				(0.25)*_b[76] + _t311 + _t312,
				_t309 + _t310,
				_t301 + _t307 + _t312,
				(0.25)*_b[79] + _t312 + _t313,
				_b[80],
				_t311 + _t314,
				_t313 + _t314,
				_b[83],
			},
			{
				_b[0],
				_t315,
				_t317,
				_t318,
				_t322,
				_t323,
				_t327,
				_b[7],
				_t316 + _t328,
				_t329 + _t330 + _t331,
				(0.375)*_b[19] + _t320 + _t332 + _t333,
				_t334 + _t335 + _t336 + _t337 + _t338,
				(0.3125)*_b[24] + (0.15625)*_b[26] + _t325 + _t339 + _t340 + _t341,
				_b[13],
				_t329 + _t342,
				_t343 + _t344 + _t345,
				(0.375)*_b[23] + _t336 + _t346 + _t347,
				_t348 + _t349 + _t350 + _t351 + _t352,
				_b[18],
				_t344 + _t353,
				_t354 + _t355 + _t356,
				(0.375)*_b[26] + _t350 + _t357 + _t358,
				_b[22],
				_t355 + _t359,
				(0.25)*_b[24] + _t360 + _t361,
				_b[25],
				_t360 + _t362,
				_b[27],
				_t26,
				_t363,
				_t366,
				_t368,
				_t373,
				_t378,
				_t328 + _t73,
				_t237 + _t330 + _t365,
				_t333 + _t379 + _t381 + _t382,
				(0.1875)*_b[23] + _t371 + _t383 + _t385 + _t386,
				_t357 + _t387 + _t388 + _t390 + _t392 + _t394,
				_t342 + _t395,
				_t166 + _t345 + _t381,
				_t347 + _t396 + _t398 + _t401,
				(0.1875)*_b[26] + _t390 + _t402 + _t404 + _t406,
				_t353 + _t407,
				_t356 + _t398 + _t408,
				(0.125)*_b[24] + _t358 + _t410 + _t413,
				_t359 + _t414,
				_t361 + _t410 + _t415,
				_t362 + _t416,
				_t74,
				_t418,
				_t422,
				_t425,
				_t430,
				_t330 + _t395 + _t75,
				_t166 + _t239 + _t333 + _t421,
				(0.0625)*_b[56] + _t338 + _t401 + _t431 + _t433,
				(0.03125)*_b[57] + _t270 + _t326 + _t406 + _t427 + _t434 + _t435 + _t437,
				_t287 + _t345 + _t407,
				_t170 + _t347 + _t408 + _t431,
				_t292 + _t298 + _t352 + _t413 + _t438 + _t439,
				_t265 + _t356 + _t414,
				(0.125)*_b[63] + _t358 + _t415 + _t439,
				(0.25)*_b[63] + _t361 + _t416,
				_t133,
				_t441,
				_t446,
				_t450,
				(0.375)*_b[43] + _t167 + _t333 + _t82,
				(0.1875)*_b[46] + _t245 + _t385 + _t444 + _t451,
				(0.03125)*_b[70] + _t210 + _t263 + _t435 + _t452 + _t453 + _t454 + _t456,
				(0.375)*_b[46] + _t289 + _t347 + _t457,
				(0.1875)*_b[48] + _t191 + _t404 + _t454 + _t458,
				(0.375)*_b[48] + _t268 + _t358 + _t459,
				_t169,
				_t461,
				_t465,
				_t308 + _t338 + _t408 + _t457 + _t95,
				_t248 + _t268 + _t392 + _t412 + _t458 + _t464,
				_t290 + _t299 + _t352 + _t415 + _t459,
				_t198,
				_t467,
				(0.15625)*_b[48] + (0.3125)*_b[63] + _t112 + _t199 + _t211 + _t341,
				_t213,
			},
			{
				_b[0],
				_b[1],
				_b[2],
				_b[3],
				_b[4],
				_b[5],
				_b[6],
				_t315,
				_t316 + _t468,
				_t469 + _t470,
				_t471 + _t472,
				_t473 + _t474,
				_t475 + _t476,
				_t317,
				_t364 + _t470 + _t477,
				_t343 + _t471 + _t478,
				_t335 + _t473 + _t479,
				(0.25)*_b[17] + _t475 + _t480,
				_t318,
				(0.375)*_b[10] + _t320 + _t419 + _t481,
				(0.375)*_b[11] + _t396 + _t482 + _t483,
				(0.375)*_b[12] + _t357 + _t484 + _t485,
				_t322,
				_t354 + _t442 + _t482 + _t486 + _t487,
				_t349 + _t438 + _t484 + _t488 + _t489,
				_t323,
				(0.15625)*_b[12] + (0.3125)*_b[17] + _t325 + _t374 + _t462 + _t490,
				_t327,
				_t2,
				_t3 + _t468,
				_t469 + _t491,
				_t472 + _t492,
				_t474 + _t493,
				_t476 + _t494,
				_t495,
				_t215 + _t477 + _t496,
				_t10 + _t478 + _t498,
				_t479 + _t499 + _t501,
				_t480 + _t502 + _t504,
				_t505,
				_t379 + _t481 + _t498 + _t506,
				_t346 + _t483 + _t501 + _t508,
				(0.125)*_b[17] + _t485 + _t504 + _t510,
				_t511,
				(0.1875)*_b[11] + _t432 + _t513 + _t514 + _t515,
				(0.1875)*_b[12] + _t402 + _t517 + _t519 + _t521,
				_t524,
				_t357 + _t452 + _t519 + _t525 + _t527 + _t528,
				_t529,
				_t6,
				_t29 + _t477 + _t491,
				_t275 + _t478 + _t492,
				_t221 + _t479 + _t493,
				(0.25)*_b[53] + _t480 + _t494,
				_t530,
				_t10 + _t228 + _t481 + _t531,
				_t47 + _t483 + _t499 + _t532,
				(0.125)*_b[53] + _t485 + _t502 + _t533,
				_t534,
				(0.0625)*_b[59] + _t487 + _t508 + _t532 + _t535,
				_t279 + _t292 + _t348 + _t489 + _t510 + _t533,
				_t538,
				(0.03125)*_b[62] + _t234 + _t324 + _t437 + _t521 + _t539 + _t540 + _t541,
				_t544,
				_t9,
				(0.375)*_b[31] + _t12 + _t481 + _t86,
				(0.375)*_b[32] + _t284 + _t483 + _t545,
				(0.375)*_b[33] + _t232 + _t485 + _t546,
				_t547,
				(0.1875)*_b[32] + _t246 + _t513 + _t548 + _t549,
				(0.1875)*_b[33] + _t123 + _t517 + _t551 + _t552,
				_t555,
				(0.03125)*_b[72] + _t19 + _t253 + _t388 + _t540 + _t552 + _t556 + _t557,
				_t560,
				_t15,
				_t141 + _t303 + _t487 + _t499 + _t545,
				_t280 + _t291 + _t489 + _t502 + _t546,
				_t562,
				_t232 + _t258 + _t509 + _t527 + _t551 + _t563,
				_t564,
				_t18,
				(0.15625)*_b[33] + (0.3125)*_b[53] + _t182 + _t20 + _t490 + _t60,
				_t565,
				_t24,
			},
			{
				_b[0],
				_t26,
				_t74,
				_t133,
				_t169,
				_t198,
				_t213,
				_t2,
				_t28,
				_t81,
				_t140,
				_t179,
				_t208,
				_t6,
				_t37,
				_t94,
				_t153,
				_t196,
				_t9,
				_t46,
				_t110,
				_t164,
				_t15,
				_t59,
				_t131,
				_t18,
				_t71,
				_t24,
				_t315,
				_t363,
				_t418,
				_t441,
				_t461,
				_t467,
				_t495,
				_t36 + _t80,
				_t137 + _t239 + _t566 + _t83 + _t92,
				_t152 + _t178 + _t445 + _t460,
				(0.09375)*_b[73] + _t126 + _t192 + _t193 + _t205 + _t206 + _t290 + _t298 + _t567 + _t569,
				_t530,
				_t228 + _t566 + _t570 + _t87 + _t90,
				_t100 + _t247 + _t399 + _t561 + _t571 + _t572,
				(0.078125)*_b[45] + _t161 + _t258 + _t569 + _t573 + _t574 + _t575 + _t576,
				_t547,
				_t100 + _t109 + _t176 + _t51 + _t554 + _t58,
				(0.078125)*_b[42] + _t158 + _t163 + _t248 + _t568 + _t577 + _t578 + _t579 + _t580,
				_t562,
				(0.09375)*_b[67] + _t118 + _t129 + _t279 + _t291 + _t568 + _t66 + _t68 + _t69,
				_t565,
				_t317,
				_t366,
				_t422,
				_t446,
				_t465,
				_t505,
				_t137 + _t379 + _t420 + _t570 + _t91,
				_t240 + _t433 + _t523 + _t553 + _t571 + _t581,
				(0.078125)*_b[70] + _t447 + _t452 + _t567 + _t576 + _t578 + _t582 + _t583,
				_t534,
				_t150 + _t243 + _t535 + _t57 + _t572 + _t581,
				(0.15625)*_b[60] + _t249 + _t252 + _t259 + _t262 + _t434 + _t436 + _t453 + _t539 + _t556 + _t580 + _t584 + _t585,
				_t555,
				(0.078125)*_b[72] + _t388 + _t558 + _t574 + _t579 + _t584 + _t586 + _t66,
				_t564,
				_t318,
				_t368,
				_t425,
				_t450,
				_t511,
				_t175 + _t372 + _t424 + _t537,
				(0.078125)*_b[57] + _t157 + _t203 + _t270 + _t449 + _t573 + _t580 + _t586 + _t587,
				_t538,
				(0.078125)*_b[62] + _t234 + _t559 + _t577 + _t582 + _t585 + _t587 + _t66,
				_t560,
				_t322,
				_t373,
				_t430,
				_t524,
				(0.09375)*_b[21] + _t119 + _t189 + _t203 + _t250 + _t260 + _t348 + _t375 + _t376 + _t438 + _t66,
				_t544,
				_t323,
				_t378,
				_t529,
				_t327,
			},
		}};
		#pragma endregion
	}
}
