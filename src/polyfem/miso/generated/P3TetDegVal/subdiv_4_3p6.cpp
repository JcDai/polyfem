#include "../P3TetDegVal.hpp"
namespace miso {
	std::array<RealVector<84>, 4> P3TetDegVal::subdiv_4_3p6(const RealVector<84> &_b) {
		#pragma region Temporaries
		const Real _t0 = (0.5)*_b[0];
		const Real _t1 = (0.5)*_b[7];
		const Real _t2 = (0.5)*_b[1];
		const Real _t3 = (0.5)*_b[8];
		const Real _t4 = (0.5)*_b[2];
		const Real _t5 = (0.5)*_b[9];
		const Real _t6 = (0.5)*_b[10];
		const Real _t7 = (0.5)*_b[3];
		const Real _t8 = (0.5)*_b[11];
		const Real _t9 = (0.5)*_b[4];
		const Real _t10 = (0.5)*_b[12];
		const Real _t11 = (0.5)*_b[5];
		const Real _t12 = _t10 + _t11;
		const Real _t13 = (0.25)*_b[0];
		const Real _t14 = (0.25)*_b[14];
		const Real _t15 = (0.25)*_b[1];
		const Real _t16 = (0.25)*_b[15];
		const Real _t17 = (0.25)*_b[2];
		const Real _t18 = (0.25)*_b[16];
		const Real _t19 = (0.25)*_b[3];
		const Real _t20 = (0.25)*_b[17];
		const Real _t21 = (0.25)*_b[4];
		const Real _t22 = _t20 + _t21 + _t8;
		const Real _t23 = (0.125)*_b[0];
		const Real _t24 = (0.375)*_b[13];
		const Real _t25 = (0.125)*_b[18];
		const Real _t26 = (0.375)*_b[14];
		const Real _t27 = (0.125)*_b[19];
		const Real _t28 = (0.125)*_b[1];
		const Real _t29 = (0.375)*_b[15];
		const Real _t30 = (0.125)*_b[20];
		const Real _t31 = (0.125)*_b[2];
		const Real _t32 = (0.125)*_b[21];
		const Real _t33 = (0.125)*_b[3];
		const Real _t34 = (0.375)*_b[10] + (0.375)*_b[16] + _t32 + _t33;
		const Real _t35 = (0.0625)*_b[0];
		const Real _t36 = (0.25)*_b[18];
		const Real _t37 = (0.0625)*_b[22];
		const Real _t38 = (0.25)*_b[7];
		const Real _t39 = (0.25)*_b[19];
		const Real _t40 = (0.0625)*_b[1];
		const Real _t41 = (0.0625)*_b[23];
		const Real _t42 = (0.25)*_b[8];
		const Real _t43 = (0.25)*_b[20];
		const Real _t44 = (0.0625)*_b[24];
		const Real _t45 = (0.0625)*_b[2];
		const Real _t46 = (0.25)*_b[9];
		const Real _t47 = _t29 + _t43 + _t44 + _t45 + _t46;
		const Real _t48 = (0.03125)*_b[0];
		const Real _t49 = (0.3125)*_b[18];
		const Real _t50 = (0.15625)*_b[22];
		const Real _t51 = (0.03125)*_b[25];
		const Real _t52 = (0.03125)*_b[1];
		const Real _t53 = (0.03125)*_b[26];
		const Real _t54 = (0.3125)*_b[14] + (0.3125)*_b[19] + (0.15625)*_b[23] + (0.15625)*_b[8] + _t52 + _t53;
		const Real _t55 = (0.015625)*_b[0];
		const Real _t56 = (0.09375)*_b[25];
		const Real _t57 = (0.015625)*_b[27];
		const Real _t58 = (0.09375)*_b[7];
		const Real _t59 = (0.234375)*_b[13] + (0.234375)*_b[22] + _t49 + _t55 + _t56 + _t57 + _t58;
		const Real _t60 = (0.5)*_b[28];
		const Real _t61 = (0.5)*_b[29];
		const Real _t62 = (0.5)*_b[30];
		const Real _t63 = (0.5)*_b[31];
		const Real _t64 = (0.5)*_b[32];
		const Real _t65 = (0.5)*_b[33];
		const Real _t66 = _t11 + _t65;
		const Real _t67 = (0.25)*_b[28];
		const Real _t68 = (0.25)*_b[34];
		const Real _t69 = _t38 + _t68;
		const Real _t70 = (0.25)*_b[29];
		const Real _t71 = (0.25)*_b[35];
		const Real _t72 = _t42 + _t71;
		const Real _t73 = (0.25)*_b[30];
		const Real _t74 = (0.25)*_b[36];
		const Real _t75 = _t46 + _t74;
		const Real _t76 = (0.25)*_b[31];
		const Real _t77 = (0.25)*_b[10];
		const Real _t78 = (0.25)*_b[37];
		const Real _t79 = _t77 + _t78;
		const Real _t80 = (0.25)*_b[11] + (0.25)*_b[32] + (0.25)*_b[38];
		const Real _t81 = _t21 + _t80;
		const Real _t82 = (0.125)*_b[28];
		const Real _t83 = (0.125)*_b[39];
		const Real _t84 = _t82 + _t83;
		const Real _t85 = (0.125)*_b[14];
		const Real _t86 = (0.125)*_b[29];
		const Real _t87 = (0.125)*_b[40];
		const Real _t88 = _t86 + _t87;
		const Real _t89 = (0.125)*_b[15];
		const Real _t90 = (0.125)*_b[30];
		const Real _t91 = (0.125)*_b[41];
		const Real _t92 = _t90 + _t91;
		const Real _t93 = (0.125)*_b[31];
		const Real _t94 = (0.125)*_b[16];
		const Real _t95 = (0.125)*_b[42];
		const Real _t96 = _t94 + _t95;
		const Real _t97 = _t93 + _t96;
		const Real _t98 = _t33 + _t79 + _t97;
		const Real _t99 = (0.0625)*_b[18];
		const Real _t100 = (0.1875)*_b[34];
		const Real _t101 = _t100 + _t35;
		const Real _t102 = (0.1875)*_b[39];
		const Real _t103 = (0.1875)*_b[13] + _t102;
		const Real _t104 = (0.0625)*_b[28];
		const Real _t105 = (0.0625)*_b[43];
		const Real _t106 = _t104 + _t105;
		const Real _t107 = (0.0625)*_b[19];
		const Real _t108 = (0.1875)*_b[35];
		const Real _t109 = _t108 + _t40;
		const Real _t110 = (0.1875)*_b[40];
		const Real _t111 = (0.1875)*_b[14] + _t110;
		const Real _t112 = (0.0625)*_b[29];
		const Real _t113 = (0.0625)*_b[44];
		const Real _t114 = _t112 + _t113;
		const Real _t115 = (0.1875)*_b[36];
		const Real _t116 = _t115 + _t45;
		const Real _t117 = (0.0625)*_b[45];
		const Real _t118 = (0.1875)*_b[41];
		const Real _t119 = (0.0625)*_b[20] + _t118;
		const Real _t120 = _t117 + _t119;
		const Real _t121 = (0.0625)*_b[30];
		const Real _t122 = (0.1875)*_b[15] + _t121;
		const Real _t123 = (0.1875)*_b[9] + _t116 + _t120 + _t122;
		const Real _t124 = (0.125)*_b[7];
		const Real _t125 = (0.03125)*_b[22];
		const Real _t126 = (0.125)*_b[34];
		const Real _t127 = _t126 + _t48;
		const Real _t128 = (0.03125)*_b[28];
		const Real _t129 = (0.125)*_b[43];
		const Real _t130 = (0.03125)*_b[46];
		const Real _t131 = _t128 + _t129 + _t130;
		const Real _t132 = (0.125)*_b[8];
		const Real _t133 = (0.125)*_b[35];
		const Real _t134 = _t133 + _t52;
		const Real _t135 = (0.03125)*_b[29];
		const Real _t136 = (0.03125)*_b[47];
		const Real _t137 = (0.125)*_b[44];
		const Real _t138 = (0.03125)*_b[23] + _t137;
		const Real _t139 = _t136 + _t138;
		const Real _t140 = _t135 + _t139;
		const Real _t141 = _t111 + _t132 + _t134 + _t140 + _t27;
		const Real _t142 = (0.15625)*_b[13];
		const Real _t143 = (0.078125)*_b[34];
		const Real _t144 = _t143 + _t55;
		const Real _t145 = (0.015625)*_b[48];
		const Real _t146 = (0.078125)*_b[46];
		const Real _t147 = (0.015625)*_b[25] + _t146;
		const Real _t148 = _t145 + _t147;
		const Real _t149 = (0.015625)*_b[28];
		const Real _t150 = (0.15625)*_b[39];
		const Real _t151 = (0.15625)*_b[43];
		const Real _t152 = (0.15625)*_b[18] + _t149 + _t150 + _t151;
		const Real _t153 = (0.078125)*_b[22] + (0.078125)*_b[7] + _t142 + _t144 + _t148 + _t152;
		const Real _t154 = (0.25)*_b[50];
		const Real _t155 = (0.25)*_b[51];
		const Real _t156 = (0.25)*_b[52];
		const Real _t157 = (0.25)*_b[53];
		const Real _t158 = _t157 + _t21 + _t64;
		const Real _t159 = (0.125)*_b[54];
		const Real _t160 = _t124 + _t159 + _t68;
		const Real _t161 = (0.125)*_b[50];
		const Real _t162 = (0.125)*_b[55];
		const Real _t163 = _t132 + _t162 + _t71;
		const Real _t164 = (0.125)*_b[51];
		const Real _t165 = (0.125)*_b[9];
		const Real _t166 = (0.125)*_b[56];
		const Real _t167 = _t165 + _t166 + _t74;
		const Real _t168 = (0.125)*_b[10];
		const Real _t169 = (0.125)*_b[57];
		const Real _t170 = _t168 + _t169;
		const Real _t171 = (0.125)*_b[52] + _t78;
		const Real _t172 = _t170 + _t171;
		const Real _t173 = _t172 + _t33 + _t76;
		const Real _t174 = (0.0625)*_b[49];
		const Real _t175 = (0.0625)*_b[13];
		const Real _t176 = (0.0625)*_b[58];
		const Real _t177 = (0.0625)*_b[50];
		const Real _t178 = (0.0625)*_b[14];
		const Real _t179 = _t177 + _t178;
		const Real _t180 = (0.0625)*_b[60];
		const Real _t181 = (0.0625)*_b[15];
		const Real _t182 = (0.0625)*_b[51];
		const Real _t183 = _t181 + _t182;
		const Real _t184 = _t180 + _t183;
		const Real _t185 = _t167 + _t184 + _t45 + _t92;
		const Real _t186 = (0.03125)*_b[49];
		const Real _t187 = (0.09375)*_b[54];
		const Real _t188 = (0.03125)*_b[18];
		const Real _t189 = (0.09375)*_b[58];
		const Real _t190 = _t188 + _t189;
		const Real _t191 = _t100 + _t48;
		const Real _t192 = (0.09375)*_b[13] + _t102;
		const Real _t193 = _t108 + _t52;
		const Real _t194 = (0.03125)*_b[62];
		const Real _t195 = (0.03125)*_b[19];
		const Real _t196 = (0.09375)*_b[55];
		const Real _t197 = _t195 + _t196;
		const Real _t198 = _t194 + _t197;
		const Real _t199 = (0.03125)*_b[50];
		const Real _t200 = (0.09375)*_b[59];
		const Real _t201 = _t199 + _t200;
		const Real _t202 = _t110 + _t201;
		const Real _t203 = (0.09375)*_b[14] + _t202;
		const Real _t204 = (0.09375)*_b[8] + _t114 + _t193 + _t198 + _t203;
		const Real _t205 = (0.0625)*_b[7];
		const Real _t206 = (0.015625)*_b[22];
		const Real _t207 = (0.015625)*_b[63];
		const Real _t208 = _t206 + _t207;
		const Real _t209 = _t126 + _t55;
		const Real _t210 = (0.0625)*_b[54];
		const Real _t211 = (0.0625)*_b[61];
		const Real _t212 = (0.015625)*_b[49];
		const Real _t213 = _t189 + _t212;
		const Real _t214 = _t210 + _t211 + _t213;
		const Real _t215 = _t214 + _t99;
		const Real _t216 = _t131 + _t192 + _t205 + _t208 + _t209 + _t215;
		const Real _t217 = (0.375)*_b[49];
		const Real _t218 = (0.125)*_b[64];
		const Real _t219 = (0.375)*_b[50];
		const Real _t220 = (0.125)*_b[65];
		const Real _t221 = (0.375)*_b[51];
		const Real _t222 = (0.125)*_b[66];
		const Real _t223 = (0.125)*_b[67];
		const Real _t224 = (0.375)*_b[31] + (0.375)*_b[52] + _t223 + _t33;
		const Real _t225 = (0.0625)*_b[64];
		const Real _t226 = (0.1875)*_b[54];
		const Real _t227 = (0.1875)*_b[49] + _t226;
		const Real _t228 = (0.0625)*_b[68];
		const Real _t229 = _t205 + _t228;
		const Real _t230 = (0.0625)*_b[65];
		const Real _t231 = (0.1875)*_b[55];
		const Real _t232 = (0.1875)*_b[50] + _t231;
		const Real _t233 = (0.0625)*_b[8];
		const Real _t234 = (0.0625)*_b[69];
		const Real _t235 = _t233 + _t234;
		const Real _t236 = (0.0625)*_b[70];
		const Real _t237 = (0.1875)*_b[56];
		const Real _t238 = (0.0625)*_b[66] + _t237;
		const Real _t239 = _t236 + _t238;
		const Real _t240 = (0.0625)*_b[9];
		const Real _t241 = (0.1875)*_b[51] + _t240;
		const Real _t242 = (0.1875)*_b[30] + _t116 + _t239 + _t241;
		const Real _t243 = (0.09375)*_b[28];
		const Real _t244 = (0.03125)*_b[13];
		const Real _t245 = (0.09375)*_b[39];
		const Real _t246 = (0.03125)*_b[64];
		const Real _t247 = _t189 + _t246;
		const Real _t248 = (0.09375)*_b[49] + _t226;
		const Real _t249 = (0.03125)*_b[72];
		const Real _t250 = (0.03125)*_b[65];
		const Real _t251 = (0.09375)*_b[40];
		const Real _t252 = _t250 + _t251;
		const Real _t253 = _t249 + _t252;
		const Real _t254 = (0.03125)*_b[14];
		const Real _t255 = _t231 + _t254;
		const Real _t256 = (0.09375)*_b[50] + _t200 + _t255;
		const Real _t257 = (0.09375)*_b[29] + _t193 + _t235 + _t253 + _t256;
		const Real _t258 = (0.015625)*_b[18];
		const Real _t259 = (0.015625)*_b[64];
		const Real _t260 = (0.140625)*_b[58];
		const Real _t261 = _t258 + _t259 + _t260;
		const Real _t262 = (0.015625)*_b[73];
		const Real _t263 = (0.046875)*_b[43] + (0.046875)*_b[61] + _t262;
		const Real _t264 = _t261 + _t263;
		const Real _t265 = (0.046875)*_b[49] + (0.140625)*_b[54];
		const Real _t266 = (0.046875)*_b[68] + (0.046875)*_b[71];
		const Real _t267 = (0.046875)*_b[13] + (0.140625)*_b[39] + _t266;
		const Real _t268 = (0.046875)*_b[28] + (0.140625)*_b[34] + (0.046875)*_b[7] + _t264 + _t265 + _t267 + _t55;
		const Real _t269 = (0.25)*_b[64];
		const Real _t270 = (0.0625)*_b[74];
		const Real _t271 = (0.25)*_b[65];
		const Real _t272 = (0.0625)*_b[75];
		const Real _t273 = (0.25)*_b[66];
		const Real _t274 = (0.0625)*_b[76];
		const Real _t275 = _t221 + _t273 + _t274 + _t45 + _t73;
		const Real _t276 = (0.03125)*_b[74];
		const Real _t277 = (0.125)*_b[68];
		const Real _t278 = (0.03125)*_b[7];
		const Real _t279 = (0.03125)*_b[77];
		const Real _t280 = _t277 + _t278 + _t279;
		const Real _t281 = (0.125)*_b[69];
		const Real _t282 = (0.03125)*_b[75] + _t281;
		const Real _t283 = (0.03125)*_b[78];
		const Real _t284 = (0.03125)*_b[8];
		const Real _t285 = _t283 + _t284;
		const Real _t286 = _t282 + _t285;
		const Real _t287 = _t134 + _t220 + _t232 + _t286 + _t86;
		const Real _t288 = (0.015625)*_b[79];
		const Real _t289 = (0.015625)*_b[74];
		const Real _t290 = _t189 + _t289;
		const Real _t291 = _t288 + _t290;
		const Real _t292 = (0.0625)*_b[39];
		const Real _t293 = (0.0625)*_b[71];
		const Real _t294 = (0.015625)*_b[13];
		const Real _t295 = _t225 + _t292 + _t293 + _t294;
		const Real _t296 = _t104 + _t209 + _t248 + _t280 + _t291 + _t295;
		const Real _t297 = (0.3125)*_b[64];
		const Real _t298 = (0.15625)*_b[74];
		const Real _t299 = (0.03125)*_b[80];
		const Real _t300 = (0.03125)*_b[81];
		const Real _t301 = (0.15625)*_b[29] + (0.3125)*_b[50] + (0.3125)*_b[65] + (0.15625)*_b[75] + _t300 + _t52;
		const Real _t302 = (0.15625)*_b[49];
		const Real _t303 = (0.015625)*_b[82];
		const Real _t304 = (0.078125)*_b[77];
		const Real _t305 = (0.015625)*_b[80] + _t304;
		const Real _t306 = _t303 + _t305;
		const Real _t307 = (0.015625)*_b[7];
		const Real _t308 = (0.15625)*_b[54];
		const Real _t309 = (0.15625)*_b[68];
		const Real _t310 = (0.15625)*_b[64] + _t307 + _t308 + _t309;
		const Real _t311 = (0.078125)*_b[28] + (0.078125)*_b[74] + _t144 + _t302 + _t306 + _t310;
		const Real _t312 = (0.09375)*_b[80];
		const Real _t313 = (0.015625)*_b[83];
		const Real _t314 = (0.234375)*_b[49] + (0.234375)*_b[74] + _t243 + _t297 + _t312 + _t313 + _t55;
		const Real _t315 = _t143 + _t149;
		const Real _t316 = (0.078125)*_b[49] + (0.078125)*_b[80] + _t298 + _t303 + _t304 + _t310 + _t313 + _t315;
		const Real _t317 = (0.125)*_b[75];
		const Real _t318 = (0.1875)*_b[65] + _t231;
		const Real _t319 = _t133 + _t135;
		const Real _t320 = _t161 + _t281 + _t285 + _t300 + _t317 + _t318 + _t319;
		const Real _t321 = _t115 + _t121;
		const Real _t322 = (0.1875)*_b[66] + _t236 + _t237 + _t241 + _t274 + _t321;
		const Real _t323 = _t156 + _t78;
		const Real _t324 = _t170 + _t223 + _t323 + _t93;
		const Real _t325 = _t157 + _t80;
		const Real _t326 = _t10 + _t65;
		const Real _t327 = (0.0625)*_b[80];
		const Real _t328 = (0.03125)*_b[82];
		const Real _t329 = (0.03125)*_b[34];
		const Real _t330 = _t159 + _t328 + _t329;
		const Real _t331 = (0.1875)*_b[68];
		const Real _t332 = (0.09375)*_b[74] + _t331;
		const Real _t333 = (0.125)*_b[77];
		const Real _t334 = _t313 + _t333;
		const Real _t335 = _t213 + _t288 + _t295 + _t327 + _t330 + _t332 + _t334;
		const Real _t336 = (0.0625)*_b[78];
		const Real _t337 = (0.0625)*_b[35];
		const Real _t338 = _t336 + _t337;
		const Real _t339 = (0.1875)*_b[69];
		const Real _t340 = _t300 + _t339;
		const Real _t341 = (0.09375)*_b[65] + _t249 + _t251;
		const Real _t342 = (0.09375)*_b[75] + _t201 + _t255 + _t338 + _t340 + _t341;
		const Real _t343 = (0.125)*_b[70];
		const Real _t344 = (0.125)*_b[36];
		const Real _t345 = _t343 + _t344;
		const Real _t346 = (0.25)*_b[56];
		const Real _t347 = _t222 + _t346 + _t91;
		const Real _t348 = _t184 + _t274 + _t345 + _t347;
		const Real _t349 = (0.25)*_b[57];
		const Real _t350 = _t171 + _t223 + _t349 + _t96;
		const Real _t351 = (0.5)*_b[38];
		const Real _t352 = _t157 + _t20 + _t351;
		const Real _t353 = (0.140625)*_b[68] + (0.046875)*_b[74];
		const Real _t354 = (0.046875)*_b[39] + (0.046875)*_b[54];
		const Real _t355 = (0.140625)*_b[71] + (0.046875)*_b[79] + _t354;
		const Real _t356 = (0.140625)*_b[77] + (0.046875)*_b[80] + (0.046875)*_b[82] + _t264 + _t313 + _t353 + _t355;
		const Real _t357 = _t113 + _t272;
		const Real _t358 = (0.1875)*_b[59];
		const Real _t359 = (0.09375)*_b[72] + _t252 + _t358;
		const Real _t360 = (0.09375)*_b[78] + _t198 + _t340 + _t357 + _t359;
		const Real _t361 = (0.1875)*_b[60];
		const Real _t362 = (0.1875)*_b[70] + _t120 + _t238 + _t274 + _t361;
		const Real _t363 = (0.375)*_b[42] + (0.375)*_b[57] + _t223 + _t32;
		const Real _t364 = (0.0625)*_b[82];
		const Real _t365 = (0.125)*_b[61];
		const Real _t366 = _t130 + _t299 + _t365;
		const Real _t367 = (0.1875)*_b[71];
		const Real _t368 = (0.09375)*_b[79] + _t367;
		const Real _t369 = (0.0625)*_b[73];
		const Real _t370 = _t105 + _t228 + _t369;
		const Real _t371 = _t208 + _t290 + _t334 + _t364 + _t366 + _t368 + _t370;
		const Real _t372 = (0.125)*_b[62];
		const Real _t373 = (0.125)*_b[78];
		const Real _t374 = (0.1875)*_b[72] + _t358;
		const Real _t375 = _t139 + _t282;
		const Real _t376 = _t300 + _t372 + _t373 + _t374 + _t375;
		const Real _t377 = (0.25)*_b[45];
		const Real _t378 = (0.375)*_b[60];
		const Real _t379 = (0.25)*_b[70];
		const Real _t380 = _t274 + _t377 + _t378 + _t379 + _t44;
		const Real _t381 = (0.15625)*_b[79];
		const Real _t382 = (0.15625)*_b[61];
		const Real _t383 = (0.15625)*_b[71];
		const Real _t384 = (0.15625)*_b[73] + _t382 + _t383;
		const Real _t385 = (0.078125)*_b[63] + (0.078125)*_b[82] + _t148 + _t305 + _t313 + _t381 + _t384;
		const Real _t386 = (0.15625)*_b[47] + (0.3125)*_b[62] + (0.3125)*_b[72] + (0.15625)*_b[78] + _t300 + _t53;
		const Real _t387 = (0.09375)*_b[48];
		const Real _t388 = (0.3125)*_b[73];
		const Real _t389 = (0.09375)*_b[82];
		const Real _t390 = (0.234375)*_b[63] + (0.234375)*_b[79] + _t313 + _t387 + _t388 + _t389 + _t57;
		const Real _t391 = (0.03125)*_b[83];
		const Real _t392 = (0.375)*_b[65];
		const Real _t393 = (0.25)*_b[75];
		const Real _t394 = (0.0625)*_b[81];
		const Real _t395 = (0.125)*_b[76];
		const Real _t396 = (0.5)*_b[52];
		const Real _t397 = (0.25)*_b[67];
		const Real _t398 = (0.5)*_b[53];
		const Real _t399 = (0.125)*_b[80];
		const Real _t400 = (0.1875)*_b[74] + _t331;
		const Real _t401 = _t333 + _t391;
		const Real _t402 = _t339 + _t394;
		const Real _t403 = _t273 + _t346;
		const Real _t404 = (0.03125)*_b[79];
		const Real _t405 = (0.09375)*_b[71];
		const Real _t406 = _t210 + _t364;
		const Real _t407 = (0.1875)*_b[77];
		const Real _t408 = _t391 + _t407;
		const Real _t409 = _t162 + _t373;
		const Real _t410 = (0.25)*_b[69];
		const Real _t411 = (0.125)*_b[59];
		const Real _t412 = _t317 + _t410 + _t411;
		const Real _t413 = (0.0625)*_b[72];
		const Real _t414 = _t230 + _t413;
		const Real _t415 = (0.125)*_b[60];
		const Real _t416 = (0.25)*_b[42];
		const Real _t417 = (0.5)*_b[57];
		const Real _t418 = (0.09375)*_b[68];
		const Real _t419 = _t211 + _t327;
		const Real _t420 = (0.03125)*_b[73];
		const Real _t421 = _t189 + _t420;
		const Real _t422 = (0.0625)*_b[62];
		const Real _t423 = (0.125)*_b[45];
		const Real _t424 = (0.125)*_b[73];
		const Real _t425 = (0.125)*_b[82];
		const Real _t426 = (0.03125)*_b[63];
		const Real _t427 = (0.1875)*_b[79] + _t367;
		const Real _t428 = (0.0625)*_b[47];
		const Real _t429 = (0.25)*_b[62];
		const Real _t430 = (0.375)*_b[72];
		const Real _t431 = (0.25)*_b[78];
		const Real _t432 = (0.03125)*_b[48];
		const Real _t433 = (0.15625)*_b[63];
		const Real _t434 = (0.375)*_b[74];
		const Real _t435 = (0.25)*_b[80];
		const Real _t436 = (0.0625)*_b[83];
		const Real _t437 = (0.125)*_b[81];
		const Real _t438 = (0.5)*_b[66];
		const Real _t439 = (0.25)*_b[76];
		const Real _t440 = (0.5)*_b[67];
		const Real _t441 = _t407 + _t436;
		const Real _t442 = _t393 + _t410;
		const Real _t443 = (0.0625)*_b[79];
		const Real _t444 = _t277 + _t425;
		const Real _t445 = (0.25)*_b[77];
		const Real _t446 = (0.125)*_b[71];
		const Real _t447 = _t399 + _t445 + _t446;
		const Real _t448 = (0.125)*_b[72];
		const Real _t449 = (0.25)*_b[60];
		const Real _t450 = (0.5)*_b[70];
		const Real _t451 = (0.0625)*_b[63];
		const Real _t452 = (0.25)*_b[73];
		const Real _t453 = (0.375)*_b[79];
		const Real _t454 = (0.25)*_b[82];
		const Real _t455 = (0.125)*_b[83];
		const Real _t456 = (0.5)*_b[75];
		const Real _t457 = (0.25)*_b[81];
		const Real _t458 = (0.5)*_b[76];
		const Real _t459 = _t435 + _t445;
		const Real _t460 = (0.25)*_b[72];
		const Real _t461 = (0.5)*_b[78];
		const Real _t462 = (0.5)*_b[80];
		const Real _t463 = (0.25)*_b[83];
		const Real _t464 = (0.5)*_b[81];
		const Real _t465 = (0.5)*_b[82];
		const Real _t466 = (0.5)*_b[83];
		const Real _t467 = (0.03125)*_b[27];
		const Real _t468 = (0.375)*_b[19];
		const Real _t469 = (0.25)*_b[23];
		const Real _t470 = (0.0625)*_b[26];
		const Real _t471 = (0.125)*_b[24];
		const Real _t472 = (0.5)*_b[16];
		const Real _t473 = (0.25)*_b[21];
		const Real _t474 = (0.5)*_b[17];
		const Real _t475 = (0.375)*_b[22];
		const Real _t476 = (0.25)*_b[25];
		const Real _t477 = (0.0625)*_b[27];
		const Real _t478 = (0.125)*_b[26];
		const Real _t479 = (0.5)*_b[20];
		const Real _t480 = (0.25)*_b[24];
		const Real _t481 = (0.5)*_b[21];
		const Real _t482 = (0.125)*_b[27];
		const Real _t483 = (0.5)*_b[23];
		const Real _t484 = (0.25)*_b[26];
		const Real _t485 = (0.5)*_b[24];
		const Real _t486 = (0.5)*_b[25];
		const Real _t487 = (0.25)*_b[27];
		const Real _t488 = (0.5)*_b[26];
		const Real _t489 = (0.5)*_b[27];
		const Real _t490 = (0.078125)*_b[13] + (0.078125)*_b[25] + _t143 + _t145 + _t146 + _t152 + _t307 + _t50 + _t57;
		const Real _t491 = (0.125)*_b[23];
		const Real _t492 = (0.1875)*_b[19] + _t110;
		const Real _t493 = _t284 + _t319;
		const Real _t494 = _t136 + _t137 + _t491 + _t492 + _t493 + _t53 + _t85;
		const Real _t495 = (0.1875)*_b[20] + _t115 + _t117 + _t118 + _t122 + _t240 + _t44;
		const Real _t496 = _t18 + _t78;
		const Real _t497 = _t168 + _t32 + _t496 + _t93 + _t95;
		const Real _t498 = _t20 + _t80;
		const Real _t499 = (0.125)*_b[25];
		const Real _t500 = (0.1875)*_b[43];
		const Real _t501 = (0.1875)*_b[22] + _t500;
		const Real _t502 = (0.125)*_b[46];
		const Real _t503 = _t467 + _t502;
		const Real _t504 = _t329 + _t432 + _t83;
		const Real _t505 = (0.1875)*_b[44];
		const Real _t506 = _t470 + _t505;
		const Real _t507 = _t337 + _t428;
		const Real _t508 = (0.25)*_b[41];
		const Real _t509 = _t43 + _t508;
		const Real _t510 = _t344 + _t423;
		const Real _t511 = (0.1875)*_b[46];
		const Real _t512 = _t477 + _t511;
		const Real _t513 = (0.0625)*_b[48];
		const Real _t514 = _t292 + _t513;
		const Real _t515 = (0.25)*_b[44];
		const Real _t516 = _t469 + _t515;
		const Real _t517 = (0.125)*_b[47];
		const Real _t518 = _t517 + _t87;
		const Real _t519 = (0.5)*_b[42];
		const Real _t520 = (0.25)*_b[46];
		const Real _t521 = _t476 + _t520;
		const Real _t522 = (0.125)*_b[48];
		const Real _t523 = _t129 + _t522;
		const Real _t524 = (0.25)*_b[47];
		const Real _t525 = (0.5)*_b[45];
		const Real _t526 = (0.25)*_b[48];
		const Real _t527 = (0.5)*_b[47];
		const Real _t528 = (0.5)*_b[48];
		const Real _t529 = (0.0625)*_b[25];
		const Real _t530 = (0.09375)*_b[22] + _t500;
		const Real _t531 = _t502 + _t57;
		const Real _t532 = _t207 + _t215 + _t294 + _t504 + _t529 + _t530 + _t531;
		const Real _t533 = _t505 + _t53;
		const Real _t534 = _t194 + _t254;
		const Real _t535 = (0.09375)*_b[19] + _t196;
		const Real _t536 = (0.09375)*_b[23] + _t202 + _t507 + _t533 + _t534 + _t535;
		const Real _t537 = _t166 + _t30 + _t508;
		const Real _t538 = _t184 + _t44 + _t510 + _t537;
		const Real _t539 = _t169 + _t171 + _t32 + _t416 + _t94;
		const Real _t540 = (0.09375)*_b[61];
		const Real _t541 = _t467 + _t511;
		const Real _t542 = _t411 + _t491 + _t515;
		const Real _t543 = _t107 + _t422;
		const Real _t544 = _t365 + _t499 + _t520;
		const Real _t545 = (0.140625)*_b[61] + (0.046875)*_b[63] + _t261 + _t354;
		const Real _t546 = (0.046875)*_b[22] + (0.140625)*_b[43] + _t262 + _t266;
		const Real _t547 = (0.046875)*_b[25] + (0.140625)*_b[46] + (0.046875)*_b[48] + _t545 + _t546 + _t57;
		const Real _t548 = _t234 + _t41;
		const Real _t549 = (0.09375)*_b[62] + _t197;
		const Real _t550 = (0.09375)*_b[47] + _t253 + _t358 + _t533 + _t548 + _t549;
		const Real _t551 = (0.1875)*_b[45] + _t119 + _t239 + _t361 + _t44;
		const Real _t552 = (0.09375)*_b[43];
		const Real _t553 = _t293 + _t529;
		const Real _t554 = (0.1875)*_b[61];
		const Real _t555 = (0.09375)*_b[63] + _t554;
		const Real _t556 = (0.1875)*_b[62] + _t358;
		const Real _t557 = (0.1875)*_b[63] + _t554;
		const Real _t558 = (0.375)*_b[62];
		const Real _t559 = (0.375)*_b[63];
		const Real _t560 = _t279 + _t446 + _t51;
		const Real _t561 = _t206 + _t291 + _t370 + _t513 + _t531 + _t555 + _t560;
		const Real _t562 = _t282 + _t283;
		const Real _t563 = _t138 + _t448 + _t517 + _t53 + _t556 + _t562;
		const Real _t564 = (0.078125)*_b[48] + (0.078125)*_b[79] + _t147 + _t306 + _t384 + _t433 + _t57;
		const Real _t565 = (0.046875)*_b[73];
		const Real _t566 = (0.109375)*_b[61];
		const Real _t567 = (0.109375)*_b[68];
		const Real _t568 = _t130 + _t208;
		const Real _t569 = (0.15625)*_b[59] + _t199 + _t337;
		const Real _t570 = (0.15625)*_b[55] + _t113 + _t195;
		const Real _t571 = _t180 + _t240 + _t321;
		const Real _t572 = (0.109375)*_b[71];
		const Real _t573 = _t294 + _t306;
		const Real _t574 = (0.109375)*_b[54] + _t260;
		const Real _t575 = _t212 + _t329;
		const Real _t576 = (0.046875)*_b[64] + _t258 + _t263;
		const Real _t577 = _t307 + _t315;
		const Real _t578 = _t288 + _t577;
		const Real _t579 = (0.109375)*_b[43];
		const Real _t580 = _t279 + _t289;
		const Real _t581 = _t288 + _t580;
		const Real _t582 = (0.15625)*_b[40] + _t234 + _t249 + _t250;
		const Real _t583 = _t294 + _t575;
		const Real _t584 = (0.109375)*_b[39] + _t260;
		const Real _t585 = (0.046875)*_b[18] + _t259;
		#pragma endregion
		#pragma region Expressions
		return {{
			{
				_b[0],
				_b[1],
				_b[2],
				_b[3],
				_b[4],
				_b[5],
				_b[6],
				_t0 + _t1,
				_t2 + _t3,
				_t4 + _t5,
				_t6 + _t7,
				_t8 + _t9,
				_t12,
				(0.25)*_b[13] + _t1 + _t13,
				_t14 + _t15 + _t3,
				_t16 + _t17 + _t5,
				_t18 + _t19 + _t6,
				_t22,
				(0.375)*_b[7] + _t23 + _t24 + _t25,
				(0.375)*_b[8] + _t26 + _t27 + _t28,
				(0.375)*_b[9] + _t29 + _t30 + _t31,
				_t34,
				_t24 + _t35 + _t36 + _t37 + _t38,
				_t26 + _t39 + _t40 + _t41 + _t42,
				_t47,
				(0.3125)*_b[13] + (0.15625)*_b[7] + _t48 + _t49 + _t50 + _t51,
				_t54,
				_t59,
				_t0 + _t60,
				_t2 + _t61,
				_t4 + _t62,
				_t63 + _t7,
				_t64 + _t9,
				_t66,
				_t13 + _t67 + _t69,
				_t15 + _t70 + _t72,
				_t17 + _t73 + _t75,
				_t19 + _t76 + _t79,
				_t81,
				(0.125)*_b[13] + _t23 + _t69 + _t84,
				_t28 + _t72 + _t85 + _t88,
				_t31 + _t75 + _t89 + _t92,
				_t98,
				(0.1875)*_b[7] + _t101 + _t103 + _t106 + _t99,
				(0.1875)*_b[8] + _t107 + _t109 + _t111 + _t114,
				_t123,
				_t103 + _t124 + _t125 + _t127 + _t131 + _t25,
				_t141,
				_t153,
				(0.25)*_b[49] + _t13 + _t60,
				_t15 + _t154 + _t61,
				_t155 + _t17 + _t62,
				_t156 + _t19 + _t63,
				_t158,
				(0.125)*_b[49] + _t160 + _t23 + _t67,
				_t161 + _t163 + _t28 + _t70,
				_t164 + _t167 + _t31 + _t73,
				_t173,
				_t160 + _t174 + _t175 + _t176 + _t35 + _t84,
				(0.0625)*_b[59] + _t163 + _t179 + _t40 + _t88,
				_t185,
				(0.03125)*_b[61] + _t106 + _t186 + _t187 + _t190 + _t191 + _t192 + _t58,
				_t204,
				_t216,
				(0.375)*_b[28] + _t217 + _t218 + _t23,
				(0.375)*_b[29] + _t219 + _t220 + _t28,
				(0.375)*_b[30] + _t221 + _t222 + _t31,
				_t224,
				(0.1875)*_b[28] + _t101 + _t225 + _t227 + _t229,
				(0.1875)*_b[29] + _t109 + _t230 + _t232 + _t235,
				_t242,
				(0.03125)*_b[71] + _t191 + _t229 + _t243 + _t244 + _t245 + _t247 + _t248,
				_t257,
				_t268,
				_t217 + _t269 + _t270 + _t35 + _t67,
				_t219 + _t271 + _t272 + _t40 + _t70,
				_t275,
				_t127 + _t218 + _t227 + _t276 + _t280 + _t82,
				_t287,
				_t296,
				(0.15625)*_b[28] + (0.3125)*_b[49] + _t297 + _t298 + _t299 + _t48,
				_t301,
				_t311,
				_t314,
			},
			{
				_t314,
				_t301,
				_t275,
				_t224,
				_t158,
				_t66,
				_b[6],
				_t316,
				_t320,
				_t322,
				_t324,
				_t325,
				_t326,
				_t335,
				_t342,
				_t348,
				_t350,
				_t352,
				_t356,
				_t360,
				_t362,
				_t363,
				_t371,
				_t376,
				_t380,
				_t385,
				_t386,
				_t390,
				(0.3125)*_b[74] + (0.15625)*_b[80] + _t128 + _t297 + _t302 + _t391,
				_t112 + _t154 + _t392 + _t393 + _t394,
				(0.375)*_b[66] + _t221 + _t395 + _t90,
				_t396 + _t397 + _t76,
				_t398 + _t64,
				_b[33],
				_t186 + _t218 + _t330 + _t399 + _t400 + _t401,
				(0.1875)*_b[75] + _t177 + _t318 + _t338 + _t402,
				_t164 + _t345 + _t395 + _t403,
				_t323 + _t349 + _t397,
				_t351 + _t398,
				(0.03125)*_b[39] + _t247 + _t312 + _t332 + _t404 + _t405 + _t406 + _t408,
				(0.0625)*_b[40] + _t394 + _t409 + _t412 + _t414,
				_t347 + _t379 + _t395 + _t415,
				_t397 + _t416 + _t417,
				(0.03125)*_b[43] + _t276 + _t368 + _t389 + _t408 + _t418 + _t419 + _t421,
				(0.1875)*_b[78] + _t357 + _t374 + _t402 + _t422,
				(0.375)*_b[70] + _t378 + _t395 + _t423,
				_t366 + _t401 + _t424 + _t425 + _t426 + _t427,
				_t394 + _t428 + _t429 + _t430 + _t431,
				(0.3125)*_b[79] + (0.15625)*_b[82] + _t388 + _t391 + _t432 + _t433,
				_t174 + _t269 + _t434 + _t435 + _t436,
				(0.375)*_b[75] + _t161 + _t392 + _t437,
				_t155 + _t438 + _t439,
				_t396 + _t440,
				_b[53],
				(0.1875)*_b[80] + _t225 + _t400 + _t406 + _t441,
				_t220 + _t409 + _t437 + _t442,
				_t379 + _t403 + _t439,
				_t417 + _t440,
				_t176 + _t270 + _t436 + _t443 + _t444 + _t447,
				_t412 + _t431 + _t437 + _t448,
				_t439 + _t449 + _t450,
				(0.1875)*_b[82] + _t369 + _t419 + _t427 + _t441,
				(0.375)*_b[78] + _t372 + _t430 + _t437,
				_t436 + _t451 + _t452 + _t453 + _t454,
				(0.375)*_b[80] + _t218 + _t434 + _t455,
				_t271 + _t456 + _t457,
				_t438 + _t458,
				_b[67],
				(0.125)*_b[74] + _t444 + _t455 + _t459,
				_t431 + _t442 + _t457,
				_t450 + _t458,
				(0.125)*_b[79] + _t447 + _t454 + _t455,
				_t457 + _t460 + _t461,
				(0.375)*_b[82] + _t424 + _t453 + _t455,
				(0.25)*_b[74] + _t462 + _t463,
				_t456 + _t464,
				_b[76],
				_t454 + _t459 + _t463,
				_t461 + _t464,
				(0.25)*_b[79] + _t463 + _t465,
				_t462 + _t466,
				_b[81],
				_t465 + _t466,
				_b[83],
			},
			{
				_t59,
				_t54,
				_t47,
				_t34,
				_t22,
				_t12,
				_b[6],
				(0.3125)*_b[22] + (0.15625)*_b[25] + _t142 + _t278 + _t467 + _t49,
				_t14 + _t233 + _t468 + _t469 + _t470,
				(0.375)*_b[20] + _t165 + _t29 + _t471,
				_t472 + _t473 + _t77,
				_t474 + _t8,
				_b[12],
				_t175 + _t36 + _t475 + _t476 + _t477,
				(0.375)*_b[23] + _t468 + _t478 + _t85,
				_t16 + _t479 + _t480,
				_t472 + _t481,
				_b[17],
				(0.375)*_b[25] + _t25 + _t475 + _t482,
				_t39 + _t483 + _t484,
				_t479 + _t485,
				_b[21],
				(0.25)*_b[22] + _t486 + _t487,
				_t483 + _t488,
				_b[24],
				_t486 + _t489,
				_b[26],
				_b[27],
				_t490,
				_t494,
				_t495,
				_t497,
				_t498,
				_t326,
				_t244 + _t25 + _t499 + _t501 + _t503 + _t504,
				(0.1875)*_b[23] + _t178 + _t492 + _t506 + _t507,
				_t471 + _t509 + _t510 + _t89,
				_t416 + _t473 + _t496,
				_t351 + _t474,
				(0.1875)*_b[25] + _t501 + _t512 + _t514 + _t99,
				_t27 + _t478 + _t516 + _t518,
				_t377 + _t480 + _t509,
				_t481 + _t519,
				(0.125)*_b[22] + _t482 + _t521 + _t523,
				_t484 + _t516 + _t524,
				_t485 + _t525,
				_t487 + _t521 + _t526,
				_t488 + _t527,
				_t489 + _t528,
				_t532,
				_t536,
				_t538,
				_t539,
				_t352,
				(0.03125)*_b[54] + _t190 + _t426 + _t514 + _t530 + _t540 + _t541 + _t56,
				(0.0625)*_b[55] + _t470 + _t518 + _t542 + _t543,
				_t377 + _t415 + _t471 + _t537,
				_t349 + _t473 + _t519,
				_t176 + _t37 + _t451 + _t477 + _t523 + _t544,
				_t372 + _t478 + _t524 + _t542,
				_t449 + _t480 + _t525,
				(0.125)*_b[63] + _t482 + _t526 + _t544,
				_t429 + _t484 + _t527,
				(0.25)*_b[63] + _t487 + _t528,
				_t547,
				_t550,
				_t551,
				_t363,
				(0.03125)*_b[68] + _t125 + _t387 + _t421 + _t541 + _t552 + _t553 + _t555,
				(0.1875)*_b[47] + _t413 + _t506 + _t548 + _t556,
				(0.375)*_b[45] + _t343 + _t378 + _t471,
				(0.1875)*_b[48] + _t369 + _t512 + _t553 + _t557,
				(0.375)*_b[47] + _t448 + _t478 + _t558,
				(0.375)*_b[48] + _t424 + _t482 + _t559,
				_t561,
				_t563,
				_t380,
				_t404 + _t424 + _t503 + _t522 + _t557 + _t560,
				_t336 + _t460 + _t470 + _t524 + _t558,
				_t443 + _t452 + _t477 + _t526 + _t559,
				_t564,
				_t386,
				(0.15625)*_b[48] + (0.3125)*_b[63] + _t328 + _t381 + _t388 + _t467,
				_t390,
			},
			{
				_t390,
				_t386,
				_t380,
				_t363,
				_t352,
				_t326,
				_b[6],
				_t385,
				_t376,
				_t362,
				_t350,
				_t325,
				_t66,
				_t371,
				_t360,
				_t348,
				_t324,
				_t158,
				_t356,
				_t342,
				_t322,
				_t224,
				_t335,
				_t320,
				_t275,
				_t316,
				_t301,
				_t314,
				_t564,
				_t563,
				_t551,
				_t539,
				_t498,
				_t12,
				(0.09375)*_b[73] + _t105 + _t148 + _t206 + _t228 + _t290 + _t306 + _t382 + _t383 + _t443 + _t451,
				_t283 + _t359 + _t375 + _t549,
				_t120 + _t183 + _t239 + _t344 + _t415,
				_t172 + _t97,
				_t81,
				(0.078125)*_b[43] + _t261 + _t276 + _t306 + _t355 + _t565 + _t566 + _t567 + _t568,
				_t414 + _t534 + _t562 + _t569 + _t570 + _t87,
				_t164 + _t181 + _t239 + _t571 + _t91,
				_t173,
				(0.078125)*_b[39] + _t353 + _t404 + _t572 + _t573 + _t574 + _t575 + _t576,
				_t256 + _t286 + _t319 + _t341,
				_t242,
				(0.09375)*_b[64] + _t174 + _t189 + _t270 + _t292 + _t293 + _t308 + _t309 + _t573 + _t578,
				_t287,
				_t311,
				_t561,
				_t550,
				_t538,
				_t497,
				_t22,
				(0.078125)*_b[68] + _t125 + _t148 + _t545 + _t565 + _t572 + _t579 + _t581,
				_t139 + _t162 + _t254 + _t543 + _t569 + _t582,
				_t120 + _t166 + _t182 + _t571 + _t89,
				_t98,
				(0.15625)*_b[58] + _t187 + _t188 + _t245 + _t246 + _t405 + _t418 + _t420 + _t540 + _t552 + _t568 + _t581 + _t583,
				_t179 + _t194 + _t411 + _t493 + _t570 + _t582,
				_t185,
				(0.078125)*_b[71] + _t244 + _t265 + _t567 + _t576 + _t578 + _t580 + _t584,
				_t257,
				_t296,
				_t547,
				_t536,
				_t495,
				_t34,
				(0.078125)*_b[54] + _t148 + _t426 + _t546 + _t566 + _t583 + _t584 + _t585,
				_t133 + _t140 + _t194 + _t203 + _t284 + _t535,
				_t123,
				(0.078125)*_b[61] + _t186 + _t262 + _t267 + _t568 + _t574 + _t577 + _t579 + _t585,
				_t204,
				_t268,
				_t532,
				_t494,
				_t47,
				(0.09375)*_b[18] + _t148 + _t150 + _t151 + _t175 + _t207 + _t214 + _t37 + _t577,
				_t141,
				_t216,
				_t490,
				_t54,
				_t153,
				_t59,
			},
		}};
		#pragma endregion
	}
}
