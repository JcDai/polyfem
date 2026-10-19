#include "../P3TriDegCGV.hpp"
namespace miso {
	std::array<RealVector<45>, 4> P3TriDegCGV::subdiv_2_2p4_1p2(const RealVector<45> &_b) {
		#pragma region Temporaries
		const Real _t0 = (0.5)*_b[0];
		const Real _t1 = (0.5)*_b[3];
		const Real _t2 = (0.5)*_b[1];
		const Real _t3 = (0.5)*_b[4];
		const Real _t4 = (0.5)*_b[2];
		const Real _t5 = (0.5)*_b[5];
		const Real _t6 = (0.25)*_b[0];
		const Real _t7 = (0.25)*_b[6];
		const Real _t8 = (0.25)*_b[1];
		const Real _t9 = (0.25)*_b[7];
		const Real _t10 = (0.25)*_b[2];
		const Real _t11 = (0.25)*_b[8];
		const Real _t12 = (0.125)*_b[0];
		const Real _t13 = (0.375)*_b[6];
		const Real _t14 = (0.125)*_b[9];
		const Real _t15 = (0.125)*_b[10];
		const Real _t16 = (0.125)*_b[1];
		const Real _t17 = (0.375)*_b[7];
		const Real _t18 = (0.125)*_b[11];
		const Real _t19 = (0.125)*_b[2];
		const Real _t20 = (0.375)*_b[8];
		const Real _t21 = (0.0625)*_b[0];
		const Real _t22 = (0.0625)*_b[12];
		const Real _t23 = (0.25)*_b[3];
		const Real _t24 = (0.25)*_b[9];
		const Real _t25 = _t13 + _t21 + _t22 + _t23 + _t24;
		const Real _t26 = (0.25)*_b[10];
		const Real _t27 = (0.0625)*_b[13];
		const Real _t28 = (0.0625)*_b[1];
		const Real _t29 = (0.25)*_b[4];
		const Real _t30 = _t17 + _t26 + _t27 + _t28 + _t29;
		const Real _t31 = (0.25)*_b[11];
		const Real _t32 = (0.0625)*_b[14];
		const Real _t33 = (0.0625)*_b[2];
		const Real _t34 = (0.25)*_b[5];
		const Real _t35 = _t20 + _t31 + _t32 + _t33 + _t34;
		const Real _t36 = (0.5)*_b[15];
		const Real _t37 = (0.5)*_b[16];
		const Real _t38 = (0.5)*_b[17];
		const Real _t39 = (0.25)*_b[15];
		const Real _t40 = (0.25)*_b[18];
		const Real _t41 = _t23 + _t40;
		const Real _t42 = (0.25)*_b[16];
		const Real _t43 = (0.25)*_b[19];
		const Real _t44 = _t29 + _t43;
		const Real _t45 = (0.25)*_b[17];
		const Real _t46 = (0.25)*_b[20];
		const Real _t47 = _t34 + _t46;
		const Real _t48 = (0.125)*_b[6];
		const Real _t49 = (0.125)*_b[15];
		const Real _t50 = (0.125)*_b[21];
		const Real _t51 = _t49 + _t50;
		const Real _t52 = (0.125)*_b[7];
		const Real _t53 = (0.125)*_b[16];
		const Real _t54 = (0.125)*_b[22];
		const Real _t55 = _t53 + _t54;
		const Real _t56 = (0.125)*_b[8];
		const Real _t57 = (0.125)*_b[17];
		const Real _t58 = (0.125)*_b[23];
		const Real _t59 = _t57 + _t58;
		const Real _t60 = (0.1875)*_b[18];
		const Real _t61 = _t21 + _t60;
		const Real _t62 = (0.0625)*_b[24];
		const Real _t63 = (0.1875)*_b[21];
		const Real _t64 = (0.0625)*_b[9] + _t63;
		const Real _t65 = _t62 + _t64;
		const Real _t66 = (0.0625)*_b[15];
		const Real _t67 = (0.1875)*_b[6] + _t66;
		const Real _t68 = (0.1875)*_b[3] + _t61 + _t65 + _t67;
		const Real _t69 = (0.1875)*_b[19];
		const Real _t70 = _t28 + _t69;
		const Real _t71 = (0.0625)*_b[25];
		const Real _t72 = (0.1875)*_b[22];
		const Real _t73 = (0.0625)*_b[10] + _t72;
		const Real _t74 = _t71 + _t73;
		const Real _t75 = (0.0625)*_b[16];
		const Real _t76 = (0.1875)*_b[7] + _t75;
		const Real _t77 = (0.1875)*_b[4] + _t70 + _t74 + _t76;
		const Real _t78 = (0.1875)*_b[20];
		const Real _t79 = _t33 + _t78;
		const Real _t80 = (0.0625)*_b[26];
		const Real _t81 = (0.1875)*_b[23];
		const Real _t82 = (0.0625)*_b[11] + _t81;
		const Real _t83 = _t80 + _t82;
		const Real _t84 = (0.0625)*_b[17];
		const Real _t85 = (0.1875)*_b[8] + _t84;
		const Real _t86 = (0.1875)*_b[5] + _t79 + _t83 + _t85;
		const Real _t87 = (0.25)*_b[27];
		const Real _t88 = (0.25)*_b[28];
		const Real _t89 = (0.25)*_b[29];
		const Real _t90 = (0.125)*_b[27];
		const Real _t91 = (0.125)*_b[3];
		const Real _t92 = (0.125)*_b[30];
		const Real _t93 = _t40 + _t91 + _t92;
		const Real _t94 = (0.125)*_b[28];
		const Real _t95 = (0.125)*_b[4];
		const Real _t96 = (0.125)*_b[31];
		const Real _t97 = _t43 + _t95 + _t96;
		const Real _t98 = (0.125)*_b[29];
		const Real _t99 = (0.125)*_b[5];
		const Real _t100 = (0.125)*_b[32];
		const Real _t101 = _t100 + _t46 + _t99;
		const Real _t102 = (0.0625)*_b[33];
		const Real _t103 = (0.0625)*_b[27];
		const Real _t104 = (0.0625)*_b[6];
		const Real _t105 = _t103 + _t104;
		const Real _t106 = _t102 + _t105;
		const Real _t107 = _t106 + _t21 + _t51 + _t93;
		const Real _t108 = (0.0625)*_b[34];
		const Real _t109 = (0.0625)*_b[28];
		const Real _t110 = (0.0625)*_b[7];
		const Real _t111 = _t109 + _t110;
		const Real _t112 = _t108 + _t111;
		const Real _t113 = _t112 + _t28 + _t55 + _t97;
		const Real _t114 = (0.0625)*_b[35];
		const Real _t115 = (0.0625)*_b[29];
		const Real _t116 = (0.0625)*_b[8];
		const Real _t117 = _t115 + _t116;
		const Real _t118 = _t114 + _t117;
		const Real _t119 = _t101 + _t118 + _t33 + _t59;
		const Real _t120 = (0.375)*_b[27];
		const Real _t121 = (0.125)*_b[36];
		const Real _t122 = (0.375)*_b[28];
		const Real _t123 = (0.125)*_b[37];
		const Real _t124 = (0.375)*_b[29];
		const Real _t125 = (0.125)*_b[38];
		const Real _t126 = (0.0625)*_b[39];
		const Real _t127 = (0.1875)*_b[30];
		const Real _t128 = (0.0625)*_b[36] + _t127;
		const Real _t129 = _t126 + _t128;
		const Real _t130 = (0.0625)*_b[3];
		const Real _t131 = (0.1875)*_b[27] + _t130;
		const Real _t132 = (0.1875)*_b[15] + _t129 + _t131 + _t61;
		const Real _t133 = (0.0625)*_b[40];
		const Real _t134 = (0.1875)*_b[31];
		const Real _t135 = (0.0625)*_b[37] + _t134;
		const Real _t136 = _t133 + _t135;
		const Real _t137 = (0.0625)*_b[4];
		const Real _t138 = (0.1875)*_b[28] + _t137;
		const Real _t139 = (0.1875)*_b[16] + _t136 + _t138 + _t70;
		const Real _t140 = (0.0625)*_b[41];
		const Real _t141 = (0.1875)*_b[32];
		const Real _t142 = (0.0625)*_b[38] + _t141;
		const Real _t143 = _t140 + _t142;
		const Real _t144 = (0.0625)*_b[5];
		const Real _t145 = (0.1875)*_b[29] + _t144;
		const Real _t146 = (0.1875)*_b[17] + _t143 + _t145 + _t79;
		const Real _t147 = (0.25)*_b[36];
		const Real _t148 = (0.0625)*_b[42];
		const Real _t149 = _t120 + _t147 + _t148 + _t21 + _t39;
		const Real _t150 = (0.25)*_b[37];
		const Real _t151 = (0.0625)*_b[43];
		const Real _t152 = _t122 + _t150 + _t151 + _t28 + _t42;
		const Real _t153 = (0.25)*_b[38];
		const Real _t154 = (0.0625)*_b[44];
		const Real _t155 = _t124 + _t153 + _t154 + _t33 + _t45;
		const Real _t156 = _t60 + _t66;
		const Real _t157 = (0.1875)*_b[36] + _t126 + _t127 + _t131 + _t148 + _t156;
		const Real _t158 = _t69 + _t75;
		const Real _t159 = (0.1875)*_b[37] + _t133 + _t134 + _t138 + _t151 + _t158;
		const Real _t160 = _t78 + _t84;
		const Real _t161 = (0.1875)*_b[38] + _t140 + _t141 + _t145 + _t154 + _t160;
		const Real _t162 = (0.125)*_b[39];
		const Real _t163 = (0.125)*_b[18];
		const Real _t164 = _t162 + _t163;
		const Real _t165 = (0.25)*_b[30];
		const Real _t166 = _t121 + _t165 + _t50;
		const Real _t167 = _t106 + _t148 + _t164 + _t166;
		const Real _t168 = (0.125)*_b[40];
		const Real _t169 = (0.125)*_b[19];
		const Real _t170 = _t168 + _t169;
		const Real _t171 = (0.25)*_b[31];
		const Real _t172 = _t123 + _t171 + _t54;
		const Real _t173 = _t112 + _t151 + _t170 + _t172;
		const Real _t174 = (0.125)*_b[41];
		const Real _t175 = (0.125)*_b[20];
		const Real _t176 = _t174 + _t175;
		const Real _t177 = (0.25)*_b[32];
		const Real _t178 = _t125 + _t177 + _t58;
		const Real _t179 = _t118 + _t154 + _t176 + _t178;
		const Real _t180 = (0.1875)*_b[33];
		const Real _t181 = (0.1875)*_b[39] + _t128 + _t148 + _t180 + _t65;
		const Real _t182 = (0.1875)*_b[34];
		const Real _t183 = (0.1875)*_b[40] + _t135 + _t151 + _t182 + _t74;
		const Real _t184 = (0.1875)*_b[35];
		const Real _t185 = (0.1875)*_b[41] + _t142 + _t154 + _t184 + _t83;
		const Real _t186 = (0.25)*_b[24];
		const Real _t187 = (0.375)*_b[33];
		const Real _t188 = (0.25)*_b[39];
		const Real _t189 = _t148 + _t186 + _t187 + _t188 + _t22;
		const Real _t190 = (0.25)*_b[25];
		const Real _t191 = (0.375)*_b[34];
		const Real _t192 = (0.25)*_b[40];
		const Real _t193 = _t151 + _t190 + _t191 + _t192 + _t27;
		const Real _t194 = (0.25)*_b[26];
		const Real _t195 = (0.375)*_b[35];
		const Real _t196 = (0.25)*_b[41];
		const Real _t197 = _t154 + _t194 + _t195 + _t196 + _t32;
		const Real _t198 = (0.125)*_b[42];
		const Real _t199 = (0.125)*_b[43];
		const Real _t200 = (0.125)*_b[44];
		const Real _t201 = _t147 + _t165;
		const Real _t202 = _t150 + _t171;
		const Real _t203 = _t153 + _t177;
		const Real _t204 = (0.125)*_b[33];
		const Real _t205 = (0.125)*_b[34];
		const Real _t206 = (0.125)*_b[35];
		const Real _t207 = (0.125)*_b[24];
		const Real _t208 = (0.125)*_b[25];
		const Real _t209 = (0.125)*_b[26];
		const Real _t210 = (0.5)*_b[36];
		const Real _t211 = (0.25)*_b[42];
		const Real _t212 = (0.5)*_b[37];
		const Real _t213 = (0.25)*_b[43];
		const Real _t214 = (0.5)*_b[38];
		const Real _t215 = (0.25)*_b[44];
		const Real _t216 = (0.25)*_b[33];
		const Real _t217 = (0.5)*_b[39];
		const Real _t218 = (0.25)*_b[34];
		const Real _t219 = (0.5)*_b[40];
		const Real _t220 = (0.25)*_b[35];
		const Real _t221 = (0.5)*_b[41];
		const Real _t222 = (0.5)*_b[42];
		const Real _t223 = (0.5)*_b[43];
		const Real _t224 = (0.5)*_b[44];
		const Real _t225 = (0.125)*_b[12];
		const Real _t226 = (0.125)*_b[13];
		const Real _t227 = (0.125)*_b[14];
		const Real _t228 = (0.25)*_b[12];
		const Real _t229 = (0.5)*_b[9];
		const Real _t230 = (0.5)*_b[10];
		const Real _t231 = (0.25)*_b[13];
		const Real _t232 = (0.5)*_b[11];
		const Real _t233 = (0.25)*_b[14];
		const Real _t234 = (0.5)*_b[12];
		const Real _t235 = (0.5)*_b[13];
		const Real _t236 = (0.5)*_b[14];
		const Real _t237 = (0.1875)*_b[9] + _t130 + _t22 + _t60 + _t62 + _t63 + _t67;
		const Real _t238 = (0.1875)*_b[10] + _t137 + _t27 + _t69 + _t71 + _t72 + _t76;
		const Real _t239 = (0.1875)*_b[11] + _t144 + _t32 + _t78 + _t80 + _t81 + _t85;
		const Real _t240 = (0.25)*_b[21];
		const Real _t241 = _t24 + _t240;
		const Real _t242 = _t163 + _t207;
		const Real _t243 = (0.25)*_b[22];
		const Real _t244 = _t243 + _t26;
		const Real _t245 = _t169 + _t208;
		const Real _t246 = (0.25)*_b[23];
		const Real _t247 = _t246 + _t31;
		const Real _t248 = _t175 + _t209;
		const Real _t249 = (0.5)*_b[24];
		const Real _t250 = (0.5)*_b[25];
		const Real _t251 = (0.5)*_b[26];
		const Real _t252 = _t14 + _t240 + _t92;
		const Real _t253 = _t106 + _t22 + _t242 + _t252;
		const Real _t254 = _t15 + _t243 + _t96;
		const Real _t255 = _t112 + _t245 + _t254 + _t27;
		const Real _t256 = _t100 + _t18 + _t246;
		const Real _t257 = _t118 + _t248 + _t256 + _t32;
		const Real _t258 = (0.1875)*_b[24] + _t129 + _t180 + _t22 + _t64;
		const Real _t259 = (0.1875)*_b[25] + _t136 + _t182 + _t27 + _t73;
		const Real _t260 = (0.1875)*_b[26] + _t143 + _t184 + _t32 + _t82;
		const Real _t261 = _t102 + _t130 + _t156;
		const Real _t262 = _t108 + _t137 + _t158;
		const Real _t263 = _t114 + _t144 + _t160;
		#pragma endregion
		#pragma region Expressions
		return {{
			{
				_b[0],
				_b[1],
				_b[2],
				_t0 + _t1,
				_t2 + _t3,
				_t4 + _t5,
				_t1 + _t6 + _t7,
				_t3 + _t8 + _t9,
				_t10 + _t11 + _t5,
				(0.375)*_b[3] + _t12 + _t13 + _t14,
				(0.375)*_b[4] + _t15 + _t16 + _t17,
				(0.375)*_b[5] + _t18 + _t19 + _t20,
				_t25,
				_t30,
				_t35,
				_t0 + _t36,
				_t2 + _t37,
				_t38 + _t4,
				_t39 + _t41 + _t6,
				_t42 + _t44 + _t8,
				_t10 + _t45 + _t47,
				_t12 + _t41 + _t48 + _t51,
				_t16 + _t44 + _t52 + _t55,
				_t19 + _t47 + _t56 + _t59,
				_t68,
				_t77,
				_t86,
				_t36 + _t6 + _t87,
				_t37 + _t8 + _t88,
				_t10 + _t38 + _t89,
				_t12 + _t39 + _t90 + _t93,
				_t16 + _t42 + _t94 + _t97,
				_t101 + _t19 + _t45 + _t98,
				_t107,
				_t113,
				_t119,
				(0.375)*_b[15] + _t12 + _t120 + _t121,
				(0.375)*_b[16] + _t122 + _t123 + _t16,
				(0.375)*_b[17] + _t124 + _t125 + _t19,
				_t132,
				_t139,
				_t146,
				_t149,
				_t152,
				_t155,
			},
			{
				_t149,
				_t152,
				_t155,
				_t157,
				_t159,
				_t161,
				_t167,
				_t173,
				_t179,
				_t181,
				_t183,
				_t185,
				_t189,
				_t193,
				_t197,
				(0.375)*_b[36] + _t120 + _t198 + _t49,
				(0.375)*_b[37] + _t122 + _t199 + _t53,
				(0.375)*_b[38] + _t124 + _t200 + _t57,
				_t164 + _t198 + _t201 + _t90,
				_t170 + _t199 + _t202 + _t94,
				_t176 + _t200 + _t203 + _t98,
				_t166 + _t188 + _t198 + _t204,
				_t172 + _t192 + _t199 + _t205,
				_t178 + _t196 + _t200 + _t206,
				(0.375)*_b[39] + _t187 + _t198 + _t207,
				(0.375)*_b[40] + _t191 + _t199 + _t208,
				(0.375)*_b[41] + _t195 + _t200 + _t209,
				_t210 + _t211 + _t87,
				_t212 + _t213 + _t88,
				_t214 + _t215 + _t89,
				_t188 + _t201 + _t211,
				_t192 + _t202 + _t213,
				_t196 + _t203 + _t215,
				_t211 + _t216 + _t217,
				_t213 + _t218 + _t219,
				_t215 + _t220 + _t221,
				_t210 + _t222,
				_t212 + _t223,
				_t214 + _t224,
				_t217 + _t222,
				_t219 + _t223,
				_t221 + _t224,
				_b[42],
				_b[43],
				_b[44],
			},
			{
				_t25,
				_t30,
				_t35,
				(0.375)*_b[9] + _t13 + _t225 + _t91,
				(0.375)*_b[10] + _t17 + _t226 + _t95,
				(0.375)*_b[11] + _t20 + _t227 + _t99,
				_t228 + _t229 + _t7,
				_t230 + _t231 + _t9,
				_t11 + _t232 + _t233,
				_t229 + _t234,
				_t230 + _t235,
				_t232 + _t236,
				_b[12],
				_b[13],
				_b[14],
				_t237,
				_t238,
				_t239,
				_t225 + _t241 + _t242 + _t48,
				_t226 + _t244 + _t245 + _t52,
				_t227 + _t247 + _t248 + _t56,
				_t186 + _t228 + _t241,
				_t190 + _t231 + _t244,
				_t194 + _t233 + _t247,
				_t234 + _t249,
				_t235 + _t250,
				_t236 + _t251,
				_t253,
				_t255,
				_t257,
				_t186 + _t204 + _t225 + _t252,
				_t190 + _t205 + _t226 + _t254,
				_t194 + _t206 + _t227 + _t256,
				_t216 + _t228 + _t249,
				_t218 + _t231 + _t250,
				_t220 + _t233 + _t251,
				_t258,
				_t259,
				_t260,
				(0.375)*_b[24] + _t162 + _t187 + _t225,
				(0.375)*_b[25] + _t168 + _t191 + _t226,
				(0.375)*_b[26] + _t174 + _t195 + _t227,
				_t189,
				_t193,
				_t197,
			},
			{
				_t189,
				_t193,
				_t197,
				_t181,
				_t183,
				_t185,
				_t167,
				_t173,
				_t179,
				_t157,
				_t159,
				_t161,
				_t149,
				_t152,
				_t155,
				_t258,
				_t259,
				_t260,
				_t105 + _t129 + _t163 + _t204 + _t65,
				_t111 + _t136 + _t169 + _t205 + _t74,
				_t117 + _t143 + _t175 + _t206 + _t83,
				_t104 + _t129 + _t261 + _t50 + _t90,
				_t110 + _t136 + _t262 + _t54 + _t94,
				_t116 + _t143 + _t263 + _t58 + _t98,
				_t132,
				_t139,
				_t146,
				_t253,
				_t255,
				_t257,
				_t103 + _t261 + _t48 + _t65 + _t92,
				_t109 + _t262 + _t52 + _t74 + _t96,
				_t100 + _t115 + _t263 + _t56 + _t83,
				_t107,
				_t113,
				_t119,
				_t237,
				_t238,
				_t239,
				_t68,
				_t77,
				_t86,
				_t25,
				_t30,
				_t35,
			},
		}};
		#pragma endregion
	}
}
