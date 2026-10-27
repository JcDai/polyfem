#include "../P3TetDegVal.hpp"
namespace miso {
	std::array<RealVector<84>, 4> P3TetDegVal::subdiv_3_3p6(const RealVector<84> &_b) {
		#pragma region Temporaries
		const Real _t0 = (0.5)*_b[0];
		const Real _t1 = (0.5)*_b[1];
		const Real _t2 = (0.25)*_b[0];
		const Real _t3 = (0.125)*_b[0];
		const Real _t4 = (0.375)*_b[2];
		const Real _t5 = (0.125)*_b[3];
		const Real _t6 = (0.0625)*_b[0];
		const Real _t7 = (0.25)*_b[1];
		const Real _t8 = (0.25)*_b[3];
		const Real _t9 = (0.0625)*_b[4];
		const Real _t10 = (0.03125)*_b[0];
		const Real _t11 = (0.3125)*_b[3];
		const Real _t12 = (0.15625)*_b[4];
		const Real _t13 = (0.03125)*_b[5];
		const Real _t14 = (0.015625)*_b[0];
		const Real _t15 = (0.09375)*_b[1];
		const Real _t16 = (0.09375)*_b[5];
		const Real _t17 = (0.015625)*_b[6];
		const Real _t18 = (0.234375)*_b[2] + (0.234375)*_b[4] + _t11 + _t14 + _t15 + _t16 + _t17;
		const Real _t19 = (0.5)*_b[7];
		const Real _t20 = (0.5)*_b[8];
		const Real _t21 = (0.25)*_b[7];
		const Real _t22 = (0.25)*_b[9];
		const Real _t23 = (0.125)*_b[10];
		const Real _t24 = (0.125)*_b[7];
		const Real _t25 = (0.375)*_b[9];
		const Real _t26 = (0.25)*_b[10];
		const Real _t27 = (0.0625)*_b[11];
		const Real _t28 = (0.0625)*_b[7];
		const Real _t29 = (0.25)*_b[8];
		const Real _t30 = (0.03125)*_b[12];
		const Real _t31 = (0.03125)*_b[7];
		const Real _t32 = (0.3125)*_b[10] + (0.15625)*_b[11] + (0.15625)*_b[8] + (0.3125)*_b[9] + _t30 + _t31;
		const Real _t33 = (0.5)*_b[13];
		const Real _t34 = (0.5)*_b[14];
		const Real _t35 = (0.25)*_b[13];
		const Real _t36 = (0.25)*_b[15];
		const Real _t37 = (0.125)*_b[13];
		const Real _t38 = (0.375)*_b[15];
		const Real _t39 = (0.125)*_b[16];
		const Real _t40 = (0.0625)*_b[13];
		const Real _t41 = (0.25)*_b[14];
		const Real _t42 = (0.25)*_b[16];
		const Real _t43 = (0.0625)*_b[17];
		const Real _t44 = _t38 + _t40 + _t41 + _t42 + _t43;
		const Real _t45 = (0.5)*_b[18];
		const Real _t46 = (0.5)*_b[19];
		const Real _t47 = (0.25)*_b[18];
		const Real _t48 = (0.25)*_b[20];
		const Real _t49 = (0.125)*_b[18];
		const Real _t50 = (0.125)*_b[21];
		const Real _t51 = (0.375)*_b[19] + (0.375)*_b[20] + _t49 + _t50;
		const Real _t52 = (0.5)*_b[22];
		const Real _t53 = (0.5)*_b[23];
		const Real _t54 = (0.25)*_b[22];
		const Real _t55 = (0.25)*_b[24];
		const Real _t56 = _t53 + _t54 + _t55;
		const Real _t57 = (0.5)*_b[25];
		const Real _t58 = (0.5)*_b[26];
		const Real _t59 = _t57 + _t58;
		const Real _t60 = (0.5)*_b[28];
		const Real _t61 = (0.25)*_b[28];
		const Real _t62 = (0.25)*_b[29];
		const Real _t63 = _t62 + _t7;
		const Real _t64 = (0.125)*_b[28];
		const Real _t65 = (0.125)*_b[30];
		const Real _t66 = _t64 + _t65;
		const Real _t67 = (0.0625)*_b[3];
		const Real _t68 = (0.1875)*_b[29];
		const Real _t69 = _t6 + _t68;
		const Real _t70 = (0.1875)*_b[30];
		const Real _t71 = (0.1875)*_b[2] + _t70;
		const Real _t72 = (0.0625)*_b[28];
		const Real _t73 = (0.0625)*_b[31];
		const Real _t74 = _t72 + _t73;
		const Real _t75 = (0.125)*_b[1];
		const Real _t76 = (0.03125)*_b[4];
		const Real _t77 = (0.125)*_b[29];
		const Real _t78 = _t10 + _t77;
		const Real _t79 = (0.03125)*_b[28];
		const Real _t80 = (0.125)*_b[31];
		const Real _t81 = (0.03125)*_b[32];
		const Real _t82 = _t79 + _t80 + _t81;
		const Real _t83 = (0.15625)*_b[2];
		const Real _t84 = (0.078125)*_b[29];
		const Real _t85 = _t14 + _t84;
		const Real _t86 = (0.015625)*_b[33];
		const Real _t87 = (0.078125)*_b[32];
		const Real _t88 = (0.015625)*_b[5] + _t87;
		const Real _t89 = _t86 + _t88;
		const Real _t90 = (0.015625)*_b[28];
		const Real _t91 = (0.15625)*_b[30];
		const Real _t92 = (0.15625)*_b[31];
		const Real _t93 = (0.15625)*_b[3] + _t90 + _t91 + _t92;
		const Real _t94 = (0.078125)*_b[1] + (0.078125)*_b[4] + _t83 + _t85 + _t89 + _t93;
		const Real _t95 = (0.5)*_b[34];
		const Real _t96 = (0.25)*_b[34];
		const Real _t97 = (0.25)*_b[35];
		const Real _t98 = _t29 + _t97;
		const Real _t99 = (0.125)*_b[9];
		const Real _t100 = (0.125)*_b[34];
		const Real _t101 = (0.125)*_b[36];
		const Real _t102 = _t100 + _t101;
		const Real _t103 = (0.0625)*_b[10];
		const Real _t104 = (0.1875)*_b[35];
		const Real _t105 = _t104 + _t28;
		const Real _t106 = (0.1875)*_b[36];
		const Real _t107 = (0.1875)*_b[9] + _t106;
		const Real _t108 = (0.0625)*_b[34];
		const Real _t109 = (0.0625)*_b[37];
		const Real _t110 = _t108 + _t109;
		const Real _t111 = (0.125)*_b[8];
		const Real _t112 = (0.125)*_b[35];
		const Real _t113 = _t112 + _t31;
		const Real _t114 = (0.03125)*_b[34];
		const Real _t115 = (0.03125)*_b[38];
		const Real _t116 = (0.125)*_b[37];
		const Real _t117 = (0.03125)*_b[11] + _t116;
		const Real _t118 = _t115 + _t117;
		const Real _t119 = _t114 + _t118;
		const Real _t120 = _t107 + _t111 + _t113 + _t119 + _t23;
		const Real _t121 = (0.5)*_b[39];
		const Real _t122 = (0.25)*_b[39];
		const Real _t123 = (0.25)*_b[40];
		const Real _t124 = _t123 + _t41;
		const Real _t125 = (0.125)*_b[15];
		const Real _t126 = (0.125)*_b[39];
		const Real _t127 = (0.125)*_b[41];
		const Real _t128 = _t126 + _t127;
		const Real _t129 = (0.1875)*_b[40];
		const Real _t130 = _t129 + _t40;
		const Real _t131 = (0.0625)*_b[42];
		const Real _t132 = (0.1875)*_b[41];
		const Real _t133 = (0.0625)*_b[16] + _t132;
		const Real _t134 = _t131 + _t133;
		const Real _t135 = (0.0625)*_b[39];
		const Real _t136 = (0.1875)*_b[15] + _t135;
		const Real _t137 = (0.1875)*_b[14] + _t130 + _t134 + _t136;
		const Real _t138 = (0.5)*_b[43];
		const Real _t139 = (0.25)*_b[43];
		const Real _t140 = (0.25)*_b[19];
		const Real _t141 = (0.25)*_b[44];
		const Real _t142 = _t140 + _t141;
		const Real _t143 = (0.125)*_b[43];
		const Real _t144 = (0.125)*_b[20];
		const Real _t145 = (0.125)*_b[45];
		const Real _t146 = _t144 + _t145;
		const Real _t147 = _t143 + _t146;
		const Real _t148 = _t142 + _t147 + _t49;
		const Real _t149 = (0.5)*_b[46];
		const Real _t150 = (0.25)*_b[23] + (0.25)*_b[46] + (0.25)*_b[47];
		const Real _t151 = _t150 + _t54;
		const Real _t152 = (0.5)*_b[48];
		const Real _t153 = _t152 + _t57;
		const Real _t154 = (0.125)*_b[50];
		const Real _t155 = _t154 + _t62 + _t75;
		const Real _t156 = (0.0625)*_b[49];
		const Real _t157 = (0.0625)*_b[2];
		const Real _t158 = (0.0625)*_b[51];
		const Real _t159 = (0.03125)*_b[49];
		const Real _t160 = (0.09375)*_b[50];
		const Real _t161 = (0.03125)*_b[3];
		const Real _t162 = (0.09375)*_b[51];
		const Real _t163 = _t161 + _t162;
		const Real _t164 = _t10 + _t68;
		const Real _t165 = (0.09375)*_b[2] + _t70;
		const Real _t166 = (0.0625)*_b[1];
		const Real _t167 = (0.015625)*_b[4];
		const Real _t168 = (0.015625)*_b[53];
		const Real _t169 = _t167 + _t168;
		const Real _t170 = _t14 + _t77;
		const Real _t171 = (0.0625)*_b[50];
		const Real _t172 = (0.0625)*_b[52];
		const Real _t173 = (0.015625)*_b[49];
		const Real _t174 = _t162 + _t173;
		const Real _t175 = _t171 + _t172 + _t174;
		const Real _t176 = _t175 + _t67;
		const Real _t177 = _t165 + _t166 + _t169 + _t170 + _t176 + _t82;
		const Real _t178 = (0.25)*_b[54];
		const Real _t179 = (0.125)*_b[54];
		const Real _t180 = (0.125)*_b[55];
		const Real _t181 = _t111 + _t180 + _t97;
		const Real _t182 = (0.0625)*_b[54];
		const Real _t183 = (0.0625)*_b[9];
		const Real _t184 = _t182 + _t183;
		const Real _t185 = _t104 + _t31;
		const Real _t186 = (0.03125)*_b[57];
		const Real _t187 = (0.03125)*_b[10];
		const Real _t188 = (0.09375)*_b[55];
		const Real _t189 = _t187 + _t188;
		const Real _t190 = _t186 + _t189;
		const Real _t191 = (0.03125)*_b[54];
		const Real _t192 = (0.09375)*_b[56];
		const Real _t193 = _t191 + _t192;
		const Real _t194 = _t106 + _t193;
		const Real _t195 = (0.09375)*_b[9] + _t194;
		const Real _t196 = (0.09375)*_b[8] + _t110 + _t185 + _t190 + _t195;
		const Real _t197 = (0.25)*_b[58];
		const Real _t198 = (0.125)*_b[58];
		const Real _t199 = (0.125)*_b[14];
		const Real _t200 = (0.125)*_b[59];
		const Real _t201 = _t123 + _t199 + _t200;
		const Real _t202 = (0.0625)*_b[60];
		const Real _t203 = (0.0625)*_b[15];
		const Real _t204 = (0.0625)*_b[58];
		const Real _t205 = _t203 + _t204;
		const Real _t206 = _t202 + _t205;
		const Real _t207 = _t128 + _t201 + _t206 + _t40;
		const Real _t208 = (0.25)*_b[61];
		const Real _t209 = (0.125)*_b[19];
		const Real _t210 = (0.125)*_b[62];
		const Real _t211 = _t209 + _t210;
		const Real _t212 = (0.125)*_b[61] + _t141;
		const Real _t213 = _t211 + _t212;
		const Real _t214 = _t139 + _t213 + _t49;
		const Real _t215 = (0.25)*_b[63];
		const Real _t216 = _t149 + _t215 + _t54;
		const Real _t217 = (0.375)*_b[49];
		const Real _t218 = (0.125)*_b[64];
		const Real _t219 = (0.0625)*_b[64];
		const Real _t220 = (0.1875)*_b[50];
		const Real _t221 = (0.1875)*_b[49] + _t220;
		const Real _t222 = (0.0625)*_b[65];
		const Real _t223 = _t166 + _t222;
		const Real _t224 = (0.09375)*_b[28];
		const Real _t225 = (0.03125)*_b[2];
		const Real _t226 = (0.09375)*_b[30];
		const Real _t227 = (0.03125)*_b[64];
		const Real _t228 = _t162 + _t227;
		const Real _t229 = (0.09375)*_b[49] + _t220;
		const Real _t230 = (0.015625)*_b[3];
		const Real _t231 = (0.015625)*_b[64];
		const Real _t232 = (0.140625)*_b[51];
		const Real _t233 = _t230 + _t231 + _t232;
		const Real _t234 = (0.015625)*_b[67];
		const Real _t235 = (0.046875)*_b[31] + (0.046875)*_b[52] + _t234;
		const Real _t236 = _t233 + _t235;
		const Real _t237 = (0.046875)*_b[49] + (0.140625)*_b[50];
		const Real _t238 = (0.046875)*_b[65] + (0.046875)*_b[66];
		const Real _t239 = (0.046875)*_b[2] + (0.140625)*_b[30] + _t238;
		const Real _t240 = (0.046875)*_b[1] + (0.046875)*_b[28] + (0.140625)*_b[29] + _t14 + _t236 + _t237 + _t239;
		const Real _t241 = (0.375)*_b[54];
		const Real _t242 = (0.125)*_b[68];
		const Real _t243 = (0.0625)*_b[68];
		const Real _t244 = (0.1875)*_b[55];
		const Real _t245 = (0.1875)*_b[54] + _t244;
		const Real _t246 = (0.0625)*_b[8];
		const Real _t247 = (0.0625)*_b[69];
		const Real _t248 = _t246 + _t247;
		const Real _t249 = (0.03125)*_b[70];
		const Real _t250 = (0.03125)*_b[68];
		const Real _t251 = (0.09375)*_b[36];
		const Real _t252 = _t250 + _t251;
		const Real _t253 = _t249 + _t252;
		const Real _t254 = (0.03125)*_b[9];
		const Real _t255 = _t244 + _t254;
		const Real _t256 = (0.09375)*_b[54] + _t192 + _t255;
		const Real _t257 = (0.09375)*_b[34] + _t185 + _t248 + _t253 + _t256;
		const Real _t258 = (0.375)*_b[58];
		const Real _t259 = (0.125)*_b[71];
		const Real _t260 = (0.0625)*_b[72];
		const Real _t261 = (0.1875)*_b[59];
		const Real _t262 = (0.0625)*_b[71] + _t261;
		const Real _t263 = _t260 + _t262;
		const Real _t264 = (0.0625)*_b[14];
		const Real _t265 = (0.1875)*_b[58] + _t264;
		const Real _t266 = (0.1875)*_b[39] + _t130 + _t263 + _t265;
		const Real _t267 = (0.125)*_b[73];
		const Real _t268 = (0.375)*_b[43] + (0.375)*_b[61] + _t267 + _t49;
		const Real _t269 = (0.25)*_b[64];
		const Real _t270 = (0.0625)*_b[74];
		const Real _t271 = (0.03125)*_b[74];
		const Real _t272 = (0.125)*_b[65];
		const Real _t273 = (0.03125)*_b[1];
		const Real _t274 = (0.03125)*_b[75];
		const Real _t275 = _t272 + _t273 + _t274;
		const Real _t276 = (0.015625)*_b[76];
		const Real _t277 = (0.015625)*_b[74];
		const Real _t278 = _t162 + _t277;
		const Real _t279 = _t276 + _t278;
		const Real _t280 = (0.0625)*_b[30];
		const Real _t281 = (0.0625)*_b[66];
		const Real _t282 = (0.015625)*_b[2];
		const Real _t283 = _t219 + _t280 + _t281 + _t282;
		const Real _t284 = _t170 + _t229 + _t275 + _t279 + _t283 + _t72;
		const Real _t285 = (0.25)*_b[68];
		const Real _t286 = (0.0625)*_b[77];
		const Real _t287 = (0.125)*_b[69];
		const Real _t288 = (0.03125)*_b[77] + _t287;
		const Real _t289 = (0.03125)*_b[78];
		const Real _t290 = (0.03125)*_b[8];
		const Real _t291 = _t289 + _t290;
		const Real _t292 = _t288 + _t291;
		const Real _t293 = _t100 + _t113 + _t242 + _t245 + _t292;
		const Real _t294 = (0.25)*_b[71];
		const Real _t295 = (0.0625)*_b[79];
		const Real _t296 = _t122 + _t258 + _t294 + _t295 + _t40;
		const Real _t297 = (0.3125)*_b[64];
		const Real _t298 = (0.15625)*_b[74];
		const Real _t299 = (0.03125)*_b[80];
		const Real _t300 = (0.15625)*_b[49];
		const Real _t301 = (0.015625)*_b[81];
		const Real _t302 = (0.078125)*_b[75];
		const Real _t303 = (0.015625)*_b[80] + _t302;
		const Real _t304 = _t301 + _t303;
		const Real _t305 = (0.015625)*_b[1];
		const Real _t306 = (0.15625)*_b[50];
		const Real _t307 = (0.15625)*_b[65];
		const Real _t308 = (0.15625)*_b[64] + _t305 + _t306 + _t307;
		const Real _t309 = (0.078125)*_b[28] + (0.078125)*_b[74] + _t300 + _t304 + _t308 + _t85;
		const Real _t310 = (0.03125)*_b[82];
		const Real _t311 = (0.15625)*_b[34] + (0.3125)*_b[54] + (0.3125)*_b[68] + (0.15625)*_b[77] + _t31 + _t310;
		const Real _t312 = (0.09375)*_b[80];
		const Real _t313 = (0.015625)*_b[83];
		const Real _t314 = (0.234375)*_b[49] + (0.234375)*_b[74] + _t14 + _t224 + _t297 + _t312 + _t313;
		const Real _t315 = _t84 + _t90;
		const Real _t316 = (0.078125)*_b[49] + (0.078125)*_b[80] + _t298 + _t301 + _t302 + _t308 + _t313 + _t315;
		const Real _t317 = (0.0625)*_b[80];
		const Real _t318 = (0.03125)*_b[81];
		const Real _t319 = (0.03125)*_b[29];
		const Real _t320 = _t154 + _t318 + _t319;
		const Real _t321 = (0.1875)*_b[65];
		const Real _t322 = (0.09375)*_b[74] + _t321;
		const Real _t323 = (0.125)*_b[75];
		const Real _t324 = _t313 + _t323;
		const Real _t325 = _t174 + _t276 + _t283 + _t317 + _t320 + _t322 + _t324;
		const Real _t326 = (0.140625)*_b[65] + (0.046875)*_b[74];
		const Real _t327 = (0.046875)*_b[30] + (0.046875)*_b[50];
		const Real _t328 = (0.140625)*_b[66] + (0.046875)*_b[76] + _t327;
		const Real _t329 = (0.140625)*_b[75] + (0.046875)*_b[80] + (0.046875)*_b[81] + _t236 + _t313 + _t326 + _t328;
		const Real _t330 = (0.0625)*_b[81];
		const Real _t331 = (0.125)*_b[52];
		const Real _t332 = _t299 + _t331 + _t81;
		const Real _t333 = (0.1875)*_b[66];
		const Real _t334 = (0.09375)*_b[76] + _t333;
		const Real _t335 = (0.0625)*_b[67];
		const Real _t336 = _t222 + _t335 + _t73;
		const Real _t337 = _t169 + _t278 + _t324 + _t330 + _t332 + _t334 + _t336;
		const Real _t338 = (0.15625)*_b[76];
		const Real _t339 = (0.15625)*_b[52];
		const Real _t340 = (0.15625)*_b[66];
		const Real _t341 = (0.15625)*_b[67] + _t339 + _t340;
		const Real _t342 = (0.078125)*_b[53] + (0.078125)*_b[81] + _t303 + _t313 + _t338 + _t341 + _t89;
		const Real _t343 = (0.09375)*_b[33];
		const Real _t344 = (0.3125)*_b[67];
		const Real _t345 = (0.09375)*_b[81];
		const Real _t346 = (0.234375)*_b[53] + (0.234375)*_b[76] + _t17 + _t313 + _t343 + _t344 + _t345;
		const Real _t347 = (0.125)*_b[77];
		const Real _t348 = (0.1875)*_b[68] + _t244;
		const Real _t349 = _t112 + _t114;
		const Real _t350 = _t179 + _t287 + _t291 + _t310 + _t347 + _t348 + _t349;
		const Real _t351 = (0.0625)*_b[78];
		const Real _t352 = (0.0625)*_b[35];
		const Real _t353 = _t351 + _t352;
		const Real _t354 = (0.1875)*_b[69];
		const Real _t355 = _t310 + _t354;
		const Real _t356 = (0.09375)*_b[68] + _t249 + _t251;
		const Real _t357 = (0.09375)*_b[77] + _t193 + _t255 + _t353 + _t355 + _t356;
		const Real _t358 = _t109 + _t286;
		const Real _t359 = (0.1875)*_b[56];
		const Real _t360 = (0.09375)*_b[70] + _t252 + _t359;
		const Real _t361 = (0.09375)*_b[78] + _t190 + _t355 + _t358 + _t360;
		const Real _t362 = (0.125)*_b[57];
		const Real _t363 = (0.125)*_b[78];
		const Real _t364 = (0.1875)*_b[70] + _t359;
		const Real _t365 = _t118 + _t288;
		const Real _t366 = _t310 + _t362 + _t363 + _t364 + _t365;
		const Real _t367 = (0.15625)*_b[38] + (0.3125)*_b[57] + (0.3125)*_b[70] + (0.15625)*_b[78] + _t30 + _t310;
		const Real _t368 = _t129 + _t135;
		const Real _t369 = (0.1875)*_b[71] + _t260 + _t261 + _t265 + _t295 + _t368;
		const Real _t370 = (0.125)*_b[72];
		const Real _t371 = (0.125)*_b[40];
		const Real _t372 = _t370 + _t371;
		const Real _t373 = (0.25)*_b[59];
		const Real _t374 = _t127 + _t259 + _t373;
		const Real _t375 = _t206 + _t295 + _t372 + _t374;
		const Real _t376 = (0.1875)*_b[60];
		const Real _t377 = (0.1875)*_b[72] + _t134 + _t262 + _t295 + _t376;
		const Real _t378 = (0.25)*_b[42];
		const Real _t379 = (0.375)*_b[60];
		const Real _t380 = (0.25)*_b[72];
		const Real _t381 = _t295 + _t378 + _t379 + _t380 + _t43;
		const Real _t382 = _t141 + _t208;
		const Real _t383 = _t143 + _t211 + _t267 + _t382;
		const Real _t384 = (0.25)*_b[62];
		const Real _t385 = _t146 + _t212 + _t267 + _t384;
		const Real _t386 = (0.375)*_b[45] + (0.375)*_b[62] + _t267 + _t50;
		const Real _t387 = _t150 + _t215;
		const Real _t388 = (0.5)*_b[47];
		const Real _t389 = _t215 + _t388 + _t55;
		const Real _t390 = _t152 + _t58;
		const Real _t391 = (0.03125)*_b[83];
		const Real _t392 = (0.125)*_b[80];
		const Real _t393 = (0.1875)*_b[74] + _t321;
		const Real _t394 = _t323 + _t391;
		const Real _t395 = (0.03125)*_b[76];
		const Real _t396 = (0.09375)*_b[66];
		const Real _t397 = _t171 + _t330;
		const Real _t398 = (0.1875)*_b[75];
		const Real _t399 = _t391 + _t398;
		const Real _t400 = (0.09375)*_b[65];
		const Real _t401 = _t172 + _t317;
		const Real _t402 = (0.03125)*_b[67];
		const Real _t403 = _t162 + _t402;
		const Real _t404 = (0.125)*_b[67];
		const Real _t405 = (0.125)*_b[81];
		const Real _t406 = (0.03125)*_b[53];
		const Real _t407 = (0.1875)*_b[76] + _t333;
		const Real _t408 = (0.03125)*_b[33];
		const Real _t409 = (0.15625)*_b[53];
		const Real _t410 = (0.375)*_b[68];
		const Real _t411 = (0.25)*_b[77];
		const Real _t412 = (0.0625)*_b[82];
		const Real _t413 = _t354 + _t412;
		const Real _t414 = _t180 + _t363;
		const Real _t415 = (0.25)*_b[69];
		const Real _t416 = (0.125)*_b[56];
		const Real _t417 = _t347 + _t415 + _t416;
		const Real _t418 = (0.0625)*_b[70];
		const Real _t419 = _t243 + _t418;
		const Real _t420 = (0.0625)*_b[57];
		const Real _t421 = (0.0625)*_b[38];
		const Real _t422 = (0.25)*_b[57];
		const Real _t423 = (0.375)*_b[70];
		const Real _t424 = (0.25)*_b[78];
		const Real _t425 = (0.125)*_b[79];
		const Real _t426 = _t294 + _t373;
		const Real _t427 = (0.125)*_b[60];
		const Real _t428 = (0.125)*_b[42];
		const Real _t429 = (0.5)*_b[61];
		const Real _t430 = (0.25)*_b[73];
		const Real _t431 = (0.25)*_b[45];
		const Real _t432 = (0.5)*_b[62];
		const Real _t433 = (0.5)*_b[63];
		const Real _t434 = (0.375)*_b[74];
		const Real _t435 = (0.25)*_b[80];
		const Real _t436 = (0.0625)*_b[83];
		const Real _t437 = _t398 + _t436;
		const Real _t438 = (0.0625)*_b[76];
		const Real _t439 = _t272 + _t405;
		const Real _t440 = (0.25)*_b[75];
		const Real _t441 = (0.125)*_b[66];
		const Real _t442 = _t392 + _t440 + _t441;
		const Real _t443 = (0.0625)*_b[53];
		const Real _t444 = (0.25)*_b[67];
		const Real _t445 = (0.375)*_b[76];
		const Real _t446 = (0.25)*_b[81];
		const Real _t447 = (0.125)*_b[82];
		const Real _t448 = _t411 + _t415;
		const Real _t449 = (0.125)*_b[70];
		const Real _t450 = (0.5)*_b[71];
		const Real _t451 = (0.25)*_b[79];
		const Real _t452 = (0.25)*_b[60];
		const Real _t453 = (0.5)*_b[72];
		const Real _t454 = (0.5)*_b[73];
		const Real _t455 = (0.125)*_b[83];
		const Real _t456 = _t435 + _t440;
		const Real _t457 = (0.5)*_b[77];
		const Real _t458 = (0.25)*_b[82];
		const Real _t459 = (0.25)*_b[70];
		const Real _t460 = (0.5)*_b[78];
		const Real _t461 = (0.5)*_b[79];
		const Real _t462 = (0.5)*_b[80];
		const Real _t463 = (0.25)*_b[83];
		const Real _t464 = (0.5)*_b[81];
		const Real _t465 = (0.5)*_b[82];
		const Real _t466 = (0.5)*_b[83];
		const Real _t467 = (0.03125)*_b[6];
		const Real _t468 = (0.375)*_b[4];
		const Real _t469 = (0.25)*_b[5];
		const Real _t470 = (0.0625)*_b[6];
		const Real _t471 = (0.125)*_b[6];
		const Real _t472 = (0.5)*_b[5];
		const Real _t473 = (0.25)*_b[6];
		const Real _t474 = (0.5)*_b[6];
		const Real _t475 = (0.375)*_b[10];
		const Real _t476 = (0.25)*_b[11];
		const Real _t477 = (0.0625)*_b[12];
		const Real _t478 = (0.125)*_b[12];
		const Real _t479 = (0.5)*_b[11];
		const Real _t480 = (0.25)*_b[12];
		const Real _t481 = (0.5)*_b[12];
		const Real _t482 = (0.125)*_b[17];
		const Real _t483 = (0.5)*_b[16];
		const Real _t484 = (0.25)*_b[17];
		const Real _t485 = (0.5)*_b[17];
		const Real _t486 = (0.5)*_b[20];
		const Real _t487 = (0.25)*_b[21];
		const Real _t488 = (0.5)*_b[21];
		const Real _t489 = (0.5)*_b[24];
		const Real _t490 = (0.078125)*_b[2] + (0.078125)*_b[5] + _t12 + _t17 + _t305 + _t84 + _t86 + _t87 + _t93;
		const Real _t491 = (0.125)*_b[5];
		const Real _t492 = (0.1875)*_b[31];
		const Real _t493 = (0.1875)*_b[4] + _t492;
		const Real _t494 = (0.125)*_b[32];
		const Real _t495 = _t467 + _t494;
		const Real _t496 = _t319 + _t408 + _t65;
		const Real _t497 = (0.1875)*_b[32];
		const Real _t498 = _t470 + _t497;
		const Real _t499 = (0.0625)*_b[33];
		const Real _t500 = _t280 + _t499;
		const Real _t501 = (0.25)*_b[32];
		const Real _t502 = _t469 + _t501;
		const Real _t503 = (0.125)*_b[33];
		const Real _t504 = _t503 + _t80;
		const Real _t505 = (0.25)*_b[33];
		const Real _t506 = (0.5)*_b[33];
		const Real _t507 = (0.125)*_b[11];
		const Real _t508 = (0.1875)*_b[10] + _t106;
		const Real _t509 = _t290 + _t349;
		const Real _t510 = _t115 + _t116 + _t30 + _t507 + _t508 + _t509 + _t99;
		const Real _t511 = (0.1875)*_b[37];
		const Real _t512 = _t477 + _t511;
		const Real _t513 = _t352 + _t421;
		const Real _t514 = (0.25)*_b[37];
		const Real _t515 = _t476 + _t514;
		const Real _t516 = (0.125)*_b[38];
		const Real _t517 = _t101 + _t516;
		const Real _t518 = (0.25)*_b[38];
		const Real _t519 = (0.5)*_b[38];
		const Real _t520 = (0.1875)*_b[16] + _t129 + _t131 + _t132 + _t136 + _t264 + _t43;
		const Real _t521 = (0.25)*_b[41];
		const Real _t522 = _t42 + _t521;
		const Real _t523 = _t371 + _t428;
		const Real _t524 = (0.5)*_b[42];
		const Real _t525 = _t141 + _t48;
		const Real _t526 = _t143 + _t145 + _t209 + _t50 + _t525;
		const Real _t527 = (0.5)*_b[45];
		const Real _t528 = _t150 + _t55;
		const Real _t529 = (0.0625)*_b[5];
		const Real _t530 = (0.09375)*_b[4] + _t492;
		const Real _t531 = _t17 + _t494;
		const Real _t532 = _t168 + _t176 + _t282 + _t496 + _t529 + _t530 + _t531;
		const Real _t533 = (0.09375)*_b[52];
		const Real _t534 = _t467 + _t497;
		const Real _t535 = _t331 + _t491 + _t501;
		const Real _t536 = _t30 + _t511;
		const Real _t537 = _t186 + _t254;
		const Real _t538 = (0.09375)*_b[10] + _t188;
		const Real _t539 = (0.09375)*_b[11] + _t194 + _t513 + _t536 + _t537 + _t538;
		const Real _t540 = _t416 + _t507 + _t514;
		const Real _t541 = _t103 + _t420;
		const Real _t542 = _t200 + _t39 + _t521;
		const Real _t543 = _t206 + _t43 + _t523 + _t542;
		const Real _t544 = _t144 + _t210 + _t212 + _t431 + _t50;
		const Real _t545 = (0.140625)*_b[52] + (0.046875)*_b[53] + _t233 + _t327;
		const Real _t546 = (0.140625)*_b[31] + (0.046875)*_b[4] + _t234 + _t238;
		const Real _t547 = (0.140625)*_b[32] + (0.046875)*_b[33] + (0.046875)*_b[5] + _t17 + _t545 + _t546;
		const Real _t548 = (0.09375)*_b[31];
		const Real _t549 = _t281 + _t529;
		const Real _t550 = (0.1875)*_b[52];
		const Real _t551 = (0.09375)*_b[53] + _t550;
		const Real _t552 = (0.1875)*_b[53] + _t550;
		const Real _t553 = (0.375)*_b[53];
		const Real _t554 = _t247 + _t27;
		const Real _t555 = (0.09375)*_b[57] + _t189;
		const Real _t556 = (0.09375)*_b[38] + _t253 + _t359 + _t536 + _t554 + _t555;
		const Real _t557 = (0.1875)*_b[57] + _t359;
		const Real _t558 = (0.375)*_b[57];
		const Real _t559 = (0.1875)*_b[42] + _t133 + _t263 + _t376 + _t43;
		const Real _t560 = _t13 + _t274 + _t441;
		const Real _t561 = _t167 + _t279 + _t336 + _t499 + _t531 + _t551 + _t560;
		const Real _t562 = _t288 + _t289;
		const Real _t563 = _t117 + _t30 + _t449 + _t516 + _t557 + _t562;
		const Real _t564 = (0.078125)*_b[33] + (0.078125)*_b[76] + _t17 + _t304 + _t341 + _t409 + _t88;
		const Real _t565 = (0.046875)*_b[67];
		const Real _t566 = (0.109375)*_b[52];
		const Real _t567 = (0.109375)*_b[65];
		const Real _t568 = _t169 + _t81;
		const Real _t569 = (0.109375)*_b[66];
		const Real _t570 = _t282 + _t304;
		const Real _t571 = (0.109375)*_b[50] + _t232;
		const Real _t572 = _t173 + _t319;
		const Real _t573 = (0.046875)*_b[64] + _t230 + _t235;
		const Real _t574 = _t305 + _t315;
		const Real _t575 = _t276 + _t574;
		const Real _t576 = (0.15625)*_b[56] + _t191 + _t352;
		const Real _t577 = (0.15625)*_b[55] + _t109 + _t187;
		const Real _t578 = _t202 + _t264 + _t368;
		const Real _t579 = (0.109375)*_b[31];
		const Real _t580 = _t274 + _t277;
		const Real _t581 = _t276 + _t580;
		const Real _t582 = _t282 + _t572;
		const Real _t583 = (0.109375)*_b[30] + _t232;
		const Real _t584 = (0.15625)*_b[36] + _t247 + _t249 + _t250;
		const Real _t585 = (0.046875)*_b[3] + _t231;
		#pragma endregion
		#pragma region Expressions
		return {{
			{
				_b[0],
				_t0 + _t1,
				(0.25)*_b[2] + _t1 + _t2,
				(0.375)*_b[1] + _t3 + _t4 + _t5,
				_t4 + _t6 + _t7 + _t8 + _t9,
				(0.15625)*_b[1] + (0.3125)*_b[2] + _t10 + _t11 + _t12 + _t13,
				_t18,
				_b[7],
				_t19 + _t20,
				_t20 + _t21 + _t22,
				(0.375)*_b[8] + _t23 + _t24 + _t25,
				_t25 + _t26 + _t27 + _t28 + _t29,
				_t32,
				_b[13],
				_t33 + _t34,
				_t34 + _t35 + _t36,
				(0.375)*_b[14] + _t37 + _t38 + _t39,
				_t44,
				_b[18],
				_t45 + _t46,
				_t46 + _t47 + _t48,
				_t51,
				_b[22],
				_t52 + _t53,
				_t56,
				_b[25],
				_t59,
				_b[27],
				_t0 + _t60,
				_t2 + _t61 + _t63,
				(0.125)*_b[2] + _t3 + _t63 + _t66,
				(0.1875)*_b[1] + _t67 + _t69 + _t71 + _t74,
				_t5 + _t71 + _t75 + _t76 + _t78 + _t82,
				_t94,
				_t19 + _t95,
				_t21 + _t96 + _t98,
				_t102 + _t24 + _t98 + _t99,
				(0.1875)*_b[8] + _t103 + _t105 + _t107 + _t110,
				_t120,
				_t121 + _t33,
				_t122 + _t124 + _t35,
				_t124 + _t125 + _t128 + _t37,
				_t137,
				_t138 + _t45,
				_t139 + _t142 + _t47,
				_t148,
				_t149 + _t52,
				_t151,
				_t153,
				(0.25)*_b[49] + _t2 + _t60,
				(0.125)*_b[49] + _t155 + _t3 + _t61,
				_t155 + _t156 + _t157 + _t158 + _t6 + _t66,
				(0.03125)*_b[52] + _t15 + _t159 + _t160 + _t163 + _t164 + _t165 + _t74,
				_t177,
				_t178 + _t21 + _t95,
				_t179 + _t181 + _t24 + _t96,
				(0.0625)*_b[56] + _t102 + _t181 + _t184 + _t28,
				_t196,
				_t121 + _t197 + _t35,
				_t122 + _t198 + _t201 + _t37,
				_t207,
				_t138 + _t208 + _t47,
				_t214,
				_t216,
				(0.375)*_b[28] + _t217 + _t218 + _t3,
				(0.1875)*_b[28] + _t219 + _t221 + _t223 + _t69,
				(0.03125)*_b[66] + _t164 + _t223 + _t224 + _t225 + _t226 + _t228 + _t229,
				_t240,
				(0.375)*_b[34] + _t24 + _t241 + _t242,
				(0.1875)*_b[34] + _t105 + _t243 + _t245 + _t248,
				_t257,
				(0.375)*_b[39] + _t258 + _t259 + _t37,
				_t266,
				_t268,
				_t217 + _t269 + _t270 + _t6 + _t61,
				_t218 + _t221 + _t271 + _t275 + _t64 + _t78,
				_t284,
				_t241 + _t28 + _t285 + _t286 + _t96,
				_t293,
				_t296,
				(0.15625)*_b[28] + (0.3125)*_b[49] + _t10 + _t297 + _t298 + _t299,
				_t309,
				_t311,
				_t314,
			},
			{
				_t314,
				_t316,
				_t325,
				_t329,
				_t337,
				_t342,
				_t346,
				_t311,
				_t350,
				_t357,
				_t361,
				_t366,
				_t367,
				_t296,
				_t369,
				_t375,
				_t377,
				_t381,
				_t268,
				_t383,
				_t385,
				_t386,
				_t216,
				_t387,
				_t389,
				_t153,
				_t390,
				_b[27],
				(0.3125)*_b[74] + (0.15625)*_b[80] + _t297 + _t300 + _t391 + _t79,
				_t159 + _t218 + _t320 + _t392 + _t393 + _t394,
				(0.03125)*_b[30] + _t228 + _t312 + _t322 + _t395 + _t396 + _t397 + _t399,
				(0.03125)*_b[31] + _t271 + _t334 + _t345 + _t399 + _t400 + _t401 + _t403,
				_t332 + _t394 + _t404 + _t405 + _t406 + _t407,
				(0.3125)*_b[76] + (0.15625)*_b[81] + _t344 + _t391 + _t408 + _t409,
				_t108 + _t178 + _t410 + _t411 + _t412,
				(0.1875)*_b[77] + _t182 + _t348 + _t353 + _t413,
				(0.0625)*_b[36] + _t412 + _t414 + _t417 + _t419,
				(0.1875)*_b[78] + _t358 + _t364 + _t413 + _t420,
				_t412 + _t421 + _t422 + _t423 + _t424,
				(0.375)*_b[71] + _t126 + _t258 + _t425,
				_t198 + _t372 + _t425 + _t426,
				_t374 + _t380 + _t425 + _t427,
				(0.375)*_b[72] + _t379 + _t425 + _t428,
				_t139 + _t429 + _t430,
				_t382 + _t384 + _t430,
				_t430 + _t431 + _t432,
				_t149 + _t433,
				_t388 + _t433,
				_b[48],
				_t156 + _t269 + _t434 + _t435 + _t436,
				(0.1875)*_b[80] + _t219 + _t393 + _t397 + _t437,
				_t158 + _t270 + _t436 + _t438 + _t439 + _t442,
				(0.1875)*_b[81] + _t335 + _t401 + _t407 + _t437,
				_t436 + _t443 + _t444 + _t445 + _t446,
				(0.375)*_b[77] + _t179 + _t410 + _t447,
				_t242 + _t414 + _t447 + _t448,
				_t417 + _t424 + _t447 + _t449,
				(0.375)*_b[78] + _t362 + _t423 + _t447,
				_t197 + _t450 + _t451,
				_t380 + _t426 + _t451,
				_t451 + _t452 + _t453,
				_t429 + _t454,
				_t432 + _t454,
				_b[63],
				(0.375)*_b[80] + _t218 + _t434 + _t455,
				(0.125)*_b[74] + _t439 + _t455 + _t456,
				(0.125)*_b[76] + _t442 + _t446 + _t455,
				(0.375)*_b[81] + _t404 + _t445 + _t455,
				_t285 + _t457 + _t458,
				_t424 + _t448 + _t458,
				_t458 + _t459 + _t460,
				_t450 + _t461,
				_t453 + _t461,
				_b[73],
				(0.25)*_b[74] + _t462 + _t463,
				_t446 + _t456 + _t463,
				(0.25)*_b[76] + _t463 + _t464,
				_t457 + _t465,
				_t460 + _t465,
				_b[79],
				_t462 + _t466,
				_t464 + _t466,
				_b[82],
				_b[83],
			},
			{
				_t18,
				(0.3125)*_b[4] + (0.15625)*_b[5] + _t11 + _t273 + _t467 + _t83,
				_t157 + _t468 + _t469 + _t470 + _t8,
				(0.375)*_b[5] + _t468 + _t471 + _t5,
				(0.25)*_b[4] + _t472 + _t473,
				_t472 + _t474,
				_b[6],
				_t32,
				_t22 + _t246 + _t475 + _t476 + _t477,
				(0.375)*_b[11] + _t475 + _t478 + _t99,
				_t26 + _t479 + _t480,
				_t479 + _t481,
				_b[12],
				_t44,
				(0.375)*_b[16] + _t199 + _t38 + _t482,
				_t36 + _t483 + _t484,
				_t483 + _t485,
				_b[17],
				_t51,
				_t140 + _t486 + _t487,
				_t486 + _t488,
				_b[21],
				_t56,
				_t489 + _t53,
				_b[24],
				_t59,
				_b[26],
				_b[27],
				_t490,
				_t225 + _t491 + _t493 + _t495 + _t496 + _t5,
				(0.1875)*_b[5] + _t493 + _t498 + _t500 + _t67,
				(0.125)*_b[4] + _t471 + _t502 + _t504,
				_t473 + _t502 + _t505,
				_t474 + _t506,
				_t510,
				(0.1875)*_b[11] + _t183 + _t508 + _t512 + _t513,
				_t23 + _t478 + _t515 + _t517,
				_t480 + _t515 + _t518,
				_t481 + _t519,
				_t520,
				_t125 + _t482 + _t522 + _t523,
				_t378 + _t484 + _t522,
				_t485 + _t524,
				_t526,
				_t431 + _t487 + _t525,
				_t488 + _t527,
				_t528,
				_t388 + _t489,
				_t390,
				_t532,
				(0.03125)*_b[50] + _t16 + _t163 + _t406 + _t500 + _t530 + _t533 + _t534,
				_t158 + _t443 + _t470 + _t504 + _t535 + _t9,
				(0.125)*_b[53] + _t471 + _t505 + _t535,
				(0.25)*_b[53] + _t473 + _t506,
				_t539,
				(0.0625)*_b[55] + _t477 + _t517 + _t540 + _t541,
				_t362 + _t478 + _t518 + _t540,
				_t422 + _t480 + _t519,
				_t543,
				_t378 + _t427 + _t482 + _t542,
				_t452 + _t484 + _t524,
				_t544,
				_t384 + _t487 + _t527,
				_t389,
				_t547,
				(0.03125)*_b[65] + _t343 + _t403 + _t534 + _t548 + _t549 + _t551 + _t76,
				(0.1875)*_b[33] + _t335 + _t498 + _t549 + _t552,
				(0.375)*_b[33] + _t404 + _t471 + _t553,
				_t556,
				(0.1875)*_b[38] + _t418 + _t512 + _t554 + _t557,
				(0.375)*_b[38] + _t449 + _t478 + _t558,
				_t559,
				(0.375)*_b[42] + _t370 + _t379 + _t482,
				_t386,
				_t561,
				_t395 + _t404 + _t495 + _t503 + _t552 + _t560,
				_t438 + _t444 + _t470 + _t505 + _t553,
				_t563,
				_t351 + _t459 + _t477 + _t518 + _t558,
				_t381,
				_t564,
				(0.15625)*_b[33] + (0.3125)*_b[53] + _t318 + _t338 + _t344 + _t467,
				_t367,
				_t346,
			},
			{
				_t346,
				_t342,
				_t337,
				_t329,
				_t325,
				_t316,
				_t314,
				_t367,
				_t366,
				_t361,
				_t357,
				_t350,
				_t311,
				_t381,
				_t377,
				_t375,
				_t369,
				_t296,
				_t386,
				_t385,
				_t383,
				_t268,
				_t389,
				_t387,
				_t216,
				_t390,
				_t153,
				_b[27],
				_t564,
				(0.09375)*_b[67] + _t167 + _t222 + _t278 + _t304 + _t339 + _t340 + _t438 + _t443 + _t73 + _t89,
				(0.078125)*_b[31] + _t233 + _t271 + _t304 + _t328 + _t565 + _t566 + _t567 + _t568,
				(0.078125)*_b[30] + _t326 + _t395 + _t569 + _t570 + _t571 + _t572 + _t573,
				(0.09375)*_b[64] + _t156 + _t162 + _t270 + _t280 + _t281 + _t306 + _t307 + _t570 + _t575,
				_t309,
				_t563,
				_t289 + _t360 + _t365 + _t555,
				_t101 + _t419 + _t537 + _t562 + _t576 + _t577,
				_t256 + _t292 + _t349 + _t356,
				_t293,
				_t559,
				_t134 + _t205 + _t263 + _t371 + _t427,
				_t127 + _t198 + _t203 + _t263 + _t578,
				_t266,
				_t544,
				_t147 + _t213,
				_t214,
				_t528,
				_t151,
				_t59,
				_t561,
				(0.078125)*_b[65] + _t545 + _t565 + _t569 + _t579 + _t581 + _t76 + _t89,
				(0.15625)*_b[51] + _t160 + _t161 + _t226 + _t227 + _t396 + _t400 + _t402 + _t533 + _t548 + _t568 + _t581 + _t582,
				(0.078125)*_b[66] + _t225 + _t237 + _t567 + _t573 + _t575 + _t580 + _t583,
				_t284,
				_t556,
				_t118 + _t180 + _t254 + _t541 + _t576 + _t584,
				_t184 + _t186 + _t416 + _t509 + _t577 + _t584,
				_t257,
				_t543,
				_t125 + _t134 + _t200 + _t204 + _t578,
				_t207,
				_t526,
				_t148,
				_t56,
				_t547,
				(0.078125)*_b[50] + _t406 + _t546 + _t566 + _t582 + _t583 + _t585 + _t89,
				(0.078125)*_b[52] + _t159 + _t234 + _t239 + _t568 + _t571 + _t574 + _t579 + _t585,
				_t240,
				_t539,
				_t112 + _t119 + _t186 + _t195 + _t290 + _t538,
				_t196,
				_t520,
				_t137,
				_t51,
				_t532,
				(0.09375)*_b[3] + _t157 + _t168 + _t175 + _t574 + _t89 + _t9 + _t91 + _t92,
				_t177,
				_t510,
				_t120,
				_t44,
				_t490,
				_t94,
				_t32,
				_t18,
			},
		}};
		#pragma endregion
	}
}
