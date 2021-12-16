

loadgraph.cpp: kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMEDGECHUNKSINABUFFER].data[0].key = globalparams.ACTSPARAMS_NUMEDGECHUNKSINABUFFER;
actsproc.cpp: if(edgebankID == 0){ copystats(edges0, kvdram, _globalparamsE, globalparamsK); }
Makefile: KERNEL_TOP_SYNC += $(RELREF)acts/acts/actsapply.cpp
acts.template
	// #ifndef FPGA_IMPL
	// #include "actsproc.h"
	// #include "actssync.h"
	// #endif 
actscommon.cpp: 	#define KVSOURCEDRAMSZ (1 << 26) // max HBM capacity (512MB) // CRITICAL REMOVEME.
loadgraph.cpp: 	// if(((((unsigned long)globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE*VECTOR_SIZE) 
goclkernel.cpp: unsigned int A = edges[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].data[0].key;

1 WORKER: 1by2by1by5_5and1 (NUM_EDGE_BANKS=5)
2 WORKERS: 1by2by1by2_4and1 (NUM_EDGE_BANKS=2)
4 WORKERS: 1by2by1by1_4and1 (NUM_EDGE_BANKS=1)