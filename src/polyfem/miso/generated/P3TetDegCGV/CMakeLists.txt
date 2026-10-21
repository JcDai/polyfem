set(SOURCES
	CL_m0.cpp
	LB_3p6_1p3.cpp
	subdiv_0_3p1_1p1.cpp
	subdiv_1_3p1_1p1.cpp
	subdiv_2_3p1_1p1.cpp
	subdiv_3_3p1_1p1.cpp
	subdiv_4_3p1_1p1.cpp
	subdiv_5_3p1_1p1.cpp
	subdiv_6_3p1_1p1.cpp
	subdiv_0_3p6_1p3.cpp
	subdiv_1_3p6_1p3.cpp
	subdiv_2_3p6_1p3.cpp
	subdiv_3_3p6_1p3.cpp
	subdiv_4_3p6_1p3.cpp
	subdiv_5_3p6_1p3.cpp
	subdiv_6_3p6_1p3.cpp
	SQ_m0.cpp
	P3TetDegCGV.cpp
)

target_sources(miso PRIVATE ${SOURCES})
