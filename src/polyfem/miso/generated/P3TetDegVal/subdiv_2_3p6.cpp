#include "../P3TetDegVal.hpp"
namespace miso {
	std::array<RealVector<84>, 4> P3TetDegVal::subdiv_2_3p6(const RealVector<84> &_b) {
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
		const Real _t20 = (0.25)*_b[7];
		const Real _t21 = (0.25)*_b[8];
		const Real _t22 = _t21 + _t7;
		const Real _t23 = (0.125)*_b[7];
		const Real _t24 = (0.125)*_b[9];
		const Real _t25 = _t23 + _t24;
		const Real _t26 = (0.0625)*_b[3];
		const Real _t27 = (0.1875)*_b[8];
		const Real _t28 = _t27 + _t6;
		const Real _t29 = (0.1875)*_b[9];
		const Real _t30 = (0.1875)*_b[2] + _t29;
		const Real _t31 = (0.0625)*_b[10];
		const Real _t32 = (0.0625)*_b[7];
		const Real _t33 = _t31 + _t32;
		const Real _t34 = (0.125)*_b[1];
		const Real _t35 = (0.03125)*_b[4];
		const Real _t36 = (0.125)*_b[8];
		const Real _t37 = _t10 + _t36;
		const Real _t38 = (0.03125)*_b[7];
		const Real _t39 = (0.125)*_b[10];
		const Real _t40 = (0.03125)*_b[11];
		const Real _t41 = _t38 + _t39 + _t40;
		const Real _t42 = (0.15625)*_b[2];
		const Real _t43 = (0.078125)*_b[8];
		const Real _t44 = _t14 + _t43;
		const Real _t45 = (0.015625)*_b[12];
		const Real _t46 = (0.078125)*_b[11];
		const Real _t47 = (0.015625)*_b[5] + _t46;
		const Real _t48 = _t45 + _t47;
		const Real _t49 = (0.015625)*_b[7];
		const Real _t50 = (0.15625)*_b[10];
		const Real _t51 = (0.15625)*_b[9];
		const Real _t52 = (0.15625)*_b[3] + _t49 + _t50 + _t51;
		const Real _t53 = (0.078125)*_b[1] + (0.078125)*_b[4] + _t42 + _t44 + _t48 + _t52;
		const Real _t54 = (0.125)*_b[14];
		const Real _t55 = _t21 + _t34 + _t54;
		const Real _t56 = (0.0625)*_b[13];
		const Real _t57 = (0.0625)*_b[2];
		const Real _t58 = (0.0625)*_b[15];
		const Real _t59 = (0.03125)*_b[13];
		const Real _t60 = (0.09375)*_b[14];
		const Real _t61 = (0.03125)*_b[3];
		const Real _t62 = (0.09375)*_b[15];
		const Real _t63 = _t61 + _t62;
		const Real _t64 = _t10 + _t27;
		const Real _t65 = (0.09375)*_b[2] + _t29;
		const Real _t66 = (0.0625)*_b[1];
		const Real _t67 = (0.015625)*_b[17];
		const Real _t68 = (0.015625)*_b[4];
		const Real _t69 = _t67 + _t68;
		const Real _t70 = _t14 + _t36;
		const Real _t71 = (0.0625)*_b[14];
		const Real _t72 = (0.0625)*_b[16];
		const Real _t73 = (0.015625)*_b[13];
		const Real _t74 = _t62 + _t73;
		const Real _t75 = _t71 + _t72 + _t74;
		const Real _t76 = _t26 + _t75;
		const Real _t77 = _t41 + _t65 + _t66 + _t69 + _t70 + _t76;
		const Real _t78 = (0.375)*_b[13];
		const Real _t79 = (0.125)*_b[18];
		const Real _t80 = (0.0625)*_b[18];
		const Real _t81 = (0.1875)*_b[14];
		const Real _t82 = (0.1875)*_b[13] + _t81;
		const Real _t83 = (0.0625)*_b[19];
		const Real _t84 = _t66 + _t83;
		const Real _t85 = (0.09375)*_b[7];
		const Real _t86 = (0.03125)*_b[2];
		const Real _t87 = (0.09375)*_b[9];
		const Real _t88 = (0.03125)*_b[18];
		const Real _t89 = _t62 + _t88;
		const Real _t90 = (0.09375)*_b[13] + _t81;
		const Real _t91 = (0.015625)*_b[18];
		const Real _t92 = (0.015625)*_b[3];
		const Real _t93 = (0.140625)*_b[15];
		const Real _t94 = _t91 + _t92 + _t93;
		const Real _t95 = (0.015625)*_b[21];
		const Real _t96 = (0.046875)*_b[10] + (0.046875)*_b[16] + _t95;
		const Real _t97 = _t94 + _t96;
		const Real _t98 = (0.046875)*_b[13] + (0.140625)*_b[14];
		const Real _t99 = (0.046875)*_b[19] + (0.046875)*_b[20];
		const Real _t100 = (0.046875)*_b[2] + (0.140625)*_b[9] + _t99;
		const Real _t101 = (0.046875)*_b[1] + (0.046875)*_b[7] + (0.140625)*_b[8] + _t100 + _t14 + _t97 + _t98;
		const Real _t102 = (0.25)*_b[18];
		const Real _t103 = (0.0625)*_b[22];
		const Real _t104 = (0.03125)*_b[22];
		const Real _t105 = (0.125)*_b[19];
		const Real _t106 = (0.03125)*_b[1];
		const Real _t107 = (0.03125)*_b[23];
		const Real _t108 = _t105 + _t106 + _t107;
		const Real _t109 = (0.015625)*_b[24];
		const Real _t110 = (0.015625)*_b[22];
		const Real _t111 = _t110 + _t62;
		const Real _t112 = _t109 + _t111;
		const Real _t113 = (0.0625)*_b[9];
		const Real _t114 = (0.0625)*_b[20];
		const Real _t115 = (0.015625)*_b[2];
		const Real _t116 = _t113 + _t114 + _t115 + _t80;
		const Real _t117 = _t108 + _t112 + _t116 + _t32 + _t70 + _t90;
		const Real _t118 = (0.3125)*_b[18];
		const Real _t119 = (0.15625)*_b[22];
		const Real _t120 = (0.03125)*_b[25];
		const Real _t121 = (0.15625)*_b[13];
		const Real _t122 = (0.015625)*_b[26];
		const Real _t123 = (0.078125)*_b[23];
		const Real _t124 = (0.015625)*_b[25] + _t123;
		const Real _t125 = _t122 + _t124;
		const Real _t126 = (0.015625)*_b[1];
		const Real _t127 = (0.15625)*_b[14];
		const Real _t128 = (0.15625)*_b[19];
		const Real _t129 = (0.15625)*_b[18] + _t126 + _t127 + _t128;
		const Real _t130 = (0.078125)*_b[22] + (0.078125)*_b[7] + _t121 + _t125 + _t129 + _t44;
		const Real _t131 = (0.09375)*_b[25];
		const Real _t132 = (0.015625)*_b[27];
		const Real _t133 = (0.234375)*_b[13] + (0.234375)*_b[22] + _t118 + _t131 + _t132 + _t14 + _t85;
		const Real _t134 = (0.5)*_b[28];
		const Real _t135 = (0.5)*_b[29];
		const Real _t136 = (0.25)*_b[28];
		const Real _t137 = (0.25)*_b[30];
		const Real _t138 = (0.125)*_b[28];
		const Real _t139 = (0.375)*_b[30];
		const Real _t140 = (0.125)*_b[31];
		const Real _t141 = (0.0625)*_b[28];
		const Real _t142 = (0.25)*_b[29];
		const Real _t143 = (0.25)*_b[31];
		const Real _t144 = (0.0625)*_b[32];
		const Real _t145 = (0.03125)*_b[28];
		const Real _t146 = (0.03125)*_b[33];
		const Real _t147 = (0.15625)*_b[29] + (0.3125)*_b[30] + (0.3125)*_b[31] + (0.15625)*_b[32] + _t145 + _t146;
		const Real _t148 = (0.5)*_b[34];
		const Real _t149 = (0.25)*_b[34];
		const Real _t150 = (0.25)*_b[35];
		const Real _t151 = _t142 + _t150;
		const Real _t152 = (0.125)*_b[30];
		const Real _t153 = (0.125)*_b[34];
		const Real _t154 = (0.125)*_b[36];
		const Real _t155 = _t153 + _t154;
		const Real _t156 = (0.0625)*_b[31];
		const Real _t157 = (0.1875)*_b[35];
		const Real _t158 = _t141 + _t157;
		const Real _t159 = (0.1875)*_b[36];
		const Real _t160 = (0.1875)*_b[30] + _t159;
		const Real _t161 = (0.0625)*_b[34];
		const Real _t162 = (0.0625)*_b[37];
		const Real _t163 = _t161 + _t162;
		const Real _t164 = (0.125)*_b[29];
		const Real _t165 = (0.125)*_b[35];
		const Real _t166 = _t145 + _t165;
		const Real _t167 = (0.03125)*_b[34];
		const Real _t168 = (0.03125)*_b[38];
		const Real _t169 = (0.125)*_b[37];
		const Real _t170 = (0.03125)*_b[32] + _t169;
		const Real _t171 = _t168 + _t170;
		const Real _t172 = _t167 + _t171;
		const Real _t173 = _t140 + _t160 + _t164 + _t166 + _t172;
		const Real _t174 = (0.25)*_b[39];
		const Real _t175 = (0.125)*_b[39];
		const Real _t176 = (0.125)*_b[40];
		const Real _t177 = _t150 + _t164 + _t176;
		const Real _t178 = (0.0625)*_b[39];
		const Real _t179 = (0.0625)*_b[30];
		const Real _t180 = _t178 + _t179;
		const Real _t181 = _t145 + _t157;
		const Real _t182 = (0.03125)*_b[42];
		const Real _t183 = (0.03125)*_b[31];
		const Real _t184 = (0.09375)*_b[40];
		const Real _t185 = _t183 + _t184;
		const Real _t186 = _t182 + _t185;
		const Real _t187 = (0.03125)*_b[39];
		const Real _t188 = (0.09375)*_b[41];
		const Real _t189 = _t187 + _t188;
		const Real _t190 = _t159 + _t189;
		const Real _t191 = (0.09375)*_b[30] + _t190;
		const Real _t192 = (0.09375)*_b[29] + _t163 + _t181 + _t186 + _t191;
		const Real _t193 = (0.375)*_b[39];
		const Real _t194 = (0.125)*_b[43];
		const Real _t195 = (0.0625)*_b[43];
		const Real _t196 = (0.1875)*_b[40];
		const Real _t197 = (0.1875)*_b[39] + _t196;
		const Real _t198 = (0.0625)*_b[29];
		const Real _t199 = (0.0625)*_b[44];
		const Real _t200 = _t198 + _t199;
		const Real _t201 = (0.03125)*_b[45];
		const Real _t202 = (0.03125)*_b[43];
		const Real _t203 = (0.09375)*_b[36];
		const Real _t204 = _t202 + _t203;
		const Real _t205 = _t201 + _t204;
		const Real _t206 = (0.03125)*_b[30];
		const Real _t207 = _t196 + _t206;
		const Real _t208 = (0.09375)*_b[39] + _t188 + _t207;
		const Real _t209 = (0.09375)*_b[34] + _t181 + _t200 + _t205 + _t208;
		const Real _t210 = (0.25)*_b[43];
		const Real _t211 = (0.0625)*_b[46];
		const Real _t212 = (0.125)*_b[44];
		const Real _t213 = (0.03125)*_b[46] + _t212;
		const Real _t214 = (0.03125)*_b[29];
		const Real _t215 = (0.03125)*_b[47];
		const Real _t216 = _t214 + _t215;
		const Real _t217 = _t213 + _t216;
		const Real _t218 = _t153 + _t166 + _t194 + _t197 + _t217;
		const Real _t219 = (0.03125)*_b[48];
		const Real _t220 = (0.15625)*_b[34] + (0.3125)*_b[39] + (0.3125)*_b[43] + (0.15625)*_b[46] + _t145 + _t219;
		const Real _t221 = (0.5)*_b[49];
		const Real _t222 = (0.5)*_b[50];
		const Real _t223 = (0.25)*_b[49];
		const Real _t224 = (0.25)*_b[51];
		const Real _t225 = (0.125)*_b[49];
		const Real _t226 = (0.375)*_b[51];
		const Real _t227 = (0.125)*_b[52];
		const Real _t228 = (0.0625)*_b[49];
		const Real _t229 = (0.25)*_b[50];
		const Real _t230 = (0.25)*_b[52];
		const Real _t231 = (0.0625)*_b[53];
		const Real _t232 = _t226 + _t228 + _t229 + _t230 + _t231;
		const Real _t233 = (0.5)*_b[54];
		const Real _t234 = (0.25)*_b[54];
		const Real _t235 = (0.25)*_b[55];
		const Real _t236 = _t229 + _t235;
		const Real _t237 = (0.125)*_b[51];
		const Real _t238 = (0.125)*_b[54];
		const Real _t239 = (0.125)*_b[56];
		const Real _t240 = _t238 + _t239;
		const Real _t241 = (0.1875)*_b[55];
		const Real _t242 = _t228 + _t241;
		const Real _t243 = (0.0625)*_b[57];
		const Real _t244 = (0.1875)*_b[56];
		const Real _t245 = (0.0625)*_b[52] + _t244;
		const Real _t246 = _t243 + _t245;
		const Real _t247 = (0.0625)*_b[54];
		const Real _t248 = (0.1875)*_b[51] + _t247;
		const Real _t249 = (0.1875)*_b[50] + _t242 + _t246 + _t248;
		const Real _t250 = (0.25)*_b[58];
		const Real _t251 = (0.125)*_b[58];
		const Real _t252 = (0.125)*_b[50];
		const Real _t253 = (0.125)*_b[59];
		const Real _t254 = _t235 + _t252 + _t253;
		const Real _t255 = (0.0625)*_b[60];
		const Real _t256 = (0.0625)*_b[51];
		const Real _t257 = (0.0625)*_b[58];
		const Real _t258 = _t256 + _t257;
		const Real _t259 = _t255 + _t258;
		const Real _t260 = _t228 + _t240 + _t254 + _t259;
		const Real _t261 = (0.375)*_b[58];
		const Real _t262 = (0.125)*_b[61];
		const Real _t263 = (0.0625)*_b[62];
		const Real _t264 = (0.1875)*_b[59];
		const Real _t265 = (0.0625)*_b[61] + _t264;
		const Real _t266 = _t263 + _t265;
		const Real _t267 = (0.0625)*_b[50];
		const Real _t268 = (0.1875)*_b[58] + _t267;
		const Real _t269 = (0.1875)*_b[54] + _t242 + _t266 + _t268;
		const Real _t270 = (0.25)*_b[61];
		const Real _t271 = (0.0625)*_b[63];
		const Real _t272 = _t228 + _t234 + _t261 + _t270 + _t271;
		const Real _t273 = (0.5)*_b[64];
		const Real _t274 = (0.5)*_b[65];
		const Real _t275 = (0.25)*_b[64];
		const Real _t276 = (0.25)*_b[66];
		const Real _t277 = (0.125)*_b[64];
		const Real _t278 = (0.125)*_b[67];
		const Real _t279 = (0.375)*_b[65] + (0.375)*_b[66] + _t277 + _t278;
		const Real _t280 = (0.5)*_b[68];
		const Real _t281 = (0.25)*_b[68];
		const Real _t282 = (0.25)*_b[65];
		const Real _t283 = (0.25)*_b[69];
		const Real _t284 = _t282 + _t283;
		const Real _t285 = (0.125)*_b[68];
		const Real _t286 = (0.125)*_b[66];
		const Real _t287 = (0.125)*_b[70];
		const Real _t288 = _t286 + _t287;
		const Real _t289 = _t285 + _t288;
		const Real _t290 = _t277 + _t284 + _t289;
		const Real _t291 = (0.25)*_b[71];
		const Real _t292 = (0.125)*_b[65];
		const Real _t293 = (0.125)*_b[72];
		const Real _t294 = _t292 + _t293;
		const Real _t295 = (0.125)*_b[71] + _t283;
		const Real _t296 = _t294 + _t295;
		const Real _t297 = _t277 + _t281 + _t296;
		const Real _t298 = (0.125)*_b[73];
		const Real _t299 = (0.375)*_b[68] + (0.375)*_b[71] + _t277 + _t298;
		const Real _t300 = (0.5)*_b[74];
		const Real _t301 = (0.5)*_b[75];
		const Real _t302 = (0.25)*_b[74];
		const Real _t303 = (0.25)*_b[76];
		const Real _t304 = _t301 + _t302 + _t303;
		const Real _t305 = (0.5)*_b[77];
		const Real _t306 = (0.25)*_b[75] + (0.25)*_b[77] + (0.25)*_b[78];
		const Real _t307 = _t302 + _t306;
		const Real _t308 = (0.25)*_b[79];
		const Real _t309 = _t302 + _t305 + _t308;
		const Real _t310 = (0.5)*_b[80];
		const Real _t311 = (0.5)*_b[81];
		const Real _t312 = _t310 + _t311;
		const Real _t313 = (0.5)*_b[82];
		const Real _t314 = _t310 + _t313;
		const Real _t315 = _t43 + _t49;
		const Real _t316 = (0.078125)*_b[13] + (0.078125)*_b[25] + _t119 + _t122 + _t123 + _t129 + _t132 + _t315;
		const Real _t317 = (0.0625)*_b[25];
		const Real _t318 = (0.03125)*_b[26];
		const Real _t319 = (0.03125)*_b[8];
		const Real _t320 = _t318 + _t319 + _t54;
		const Real _t321 = (0.1875)*_b[19];
		const Real _t322 = (0.09375)*_b[22] + _t321;
		const Real _t323 = (0.125)*_b[23];
		const Real _t324 = _t132 + _t323;
		const Real _t325 = _t109 + _t116 + _t317 + _t320 + _t322 + _t324 + _t74;
		const Real _t326 = (0.140625)*_b[19] + (0.046875)*_b[22];
		const Real _t327 = (0.046875)*_b[14] + (0.046875)*_b[9];
		const Real _t328 = (0.140625)*_b[20] + (0.046875)*_b[24] + _t327;
		const Real _t329 = (0.140625)*_b[23] + (0.046875)*_b[25] + (0.046875)*_b[26] + _t132 + _t326 + _t328 + _t97;
		const Real _t330 = (0.0625)*_b[26];
		const Real _t331 = (0.125)*_b[16];
		const Real _t332 = _t120 + _t331 + _t40;
		const Real _t333 = (0.1875)*_b[20];
		const Real _t334 = (0.09375)*_b[24] + _t333;
		const Real _t335 = (0.0625)*_b[21];
		const Real _t336 = _t31 + _t335 + _t83;
		const Real _t337 = _t111 + _t324 + _t330 + _t332 + _t334 + _t336 + _t69;
		const Real _t338 = (0.15625)*_b[24];
		const Real _t339 = (0.15625)*_b[16];
		const Real _t340 = (0.15625)*_b[20];
		const Real _t341 = (0.15625)*_b[21] + _t339 + _t340;
		const Real _t342 = (0.078125)*_b[17] + (0.078125)*_b[26] + _t124 + _t132 + _t338 + _t341 + _t48;
		const Real _t343 = (0.09375)*_b[12];
		const Real _t344 = (0.3125)*_b[21];
		const Real _t345 = (0.09375)*_b[26];
		const Real _t346 = (0.234375)*_b[17] + (0.234375)*_b[24] + _t132 + _t17 + _t343 + _t344 + _t345;
		const Real _t347 = (0.03125)*_b[27];
		const Real _t348 = (0.125)*_b[25];
		const Real _t349 = (0.1875)*_b[22] + _t321;
		const Real _t350 = _t323 + _t347;
		const Real _t351 = (0.03125)*_b[24];
		const Real _t352 = (0.09375)*_b[20];
		const Real _t353 = _t330 + _t71;
		const Real _t354 = (0.1875)*_b[23];
		const Real _t355 = _t347 + _t354;
		const Real _t356 = (0.09375)*_b[19];
		const Real _t357 = _t317 + _t72;
		const Real _t358 = (0.03125)*_b[21];
		const Real _t359 = _t358 + _t62;
		const Real _t360 = (0.125)*_b[21];
		const Real _t361 = (0.125)*_b[26];
		const Real _t362 = (0.03125)*_b[17];
		const Real _t363 = (0.1875)*_b[24] + _t333;
		const Real _t364 = (0.03125)*_b[12];
		const Real _t365 = (0.15625)*_b[17];
		const Real _t366 = (0.375)*_b[22];
		const Real _t367 = (0.25)*_b[25];
		const Real _t368 = (0.0625)*_b[27];
		const Real _t369 = _t354 + _t368;
		const Real _t370 = (0.0625)*_b[24];
		const Real _t371 = _t105 + _t361;
		const Real _t372 = (0.25)*_b[23];
		const Real _t373 = (0.125)*_b[20];
		const Real _t374 = _t348 + _t372 + _t373;
		const Real _t375 = (0.0625)*_b[17];
		const Real _t376 = (0.25)*_b[21];
		const Real _t377 = (0.375)*_b[24];
		const Real _t378 = (0.25)*_b[26];
		const Real _t379 = (0.125)*_b[27];
		const Real _t380 = _t367 + _t372;
		const Real _t381 = (0.5)*_b[25];
		const Real _t382 = (0.25)*_b[27];
		const Real _t383 = (0.5)*_b[26];
		const Real _t384 = (0.5)*_b[27];
		const Real _t385 = (0.125)*_b[46];
		const Real _t386 = (0.1875)*_b[43] + _t196;
		const Real _t387 = _t165 + _t167;
		const Real _t388 = _t175 + _t212 + _t216 + _t219 + _t385 + _t386 + _t387;
		const Real _t389 = (0.0625)*_b[47];
		const Real _t390 = (0.0625)*_b[35];
		const Real _t391 = _t389 + _t390;
		const Real _t392 = (0.1875)*_b[44];
		const Real _t393 = _t219 + _t392;
		const Real _t394 = (0.09375)*_b[43] + _t201 + _t203;
		const Real _t395 = (0.09375)*_b[46] + _t189 + _t207 + _t391 + _t393 + _t394;
		const Real _t396 = _t162 + _t211;
		const Real _t397 = (0.1875)*_b[41];
		const Real _t398 = (0.09375)*_b[45] + _t204 + _t397;
		const Real _t399 = (0.09375)*_b[47] + _t186 + _t393 + _t396 + _t398;
		const Real _t400 = (0.125)*_b[42];
		const Real _t401 = (0.125)*_b[47];
		const Real _t402 = (0.1875)*_b[45] + _t397;
		const Real _t403 = _t171 + _t213;
		const Real _t404 = _t219 + _t400 + _t401 + _t402 + _t403;
		const Real _t405 = (0.15625)*_b[38] + (0.3125)*_b[42] + (0.3125)*_b[45] + (0.15625)*_b[47] + _t146 + _t219;
		const Real _t406 = (0.375)*_b[43];
		const Real _t407 = (0.25)*_b[46];
		const Real _t408 = (0.0625)*_b[48];
		const Real _t409 = _t392 + _t408;
		const Real _t410 = _t176 + _t401;
		const Real _t411 = (0.25)*_b[44];
		const Real _t412 = (0.125)*_b[41];
		const Real _t413 = _t385 + _t411 + _t412;
		const Real _t414 = (0.0625)*_b[45];
		const Real _t415 = _t195 + _t414;
		const Real _t416 = (0.0625)*_b[42];
		const Real _t417 = (0.0625)*_b[38];
		const Real _t418 = (0.25)*_b[42];
		const Real _t419 = (0.375)*_b[45];
		const Real _t420 = (0.25)*_b[47];
		const Real _t421 = (0.125)*_b[48];
		const Real _t422 = _t407 + _t411;
		const Real _t423 = (0.125)*_b[45];
		const Real _t424 = (0.5)*_b[46];
		const Real _t425 = (0.25)*_b[48];
		const Real _t426 = (0.25)*_b[45];
		const Real _t427 = (0.5)*_b[47];
		const Real _t428 = (0.5)*_b[48];
		const Real _t429 = _t241 + _t247;
		const Real _t430 = (0.1875)*_b[61] + _t263 + _t264 + _t268 + _t271 + _t429;
		const Real _t431 = (0.125)*_b[62];
		const Real _t432 = (0.125)*_b[55];
		const Real _t433 = _t431 + _t432;
		const Real _t434 = (0.25)*_b[59];
		const Real _t435 = _t239 + _t262 + _t434;
		const Real _t436 = _t259 + _t271 + _t433 + _t435;
		const Real _t437 = (0.1875)*_b[60];
		const Real _t438 = (0.1875)*_b[62] + _t246 + _t265 + _t271 + _t437;
		const Real _t439 = (0.25)*_b[57];
		const Real _t440 = (0.375)*_b[60];
		const Real _t441 = (0.25)*_b[62];
		const Real _t442 = _t231 + _t271 + _t439 + _t440 + _t441;
		const Real _t443 = (0.125)*_b[63];
		const Real _t444 = _t270 + _t434;
		const Real _t445 = (0.125)*_b[60];
		const Real _t446 = (0.125)*_b[57];
		const Real _t447 = (0.5)*_b[61];
		const Real _t448 = (0.25)*_b[63];
		const Real _t449 = (0.25)*_b[60];
		const Real _t450 = (0.5)*_b[62];
		const Real _t451 = (0.5)*_b[63];
		const Real _t452 = _t283 + _t291;
		const Real _t453 = _t285 + _t294 + _t298 + _t452;
		const Real _t454 = (0.25)*_b[72];
		const Real _t455 = _t288 + _t295 + _t298 + _t454;
		const Real _t456 = (0.375)*_b[70] + (0.375)*_b[72] + _t278 + _t298;
		const Real _t457 = (0.5)*_b[71];
		const Real _t458 = (0.25)*_b[73];
		const Real _t459 = (0.25)*_b[70];
		const Real _t460 = (0.5)*_b[72];
		const Real _t461 = (0.5)*_b[73];
		const Real _t462 = _t306 + _t308;
		const Real _t463 = (0.5)*_b[78];
		const Real _t464 = _t303 + _t308 + _t463;
		const Real _t465 = (0.5)*_b[79];
		const Real _t466 = _t311 + _t313;
		const Real _t467 = (0.03125)*_b[6];
		const Real _t468 = (0.375)*_b[4];
		const Real _t469 = (0.25)*_b[5];
		const Real _t470 = (0.0625)*_b[6];
		const Real _t471 = (0.125)*_b[6];
		const Real _t472 = (0.5)*_b[5];
		const Real _t473 = (0.25)*_b[6];
		const Real _t474 = (0.5)*_b[6];
		const Real _t475 = (0.078125)*_b[2] + (0.078125)*_b[5] + _t12 + _t126 + _t17 + _t43 + _t45 + _t46 + _t52;
		const Real _t476 = (0.125)*_b[5];
		const Real _t477 = (0.1875)*_b[10];
		const Real _t478 = (0.1875)*_b[4] + _t477;
		const Real _t479 = (0.125)*_b[11];
		const Real _t480 = _t467 + _t479;
		const Real _t481 = _t24 + _t319 + _t364;
		const Real _t482 = (0.1875)*_b[11];
		const Real _t483 = _t470 + _t482;
		const Real _t484 = (0.0625)*_b[12];
		const Real _t485 = _t113 + _t484;
		const Real _t486 = (0.25)*_b[11];
		const Real _t487 = _t469 + _t486;
		const Real _t488 = (0.125)*_b[12];
		const Real _t489 = _t39 + _t488;
		const Real _t490 = (0.25)*_b[12];
		const Real _t491 = (0.5)*_b[12];
		const Real _t492 = (0.0625)*_b[5];
		const Real _t493 = (0.09375)*_b[4] + _t477;
		const Real _t494 = _t17 + _t479;
		const Real _t495 = _t115 + _t481 + _t492 + _t493 + _t494 + _t67 + _t76;
		const Real _t496 = (0.09375)*_b[16];
		const Real _t497 = _t467 + _t482;
		const Real _t498 = _t331 + _t476 + _t486;
		const Real _t499 = (0.140625)*_b[16] + (0.046875)*_b[17] + _t327 + _t94;
		const Real _t500 = (0.140625)*_b[10] + (0.046875)*_b[4] + _t95 + _t99;
		const Real _t501 = (0.140625)*_b[11] + (0.046875)*_b[12] + (0.046875)*_b[5] + _t17 + _t499 + _t500;
		const Real _t502 = (0.09375)*_b[10];
		const Real _t503 = _t114 + _t492;
		const Real _t504 = (0.1875)*_b[16];
		const Real _t505 = (0.09375)*_b[17] + _t504;
		const Real _t506 = (0.1875)*_b[17] + _t504;
		const Real _t507 = (0.375)*_b[17];
		const Real _t508 = _t107 + _t13 + _t373;
		const Real _t509 = _t112 + _t336 + _t484 + _t494 + _t505 + _t508 + _t68;
		const Real _t510 = (0.078125)*_b[12] + (0.078125)*_b[24] + _t125 + _t17 + _t341 + _t365 + _t47;
		const Real _t511 = (0.375)*_b[31];
		const Real _t512 = (0.25)*_b[32];
		const Real _t513 = (0.0625)*_b[33];
		const Real _t514 = (0.125)*_b[33];
		const Real _t515 = (0.5)*_b[32];
		const Real _t516 = (0.25)*_b[33];
		const Real _t517 = (0.5)*_b[33];
		const Real _t518 = (0.125)*_b[32];
		const Real _t519 = (0.1875)*_b[31] + _t159;
		const Real _t520 = _t214 + _t387;
		const Real _t521 = _t146 + _t152 + _t168 + _t169 + _t518 + _t519 + _t520;
		const Real _t522 = (0.1875)*_b[37];
		const Real _t523 = _t513 + _t522;
		const Real _t524 = _t390 + _t417;
		const Real _t525 = (0.25)*_b[37];
		const Real _t526 = _t512 + _t525;
		const Real _t527 = (0.125)*_b[38];
		const Real _t528 = _t154 + _t527;
		const Real _t529 = (0.25)*_b[38];
		const Real _t530 = (0.5)*_b[38];
		const Real _t531 = _t146 + _t522;
		const Real _t532 = _t182 + _t206;
		const Real _t533 = (0.09375)*_b[31] + _t184;
		const Real _t534 = (0.09375)*_b[32] + _t190 + _t524 + _t531 + _t532 + _t533;
		const Real _t535 = _t412 + _t518 + _t525;
		const Real _t536 = _t156 + _t416;
		const Real _t537 = _t144 + _t199;
		const Real _t538 = (0.09375)*_b[42] + _t185;
		const Real _t539 = (0.09375)*_b[38] + _t205 + _t397 + _t531 + _t537 + _t538;
		const Real _t540 = (0.1875)*_b[42] + _t397;
		const Real _t541 = (0.375)*_b[42];
		const Real _t542 = _t213 + _t215;
		const Real _t543 = _t146 + _t170 + _t423 + _t527 + _t540 + _t542;
		const Real _t544 = (0.125)*_b[53];
		const Real _t545 = (0.5)*_b[52];
		const Real _t546 = (0.25)*_b[53];
		const Real _t547 = (0.5)*_b[53];
		const Real _t548 = (0.1875)*_b[52] + _t231 + _t241 + _t243 + _t244 + _t248 + _t267;
		const Real _t549 = (0.25)*_b[56];
		const Real _t550 = _t230 + _t549;
		const Real _t551 = _t432 + _t446;
		const Real _t552 = (0.5)*_b[57];
		const Real _t553 = _t227 + _t253 + _t549;
		const Real _t554 = _t231 + _t259 + _t551 + _t553;
		const Real _t555 = (0.1875)*_b[57] + _t231 + _t245 + _t266 + _t437;
		const Real _t556 = (0.5)*_b[66];
		const Real _t557 = (0.25)*_b[67];
		const Real _t558 = (0.5)*_b[67];
		const Real _t559 = _t276 + _t283;
		const Real _t560 = _t278 + _t285 + _t287 + _t292 + _t559;
		const Real _t561 = (0.5)*_b[70];
		const Real _t562 = _t278 + _t286 + _t293 + _t295 + _t459;
		const Real _t563 = (0.5)*_b[76];
		const Real _t564 = _t303 + _t306;
		const Real _t565 = (0.046875)*_b[21];
		const Real _t566 = (0.109375)*_b[16];
		const Real _t567 = (0.109375)*_b[19];
		const Real _t568 = _t40 + _t69;
		const Real _t569 = (0.109375)*_b[20];
		const Real _t570 = _t115 + _t125;
		const Real _t571 = (0.109375)*_b[14] + _t93;
		const Real _t572 = _t319 + _t73;
		const Real _t573 = (0.046875)*_b[18] + _t92 + _t96;
		const Real _t574 = _t126 + _t315;
		const Real _t575 = _t109 + _t574;
		const Real _t576 = (0.109375)*_b[10];
		const Real _t577 = _t107 + _t110;
		const Real _t578 = _t109 + _t577;
		const Real _t579 = _t115 + _t572;
		const Real _t580 = (0.109375)*_b[9] + _t93;
		const Real _t581 = (0.046875)*_b[3] + _t91;
		const Real _t582 = (0.15625)*_b[41] + _t187 + _t390;
		const Real _t583 = (0.15625)*_b[40] + _t162 + _t183;
		const Real _t584 = (0.15625)*_b[36] + _t199 + _t201 + _t202;
		const Real _t585 = _t255 + _t267 + _t429;
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
				_t0 + _t19,
				_t2 + _t20 + _t22,
				(0.125)*_b[2] + _t22 + _t25 + _t3,
				(0.1875)*_b[1] + _t26 + _t28 + _t30 + _t33,
				_t30 + _t34 + _t35 + _t37 + _t41 + _t5,
				_t53,
				(0.25)*_b[13] + _t19 + _t2,
				(0.125)*_b[13] + _t20 + _t3 + _t55,
				_t25 + _t55 + _t56 + _t57 + _t58 + _t6,
				(0.03125)*_b[16] + _t15 + _t33 + _t59 + _t60 + _t63 + _t64 + _t65,
				_t77,
				(0.375)*_b[7] + _t3 + _t78 + _t79,
				(0.1875)*_b[7] + _t28 + _t80 + _t82 + _t84,
				(0.03125)*_b[20] + _t64 + _t84 + _t85 + _t86 + _t87 + _t89 + _t90,
				_t101,
				_t102 + _t103 + _t20 + _t6 + _t78,
				_t104 + _t108 + _t23 + _t37 + _t79 + _t82,
				_t117,
				(0.3125)*_b[13] + (0.15625)*_b[7] + _t10 + _t118 + _t119 + _t120,
				_t130,
				_t133,
				_b[28],
				_t134 + _t135,
				_t135 + _t136 + _t137,
				(0.375)*_b[29] + _t138 + _t139 + _t140,
				_t139 + _t141 + _t142 + _t143 + _t144,
				_t147,
				_t134 + _t148,
				_t136 + _t149 + _t151,
				_t138 + _t151 + _t152 + _t155,
				(0.1875)*_b[29] + _t156 + _t158 + _t160 + _t163,
				_t173,
				_t136 + _t148 + _t174,
				_t138 + _t149 + _t175 + _t177,
				(0.0625)*_b[41] + _t141 + _t155 + _t177 + _t180,
				_t192,
				(0.375)*_b[34] + _t138 + _t193 + _t194,
				(0.1875)*_b[34] + _t158 + _t195 + _t197 + _t200,
				_t209,
				_t141 + _t149 + _t193 + _t210 + _t211,
				_t218,
				_t220,
				_b[49],
				_t221 + _t222,
				_t222 + _t223 + _t224,
				(0.375)*_b[50] + _t225 + _t226 + _t227,
				_t232,
				_t221 + _t233,
				_t223 + _t234 + _t236,
				_t225 + _t236 + _t237 + _t240,
				_t249,
				_t223 + _t233 + _t250,
				_t225 + _t234 + _t251 + _t254,
				_t260,
				(0.375)*_b[54] + _t225 + _t261 + _t262,
				_t269,
				_t272,
				_b[64],
				_t273 + _t274,
				_t274 + _t275 + _t276,
				_t279,
				_t273 + _t280,
				_t275 + _t281 + _t284,
				_t290,
				_t275 + _t280 + _t291,
				_t297,
				_t299,
				_b[74],
				_t300 + _t301,
				_t304,
				_t300 + _t305,
				_t307,
				_t309,
				_b[80],
				_t312,
				_t314,
				_b[83],
			},
			{
				_t133,
				_t316,
				_t325,
				_t329,
				_t337,
				_t342,
				_t346,
				(0.3125)*_b[22] + (0.15625)*_b[25] + _t118 + _t121 + _t347 + _t38,
				_t320 + _t348 + _t349 + _t350 + _t59 + _t79,
				(0.03125)*_b[9] + _t131 + _t322 + _t351 + _t352 + _t353 + _t355 + _t89,
				(0.03125)*_b[10] + _t104 + _t334 + _t345 + _t355 + _t356 + _t357 + _t359,
				_t332 + _t350 + _t360 + _t361 + _t362 + _t363,
				(0.3125)*_b[24] + (0.15625)*_b[26] + _t344 + _t347 + _t364 + _t365,
				_t102 + _t366 + _t367 + _t368 + _t56,
				(0.1875)*_b[25] + _t349 + _t353 + _t369 + _t80,
				_t103 + _t368 + _t370 + _t371 + _t374 + _t58,
				(0.1875)*_b[26] + _t335 + _t357 + _t363 + _t369,
				_t368 + _t375 + _t376 + _t377 + _t378,
				(0.375)*_b[25] + _t366 + _t379 + _t79,
				(0.125)*_b[22] + _t371 + _t379 + _t380,
				(0.125)*_b[24] + _t374 + _t378 + _t379,
				(0.375)*_b[26] + _t360 + _t377 + _t379,
				(0.25)*_b[22] + _t381 + _t382,
				_t378 + _t380 + _t382,
				(0.25)*_b[24] + _t382 + _t383,
				_t381 + _t384,
				_t383 + _t384,
				_b[27],
				_t220,
				_t388,
				_t395,
				_t399,
				_t404,
				_t405,
				_t161 + _t174 + _t406 + _t407 + _t408,
				(0.1875)*_b[46] + _t178 + _t386 + _t391 + _t409,
				(0.0625)*_b[36] + _t408 + _t410 + _t413 + _t415,
				(0.1875)*_b[47] + _t396 + _t402 + _t409 + _t416,
				_t408 + _t417 + _t418 + _t419 + _t420,
				(0.375)*_b[46] + _t175 + _t406 + _t421,
				_t194 + _t410 + _t421 + _t422,
				_t413 + _t420 + _t421 + _t423,
				(0.375)*_b[47] + _t400 + _t419 + _t421,
				_t210 + _t424 + _t425,
				_t420 + _t422 + _t425,
				_t425 + _t426 + _t427,
				_t424 + _t428,
				_t427 + _t428,
				_b[48],
				_t272,
				_t430,
				_t436,
				_t438,
				_t442,
				(0.375)*_b[61] + _t238 + _t261 + _t443,
				_t251 + _t433 + _t443 + _t444,
				_t435 + _t441 + _t443 + _t445,
				(0.375)*_b[62] + _t440 + _t443 + _t446,
				_t250 + _t447 + _t448,
				_t441 + _t444 + _t448,
				_t448 + _t449 + _t450,
				_t447 + _t451,
				_t450 + _t451,
				_b[63],
				_t299,
				_t453,
				_t455,
				_t456,
				_t281 + _t457 + _t458,
				_t452 + _t454 + _t458,
				_t458 + _t459 + _t460,
				_t457 + _t461,
				_t460 + _t461,
				_b[73],
				_t309,
				_t462,
				_t464,
				_t305 + _t465,
				_t463 + _t465,
				_b[79],
				_t314,
				_t466,
				_b[82],
				_b[83],
			},
			{
				_t18,
				(0.3125)*_b[4] + (0.15625)*_b[5] + _t106 + _t11 + _t42 + _t467,
				_t468 + _t469 + _t470 + _t57 + _t8,
				(0.375)*_b[5] + _t468 + _t471 + _t5,
				(0.25)*_b[4] + _t472 + _t473,
				_t472 + _t474,
				_b[6],
				_t475,
				_t476 + _t478 + _t480 + _t481 + _t5 + _t86,
				(0.1875)*_b[5] + _t26 + _t478 + _t483 + _t485,
				(0.125)*_b[4] + _t471 + _t487 + _t489,
				_t473 + _t487 + _t490,
				_t474 + _t491,
				_t495,
				(0.03125)*_b[14] + _t16 + _t362 + _t485 + _t493 + _t496 + _t497 + _t63,
				_t375 + _t470 + _t489 + _t498 + _t58 + _t9,
				(0.125)*_b[17] + _t471 + _t490 + _t498,
				(0.25)*_b[17] + _t473 + _t491,
				_t501,
				(0.03125)*_b[19] + _t343 + _t35 + _t359 + _t497 + _t502 + _t503 + _t505,
				(0.1875)*_b[12] + _t335 + _t483 + _t503 + _t506,
				(0.375)*_b[12] + _t360 + _t471 + _t507,
				_t509,
				_t351 + _t360 + _t480 + _t488 + _t506 + _t508,
				_t370 + _t376 + _t470 + _t490 + _t507,
				_t510,
				(0.15625)*_b[12] + (0.3125)*_b[17] + _t318 + _t338 + _t344 + _t467,
				_t346,
				_t147,
				_t137 + _t198 + _t511 + _t512 + _t513,
				(0.375)*_b[32] + _t152 + _t511 + _t514,
				_t143 + _t515 + _t516,
				_t515 + _t517,
				_b[33],
				_t521,
				(0.1875)*_b[32] + _t179 + _t519 + _t523 + _t524,
				_t140 + _t514 + _t526 + _t528,
				_t516 + _t526 + _t529,
				_t517 + _t530,
				_t534,
				(0.0625)*_b[40] + _t513 + _t528 + _t535 + _t536,
				_t400 + _t514 + _t529 + _t535,
				_t418 + _t516 + _t530,
				_t539,
				(0.1875)*_b[38] + _t414 + _t523 + _t537 + _t540,
				(0.375)*_b[38] + _t423 + _t514 + _t541,
				_t543,
				_t389 + _t426 + _t513 + _t529 + _t541,
				_t405,
				_t232,
				(0.375)*_b[52] + _t226 + _t252 + _t544,
				_t224 + _t545 + _t546,
				_t545 + _t547,
				_b[53],
				_t548,
				_t237 + _t544 + _t550 + _t551,
				_t439 + _t546 + _t550,
				_t547 + _t552,
				_t554,
				_t439 + _t445 + _t544 + _t553,
				_t449 + _t546 + _t552,
				_t555,
				(0.375)*_b[57] + _t431 + _t440 + _t544,
				_t442,
				_t279,
				_t282 + _t556 + _t557,
				_t556 + _t558,
				_b[67],
				_t560,
				_t459 + _t557 + _t559,
				_t558 + _t561,
				_t562,
				_t454 + _t557 + _t561,
				_t456,
				_t304,
				_t301 + _t563,
				_b[76],
				_t564,
				_t463 + _t563,
				_t464,
				_t312,
				_b[81],
				_t466,
				_b[83],
			},
			{
				_t346,
				_t342,
				_t337,
				_t329,
				_t325,
				_t316,
				_t133,
				_t510,
				(0.09375)*_b[21] + _t111 + _t125 + _t31 + _t339 + _t340 + _t370 + _t375 + _t48 + _t68 + _t83,
				(0.078125)*_b[10] + _t104 + _t125 + _t328 + _t565 + _t566 + _t567 + _t568 + _t94,
				(0.078125)*_b[9] + _t326 + _t351 + _t569 + _t570 + _t571 + _t572 + _t573,
				(0.09375)*_b[18] + _t103 + _t113 + _t114 + _t127 + _t128 + _t56 + _t570 + _t575 + _t62,
				_t130,
				_t509,
				(0.078125)*_b[19] + _t35 + _t48 + _t499 + _t565 + _t569 + _t576 + _t578,
				(0.15625)*_b[15] + _t352 + _t356 + _t358 + _t496 + _t502 + _t568 + _t578 + _t579 + _t60 + _t61 + _t87 + _t88,
				(0.078125)*_b[20] + _t567 + _t573 + _t575 + _t577 + _t580 + _t86 + _t98,
				_t117,
				_t501,
				(0.078125)*_b[14] + _t362 + _t48 + _t500 + _t566 + _t579 + _t580 + _t581,
				(0.078125)*_b[16] + _t100 + _t568 + _t571 + _t574 + _t576 + _t581 + _t59 + _t95,
				_t101,
				_t495,
				(0.09375)*_b[3] + _t48 + _t50 + _t51 + _t57 + _t574 + _t67 + _t75 + _t9,
				_t77,
				_t475,
				_t53,
				_t18,
				_t405,
				_t404,
				_t399,
				_t395,
				_t388,
				_t220,
				_t543,
				_t215 + _t398 + _t403 + _t538,
				_t154 + _t415 + _t532 + _t542 + _t582 + _t583,
				_t208 + _t217 + _t387 + _t394,
				_t218,
				_t539,
				_t171 + _t176 + _t206 + _t536 + _t582 + _t584,
				_t180 + _t182 + _t412 + _t520 + _t583 + _t584,
				_t209,
				_t534,
				_t165 + _t172 + _t182 + _t191 + _t214 + _t533,
				_t192,
				_t521,
				_t173,
				_t147,
				_t442,
				_t438,
				_t436,
				_t430,
				_t272,
				_t555,
				_t246 + _t258 + _t266 + _t432 + _t445,
				_t239 + _t251 + _t256 + _t266 + _t585,
				_t269,
				_t554,
				_t237 + _t246 + _t253 + _t257 + _t585,
				_t260,
				_t548,
				_t249,
				_t232,
				_t456,
				_t455,
				_t453,
				_t299,
				_t562,
				_t289 + _t296,
				_t297,
				_t560,
				_t290,
				_t279,
				_t464,
				_t462,
				_t309,
				_t564,
				_t307,
				_t304,
				_t466,
				_t314,
				_t312,
				_b[83],
			},
		}};
		#pragma endregion
	}
}
