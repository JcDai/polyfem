set(SOURCES
	CL_m0.cpp
	LB_3p6.cpp
	subdiv_0_3p1.cpp
	subdiv_1_3p1.cpp
	subdiv_2_3p1.cpp
	subdiv_3_3p1.cpp
	subdiv_4_3p1.cpp
	subdiv_0_3p6.cpp
	subdiv_1_3p6.cpp
	subdiv_2_3p6.cpp
	subdiv_3_3p6.cpp
	subdiv_4_3p6.cpp
	SQ_m0.cpp
	P3TetDegVal.cpp
)

target_sources(miso PRIVATE ${SOURCES})
