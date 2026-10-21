set(SOURCES
	CL_m0.cpp
	LB_2p4_1p2.cpp
	subdiv_0_2p1_1p1.cpp
	subdiv_1_2p1_1p1.cpp
	subdiv_2_2p1_1p1.cpp
	subdiv_0_2p4_1p2.cpp
	subdiv_1_2p4_1p2.cpp
	subdiv_2_2p4_1p2.cpp
	SQ_m0.cpp
	P3TriDegCGV.cpp
)

target_sources(miso PRIVATE ${SOURCES})
