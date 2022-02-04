#include "processedges_splitdstvxs.h"
using namespace std;

// => VDATA (to read)
// 16:[0,32,...,480] | [1,33,...,481] | ... | [31,63,...,511]

// => VMASK (to read)
// 32:[0,32,...,480,...,992] | [1,33,...,481,...,993] | ... | [31,63,...,511,...,1023]

// int programglobalvar_numvtxsprocessed = 0; // program global variables

#define VSETSZ 32
#define VMSETSZ 32
#define PE_SETSZ 16

#ifdef SW
processedges_splitdstvxs::processedges_splitdstvxs(mydebug * _mydebugobj){ 
	actsutilityobj = new actsutility(); 
	acts_utilobj = new acts_util(_mydebugobj);
	mydebugobj = _mydebugobj;
}
processedges_splitdstvxs::~processedges_splitdstvxs(){}
#endif

//
value_t acts_all::PROCESS_SPL_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo){
	value_t res = 0;
	// if(GraphAlgo == PAGERANK){
		// res = udata;
	// } else if(GraphAlgo == BFS){
		// res = NAp;
	// } else if(GraphAlgo == SSSP){
		// res = udata + edgew;
	// } else {
		// res = NAp;
	// }
	
	res = udata + edgew;
	return res;
}

parsededge_t acts_all::PARSE_EDGE(uint32_type data){ 
	parsededge_t parsededge;
	#ifdef _WIDEWORD
	parsededge.incr = data.range(31, 28);
	parsededge.dstvid = data.range(28, 0);
	#else
	parsededge.incr = UTIL_READFROM_UINT(data, 28, 4);
	parsededge.dstvid = UTIL_READFROM_UINT(data, 0, 28);
	#endif
	return parsededge; 
}

void acts_all::PROCESS_SPL_debug(unsigned int debugid,
	unsigned int i, value_t E[VECTOR2_SIZE], bool_type ens[VECTOR2_SIZE], unsigned int mask[VECTOR2_SIZE],
		value_t udataset[MAX_NUM_UNIQ_EDGES_PER_VEC], value_t maskset[VECTOR2_SIZE], value_t Vset[MAX_NUM_UNIQ_EDGES_PER_VEC], unit1_type VMset[MAX_NUM_UNIQ_EDGES_PER_VEC], vertex_t lvids[VECTOR2_SIZE],
			unsigned int incr[VECTOR2_SIZE], unsigned int lsrcvids[VECTOR2_SIZE], unsigned int ldstvids[VECTOR2_SIZE], value_t res[VECTOR2_SIZE], keyvalue_t mykeyvalue[VECTOR2_SIZE], sweepparams_t sweepparams, globalparams_t globalparams,
				unsigned int lvid_head, unsigned int srcvid_head, travstate_t travstate, unsigned int chunk_size, sliceinfos_t sliceinfos, unsigned int * activeloadcount, unsigned int * inactiveloadcount, unsigned int * debug_numinvalidheads
				){
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	unsigned int validbound = reducebuffersz * FETFACTOR * VECTOR2_SIZE;
	
	if(debugid == 0){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[0]: "<<E[0]<<endl;
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[1]: "<<E[1]<<endl;
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[2]: "<<E[2]<<endl;
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[3]: "<<E[3]<<endl;
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[4]: "<<E[4]<<endl;
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[5]: "<<E[5]<<endl;
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[6]: "<<E[6]<<endl;
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[7]: "<<E[7]<<endl;
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[8]: "<<E[8]<<endl;
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[9]: "<<E[9]<<endl;
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[10]: "<<E[10]<<endl;
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[11]: "<<E[11]<<endl;
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[12]: "<<E[12]<<endl;
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[13]: "<<E[13]<<endl;
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[14]: "<<E[14]<<endl;
		cout<<"readandprocess(1-2)::DEBUG CODE 0:: E[15]: "<<E[15]<<endl;
		#endif
	}
	
	if(debugid == 1){
		#ifdef _DEBUGMODE_CHECKS
		if(srcvid_head < travstate.i2){ cout<<"readandprocess(12)::DEBUG CODE 1:: INVALID srcvid_head. this is an error. i: "<<i<<"(of "<<chunk_size<<"), srcvid_head: "<<srcvid_head<<", travstate.i2: "<<travstate.i2<<" offset_kvs: "<<offset_kvs<<". exiting..."<<endl;					 
			for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"readandprocess(12): E["<<v<<"]: "<<E[v]<<endl; }
			exit(EXIT_FAILURE); }
		if(lvid_head >= (reducebuffersz*FETFACTOR*VECTOR2_SIZE)){ cout<<"readandprocess(12)::DEBUG CODE 1:: INVALID srcvid_head. this is an error. i: "<<i<<"(of "<<chunk_size<<"), lvid_head: "<<lvid_head<<", reducebuffersz*FETFACTOR*VECTOR2_SIZE: "<<reducebuffersz*FETFACTOR*VECTOR2_SIZE<<". exiting..."<<endl;					 
			for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"readandprocess(12)::DEBUG CODE 1:: E["<<v<<"]: "<<E[v]<<endl; }
			exit(EXIT_FAILURE); }
		actsutilityobj->checkoutofbounds("readandprocess(12)::DEBUG CODE 1::1", lvid_head, reducebuffersz * FETFACTOR * VECTOR2_SIZE, srcvid_head, travstate.i2, i);
		#endif
	}
	
	if(debugid == 2){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"readandprocess(2)::DEBUG CODE 2:: lvid_head: "<<lvid_head<<", lvid: "<<lvid_head + 0<<", udataset[0]: "<<udataset[0]<<", maskset[0]: "<<maskset[0]<<endl;
		cout<<"readandprocess(2)::DEBUG CODE 2:: lvid_head: "<<lvid_head<<", lvid: "<<lvid_head + 1<<", udataset[1]: "<<udataset[1]<<", maskset[1]: "<<maskset[1]<<endl;
		cout<<"readandprocess(2)::DEBUG CODE 2:: lvid_head: "<<lvid_head<<", lvid: "<<lvid_head + 2<<", udataset[2]: "<<udataset[2]<<", maskset[2]: "<<maskset[2]<<endl;
		cout<<"readandprocess(2)::DEBUG CODE 2:: lvid_head: "<<lvid_head<<", lvid: "<<lvid_head + 3<<", udataset[3]: "<<udataset[3]<<", maskset[3]: "<<maskset[3]<<endl;
		cout<<"readandprocess(2)::DEBUG CODE 2:: lvid_head: "<<lvid_head<<", lvid: "<<lvid_head + 4<<", udataset[4]: "<<udataset[4]<<", maskset[4]: "<<maskset[4]<<endl;
		cout<<"readandprocess(2)::DEBUG CODE 2:: lvid_head: "<<lvid_head<<", lvid: "<<lvid_head + 5<<", udataset[5]: "<<udataset[5]<<", maskset[5]: "<<maskset[5]<<endl;
		cout<<"readandprocess(2)::DEBUG CODE 2:: lvid_head: "<<lvid_head<<", lvid: "<<lvid_head + 6<<", udataset[6]: "<<udataset[6]<<", maskset[6]: "<<maskset[6]<<endl;
		cout<<"readandprocess(2)::DEBUG CODE 2:: lvid_head: "<<lvid_head<<", lvid: "<<lvid_head + 7<<", udataset[7]: "<<udataset[7]<<", maskset[7]: "<<maskset[7]<<endl;
		#endif
		#ifdef _DEBUGMODE_CHECKS
		if(maskset[0] > 2){ cout<<"ERROR @ readandprocess(13)::DEBUG CODE 2::.maskset[0].1. maskset[0]: "<<maskset[0]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: maskset["<<n<<"]: "<<maskset[n]<<", lvid + 0: "<<lvid + 0<<endl; } exit(EXIT_FAILURE); }
		if(maskset[1] > 2){ cout<<"ERROR @ readandprocess(13)::DEBUG CODE 2::.maskset[1].1. maskset[1]: "<<maskset[1]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: maskset["<<n<<"]: "<<maskset[n]<<", lvid + 1: "<<lvid + 1<<endl; } exit(EXIT_FAILURE); }
		if(maskset[2] > 2){ cout<<"ERROR @ readandprocess(13)::DEBUG CODE 2::.maskset[2].1. maskset[2]: "<<maskset[2]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: maskset["<<n<<"]: "<<maskset[n]<<", lvid + 2: "<<lvid + 2<<endl; } exit(EXIT_FAILURE); }
		if(maskset[3] > 2){ cout<<"ERROR @ readandprocess(13)::DEBUG CODE 2::.maskset[3].1. maskset[3]: "<<maskset[3]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: maskset["<<n<<"]: "<<maskset[n]<<", lvid + 3: "<<lvid + 3<<endl; } exit(EXIT_FAILURE); }
		if(maskset[4] > 2){ cout<<"ERROR @ readandprocess(13)::DEBUG CODE 2::.maskset[4].1. maskset[4]: "<<maskset[4]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: maskset["<<n<<"]: "<<maskset[n]<<", lvid + 4: "<<lvid + 4<<endl; } exit(EXIT_FAILURE); }
		if(maskset[5] > 2){ cout<<"ERROR @ readandprocess(13)::DEBUG CODE 2::.maskset[5].1. maskset[5]: "<<maskset[5]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: maskset["<<n<<"]: "<<maskset[n]<<", lvid + 5: "<<lvid + 5<<endl; } exit(EXIT_FAILURE); }
		if(maskset[6] > 2){ cout<<"ERROR @ readandprocess(13)::DEBUG CODE 2::.maskset[6].1. maskset[6]: "<<maskset[6]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: maskset["<<n<<"]: "<<maskset[n]<<", lvid + 6: "<<lvid + 6<<endl; } exit(EXIT_FAILURE); }
		if(maskset[7] > 2){ cout<<"ERROR @ readandprocess(13)::DEBUG CODE 2::.maskset[7].1. maskset[7]: "<<maskset[7]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: maskset["<<n<<"]: "<<maskset[n]<<", lvid + 7: "<<lvid + 7<<endl; } exit(EXIT_FAILURE); }
		#endif
	}
	
	if(debugid == 3){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", incr[0]: "<<incr[0]<<", lsrcvids[0]: "<<lsrcvids[0]<<", ens[0]: "<<ens[0]<<endl;
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", incr[1]: "<<incr[1]<<", lsrcvids[1]: "<<lsrcvids[1]<<", ens[1]: "<<ens[1]<<endl;
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", incr[2]: "<<incr[2]<<", lsrcvids[2]: "<<lsrcvids[2]<<", ens[2]: "<<ens[2]<<endl;
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", incr[3]: "<<incr[3]<<", lsrcvids[3]: "<<lsrcvids[3]<<", ens[3]: "<<ens[3]<<endl;
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", incr[4]: "<<incr[4]<<", lsrcvids[4]: "<<lsrcvids[4]<<", ens[4]: "<<ens[4]<<endl;
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", incr[5]: "<<incr[5]<<", lsrcvids[5]: "<<lsrcvids[5]<<", ens[5]: "<<ens[5]<<endl;
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", incr[6]: "<<incr[6]<<", lsrcvids[6]: "<<lsrcvids[6]<<", ens[6]: "<<ens[6]<<endl;
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", incr[7]: "<<incr[7]<<", lsrcvids[7]: "<<lsrcvids[7]<<", ens[7]: "<<ens[7]<<endl;
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", incr[8]: "<<incr[8]<<", lsrcvids[8]: "<<lsrcvids[8]<<", ens[8]: "<<ens[8]<<endl;
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", incr[9]: "<<incr[9]<<", lsrcvids[9]: "<<lsrcvids[9]<<", ens[9]: "<<ens[9]<<endl;
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", incr[10]: "<<incr[10]<<", lsrcvids[10]: "<<lsrcvids[10]<<", ens[10]: "<<ens[10]<<endl;
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", incr[11]: "<<incr[11]<<", lsrcvids[11]: "<<lsrcvids[11]<<", ens[11]: "<<ens[11]<<endl;
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", incr[12]: "<<incr[12]<<", lsrcvids[12]: "<<lsrcvids[12]<<", ens[12]: "<<ens[12]<<endl;
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", incr[13]: "<<incr[13]<<", lsrcvids[13]: "<<lsrcvids[13]<<", ens[13]: "<<ens[13]<<endl;
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", incr[14]: "<<incr[14]<<", lsrcvids[14]: "<<lsrcvids[14]<<", ens[14]: "<<ens[14]<<endl;
		cout<<"readandprocess(3)::DEBUG CODE 3:: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", incr[15]: "<<incr[15]<<", lsrcvids[15]: "<<lsrcvids[15]<<", ens[15]: "<<ens[15]<<endl;
		#endif
		#ifdef _DEBUGMODE_CHECKS2
		if(ens[0] == ON && incr[0] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[0]("<<incr[0]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[1] == ON && incr[1] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[1]("<<incr[1]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[2] == ON && incr[2] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[2]("<<incr[2]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[3] == ON && incr[3] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[3]("<<incr[3]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[4] == ON && incr[4] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[4]("<<incr[4]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[5] == ON && incr[5] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[5]("<<incr[5]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[6] == ON && incr[6] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[6]("<<incr[6]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[7] == ON && incr[7] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[7]("<<incr[7]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[8] == ON && incr[8] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[8]("<<incr[8]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[9] == ON && incr[9] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[9]("<<incr[9]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[10] == ON && incr[10] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[10]("<<incr[10]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[11] == ON && incr[11] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[11]("<<incr[11]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[12] == ON && incr[12] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[12]("<<incr[12]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[13] == ON && incr[13] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[13]("<<incr[13]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[14] == ON && incr[14] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[14]("<<incr[14]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[15] == ON && incr[15] >= MAX_NUM_UNIQ_EDGES_PER_VEC){ 
			cout<<"readandprocess(12)::DEBUG CODE 3:: incr[15]("<<incr[15]<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<endl;
 cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(12)::DEBUG CODE 3::mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			exit(EXIT_FAILURE); 
		}
		#endif
		#ifdef _DEBUGMODE_CHECKS2
		if(lsrcvids[0] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 0, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 0, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		if(lsrcvids[1] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 1, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 1, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		if(lsrcvids[2] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 2, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 2, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		if(lsrcvids[3] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 3, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 3, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		if(lsrcvids[4] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 4, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 4, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		if(lsrcvids[5] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 5, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 5, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		if(lsrcvids[6] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 6, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 6, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		if(lsrcvids[7] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 7, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 7, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		if(lsrcvids[8] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 8, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 8, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		if(lsrcvids[9] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 9, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 9, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		if(lsrcvids[10] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 10, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 10, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		if(lsrcvids[11] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 11, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 11, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		if(lsrcvids[12] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 12, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 12, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		if(lsrcvids[13] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 13, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 13, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		if(lsrcvids[14] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 14, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 14, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		if(lsrcvids[15] >= validbound){
			if(*debug_numinvalidheads > 32){
 cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<endl;  cout<<">>> readandprocess(17)::DEBUG CODE 3:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<endl;  
			cout<<"readandprocess::::DEBUG CODE 3::ERROR(17): "<<*debug_numinvalidheads<<" srcv heads found (4 was limit set). i: "<<i<<" (of "<<chunk_size<<"), v: 15, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<". EXITING..."<<endl; exit(EXIT_FAILURE); } 
			else {
			// cout<<"readandprocess:: i: "<<i<<" (of "<<chunk_size<<"), v: 15, validbound: "<<validbound<<", *debug_numinvalidheads: "<<*debug_numinvalidheads<<endl;
			*debug_numinvalidheads += 1; }}
		#endif 
	}
	
	if(debugid == 4){
		#ifdef _DEBUGMODE_CHECKS2	
		//  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
		// for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
		// cout<<"SUCCESS? @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
		// cout<<"SUCCESS? @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
		
		// for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			
		
		if(ens[0] == ON && mask[0] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[0].2. i: "<<i<<", mask[0]: "<<mask[0]<<", incr[0]: "<<incr[0]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[1] == ON && mask[1] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[1].2. i: "<<i<<", mask[1]: "<<mask[1]<<", incr[1]: "<<incr[1]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[2] == ON && mask[2] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[2].2. i: "<<i<<", mask[2]: "<<mask[2]<<", incr[2]: "<<incr[2]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[3] == ON && mask[3] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[3].2. i: "<<i<<", mask[3]: "<<mask[3]<<", incr[3]: "<<incr[3]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[4] == ON && mask[4] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[4].2. i: "<<i<<", mask[4]: "<<mask[4]<<", incr[4]: "<<incr[4]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[5] == ON && mask[5] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[5].2. i: "<<i<<", mask[5]: "<<mask[5]<<", incr[5]: "<<incr[5]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[6] == ON && mask[6] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[6].2. i: "<<i<<", mask[6]: "<<mask[6]<<", incr[6]: "<<incr[6]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[7] == ON && mask[7] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[7].2. i: "<<i<<", mask[7]: "<<mask[7]<<", incr[7]: "<<incr[7]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[8] == ON && mask[8] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[8].2. i: "<<i<<", mask[8]: "<<mask[8]<<", incr[8]: "<<incr[8]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[9] == ON && mask[9] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[9].2. i: "<<i<<", mask[9]: "<<mask[9]<<", incr[9]: "<<incr[9]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[10] == ON && mask[10] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[10].2. i: "<<i<<", mask[10]: "<<mask[10]<<", incr[10]: "<<incr[10]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[11] == ON && mask[11] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[11].2. i: "<<i<<", mask[11]: "<<mask[11]<<", incr[11]: "<<incr[11]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[12] == ON && mask[12] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[12].2. i: "<<i<<", mask[12]: "<<mask[12]<<", incr[12]: "<<incr[12]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[13] == ON && mask[13] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[13].2. i: "<<i<<", mask[13]: "<<mask[13]<<", incr[13]: "<<incr[13]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[14] == ON && mask[14] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[14].2. i: "<<i<<", mask[14]: "<<mask[14]<<", incr[14]: "<<incr[14]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		if(ens[15] == ON && mask[15] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[15].2. i: "<<i<<", mask[15]: "<<mask[15]<<", incr[15]: "<<incr[15]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			for(unsigned int v=0; v<VSETSZ; v++){ cout<<"readandprocess(14)::DEBUG CODE 4:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
			exit(EXIT_FAILURE); 
		}
		#endif
	}
	
	if(debugid == 5){
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		if(ens[0] == ON && mask[0] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", udata: "<<udataset[incr[0]]<<"], [ens[0]: "<<ens[0]<<", mask[0]: "<<mask[0]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		if(ens[1] == ON && mask[1] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", udata: "<<udataset[incr[1]]<<"], [ens[1]: "<<ens[1]<<", mask[1]: "<<mask[1]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		if(ens[2] == ON && mask[2] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", udata: "<<udataset[incr[2]]<<"], [ens[2]: "<<ens[2]<<", mask[2]: "<<mask[2]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		if(ens[3] == ON && mask[3] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", udata: "<<udataset[incr[3]]<<"], [ens[3]: "<<ens[3]<<", mask[3]: "<<mask[3]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		if(ens[4] == ON && mask[4] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", udata: "<<udataset[incr[4]]<<"], [ens[4]: "<<ens[4]<<", mask[4]: "<<mask[4]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		if(ens[5] == ON && mask[5] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", udata: "<<udataset[incr[5]]<<"], [ens[5]: "<<ens[5]<<", mask[5]: "<<mask[5]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		if(ens[6] == ON && mask[6] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", udata: "<<udataset[incr[6]]<<"], [ens[6]: "<<ens[6]<<", mask[6]: "<<mask[6]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		if(ens[7] == ON && mask[7] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", udata: "<<udataset[incr[7]]<<"], [ens[7]: "<<ens[7]<<", mask[7]: "<<mask[7]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		if(ens[8] == ON && mask[8] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", udata: "<<udataset[incr[8]]<<"], [ens[8]: "<<ens[8]<<", mask[8]: "<<mask[8]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		if(ens[9] == ON && mask[9] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", udata: "<<udataset[incr[9]]<<"], [ens[9]: "<<ens[9]<<", mask[9]: "<<mask[9]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		if(ens[10] == ON && mask[10] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", udata: "<<udataset[incr[10]]<<"], [ens[10]: "<<ens[10]<<", mask[10]: "<<mask[10]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		if(ens[11] == ON && mask[11] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", udata: "<<udataset[incr[11]]<<"], [ens[11]: "<<ens[11]<<", mask[11]: "<<mask[11]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		if(ens[12] == ON && mask[12] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", udata: "<<udataset[incr[12]]<<"], [ens[12]: "<<ens[12]<<", mask[12]: "<<mask[12]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		if(ens[13] == ON && mask[13] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", udata: "<<udataset[incr[13]]<<"], [ens[13]: "<<ens[13]<<", mask[13]: "<<mask[13]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		if(ens[14] == ON && mask[14] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", udata: "<<udataset[incr[14]]<<"], [ens[14]: "<<ens[14]<<", mask[14]: "<<mask[14]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		if(ens[15] == ON && mask[15] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", udata: "<<udataset[incr[15]]<<"], [ens[15]: "<<ens[15]<<", mask[15]: "<<mask[15]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
	
		// for(unsigned int v=0; v<MAX_NUM_UNIQ_EDGES_PER_VEC; v++){ cout<<"readandprocess(15)::DEBUG CODE 5:: Vset["<<v<<"]: "<<Vset[v]<<", VMset["<<v<<"]: "<<VMset[v]<<endl; }
		#endif
	}
	
	if(debugid == 6){
		#ifdef _DEBUGMODE_CHECKS3
		if(mykeyvalue[0].key == INVALIDDATA || mykeyvalue[0].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		if(mykeyvalue[1].key == INVALIDDATA || mykeyvalue[1].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		if(mykeyvalue[2].key == INVALIDDATA || mykeyvalue[2].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		if(mykeyvalue[3].key == INVALIDDATA || mykeyvalue[3].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		if(mykeyvalue[4].key == INVALIDDATA || mykeyvalue[4].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		if(mykeyvalue[5].key == INVALIDDATA || mykeyvalue[5].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		if(mykeyvalue[6].key == INVALIDDATA || mykeyvalue[6].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		if(mykeyvalue[7].key == INVALIDDATA || mykeyvalue[7].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		if(mykeyvalue[8].key == INVALIDDATA || mykeyvalue[8].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		if(mykeyvalue[9].key == INVALIDDATA || mykeyvalue[9].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		if(mykeyvalue[10].key == INVALIDDATA || mykeyvalue[10].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		if(mykeyvalue[11].key == INVALIDDATA || mykeyvalue[11].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		if(mykeyvalue[12].key == INVALIDDATA || mykeyvalue[12].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		if(mykeyvalue[13].key == INVALIDDATA || mykeyvalue[13].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		if(mykeyvalue[14].key == INVALIDDATA || mykeyvalue[14].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		if(mykeyvalue[15].key == INVALIDDATA || mykeyvalue[15].value == INVALIDDATA){ *inactiveloadcount += 1; } else { *activeloadcount += 1; } 
		#endif 
	}

	if(debugid == 7){
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[0] == ON && mask[0] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[1] == ON && mask[1] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[2] == ON && mask[2] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[3] == ON && mask[3] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[4] == ON && mask[4] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[5] == ON && mask[5] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[6] == ON && mask[6] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[7] == ON && mask[7] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[8] == ON && mask[8] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[9] == ON && mask[9] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[10] == ON && mask[10] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[11] == ON && mask[11] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[12] == ON && mask[12] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[13] == ON && mask[13] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[14] == ON && mask[14] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		actsutilityobj->globalstats_countkvsprocessed(1);
		if(ens[15] == ON && mask[15] == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
		#endif 
	}
	return;
}

//
value_t acts_all::PROCESS_SPL_GETVTXMASK(unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
	#pragma HLS INLINE
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<">>> PROCESS_SPL_GETVTXMASK: ERROR DETECTED (23). loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE("<<globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE<<"). loc: "<<loc<<endl;
		#endif 
		loc = 0; }
		
	unsigned int instid = loc % NUM_PEs;
	unsigned int lloc = UTIL_GETLOCALVID(loc, instid);
	unsigned int rowoffset = instid * (globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs);
	
	unsigned int col = lloc % 16;
	unsigned int row = lloc / 16;
	unsigned int realcol = col;
	unsigned int realrow = rowoffset + row;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<">>> PROCESS_SPL_GETVTXMASK: loc: "<<loc<<", col: "<<col<<", row: "<<row<<", realcol: "<<realcol<<", realrow: "<<realrow<<endl;
	#endif 
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("UTIL_SETVTXMASK.row", row, REDUCEPARTITIONSZ_KVS2, col, row, NAp);
	actsutilityobj->checkoutofbounds("UTIL_SETVTXMASK.realcol", realcol, VDATA_PACKINGSIZE, col, row, NAp);
	actsutilityobj->checkoutofbounds("UTIL_SETVTXMASK.realrow", realrow, DOUBLE_BLOCKRAM_SIZE, col, row, NAp); 
	#endif 
	
	value_t data = vmaskBITS[realcol][realrow];
	return data;
}

value_t acts_all::PROCESS_SPL_GETVTXMASK_SLIDED(unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
	#pragma HLS INLINE
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<">>> PROCESS_SPL_GETVTXMASK: ERROR DETECTED (23). loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE("<<globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE<<"). loc: "<<loc<<endl;
		#endif 
		loc = 0; }
		
	unsigned int s = loc % NUM_PEs;
	unsigned int lloc = UTIL_GETLOCALVID(loc, s);
	unsigned int rowoffset = s * (globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs);
	
	unsigned int col = lloc % 16;
	unsigned int row = lloc / 16;
	unsigned int realcol = col;
	unsigned int rrealcol = (s + realcol) % 16;
	unsigned int realrow = rowoffset + row;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<">>> PROCESS_SPL_GETVTXMASK: loc: "<<loc<<", col: "<<col<<", row: "<<row<<", realcol: "<<realcol<<", realrow: "<<realrow<<endl;
	#endif 
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("UTIL_SETVTXMASK.row", row, REDUCEPARTITIONSZ_KVS2, col, row, NAp);
	actsutilityobj->checkoutofbounds("UTIL_SETVTXMASK.realcol", realcol, VDATA_PACKINGSIZE, col, row, NAp);
	actsutilityobj->checkoutofbounds("UTIL_SETVTXMASK.rrealcol", rrealcol, VDATA_PACKINGSIZE, col, row, NAp);
	actsutilityobj->checkoutofbounds("UTIL_SETVTXMASK.realrow", realrow, DOUBLE_BLOCKRAM_SIZE, col, row, NAp); 
	#endif 
	
	value_t data = vmaskBITS[rrealcol][realrow];
	return data;
}

sliceinfos_t acts_all::PROCESS_SPL_GETVTXSLICES(unsigned int loc, globalparams_t globalparams){					
	#pragma HLS INLINE
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<">>> PROCESS_SPL_GETVTXMASK: ERROR DETECTED (23). loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE("<<globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE<<"). loc: "<<loc<<endl;
		#endif 
		loc = 0; }

	int s_begin = loc % NUM_PEs; //
	int llocA = UTIL_GETLOCALVID(loc, s_begin);
	int v_begin = llocA % 16; //
	int s_base = -1 * v_begin;
	int row = llocA / 16;

	int loc2 = ((loc / NUM_PEs) * NUM_PEs) + NUM_PEs;
	int s_beginB = 0;
	int llocB = llocA + 1;
	int v_beginB = llocB % 16;
	int s_baseB = -1 * v_beginB;
	int rowB = llocB / 16;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"PROCESS_SPL_GETVTXSLICES(A): loc: "<<loc<<", s_begin: "<<s_begin<<", llocA: "<<llocA<<", v_begin: "<<v_begin<<", s_base: "<<s_base<<", row: "<<row<<endl;
	cout<<"PROCESS_SPL_GETVTXSLICES(B): loc2: "<<loc2<<", s_beginB: "<<s_beginB<<", llocB: "<<llocB<<", v_beginB: "<<v_beginB<<", s_baseB: "<<s_baseB<<", rowB: "<<rowB<<" "<<endl;
	#endif 
	
	sliceinfo_t sliceX; sliceX.active = false; sliceX.beginoffset = 0; sliceX.endoffset = NUM_PEs;
	sliceinfo_t sliceY; sliceY.active = false; sliceY.beginoffset = 0; sliceY.endoffset = NUM_PEs;

	int s_baseX; 
	int s_baseY;
	
	int rowX; 
	int rowY;
	
	int s_baseA0 = s_base+0;
	int s_baseA1 = s_base+VECTOR2_SIZE;
	int s_baseA2 = s_base+VECTOR2_SIZE+VECTOR2_SIZE;
	
	int s_baseB0 = s_baseB+0;
	int s_baseB1 = s_baseB+VECTOR2_SIZE;
	int s_baseB2 = s_baseB+VECTOR2_SIZE+VECTOR2_SIZE;

	if(s_begin>=s_baseA0 && s_begin<s_baseA0+VECTOR2_SIZE){
		sliceX.active = true; sliceX.s_base = s_baseA0; sliceX.beginoffset = s_begin - s_baseA0; if(s_baseA0 + VECTOR2_SIZE > NUM_PEs){ sliceX.endoffset = NUM_PEs - s_baseA0; } else{ sliceX.endoffset = VECTOR2_SIZE; } sliceX.local_rowoffset = row; 
		sliceY.active = true; sliceY.s_base = s_baseA1; sliceY.beginoffset = 0; if(s_baseA1 + VECTOR2_SIZE > NUM_PEs){ sliceY.endoffset = NUM_PEs - s_baseA1; } else{ sliceY.endoffset = VECTOR2_SIZE; } sliceY.local_rowoffset = row; 
	} else if(s_begin>=s_baseA1 && s_begin<s_baseA1+VECTOR2_SIZE){
		sliceX.active = true; sliceX.s_base = s_baseA1; sliceX.beginoffset = s_begin - s_baseA1; if(s_baseA1 + VECTOR2_SIZE > NUM_PEs){ sliceX.endoffset = NUM_PEs - s_baseA1; } else { sliceX.endoffset = VECTOR2_SIZE; } sliceX.local_rowoffset = row; 
		sliceY.active = true; sliceY.s_base = s_baseA2; sliceY.beginoffset = 0; if(s_baseA2 + VECTOR2_SIZE > NUM_PEs){ sliceY.endoffset = NUM_PEs - s_baseA2; } else{ sliceY.endoffset = VECTOR2_SIZE; } sliceY.local_rowoffset = row; 
	} else if(s_begin>=s_baseA2 && s_begin<s_baseA2+VECTOR2_SIZE){
		sliceX.active = true; sliceX.s_base = s_baseA2; sliceX.beginoffset = s_begin - s_baseA2; if(s_baseA2 + VECTOR2_SIZE > NUM_PEs){ sliceX.endoffset = NUM_PEs - s_baseA2; } else { sliceX.endoffset = VECTOR2_SIZE; } sliceX.local_rowoffset = row; 
		sliceY.active = true; sliceY.s_base = s_baseB0; sliceY.beginoffset = s_beginB - s_baseB0; if(s_baseB0 + VECTOR2_SIZE > NUM_PEs){ sliceY.endoffset = NUM_PEs - s_baseB0; } else{ sliceY.endoffset = VECTOR2_SIZE; } sliceY.local_rowoffset = rowB; 
	} else {
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"PROCESS_SPL_GETVTXSLICES:: SHOULD NOT BE SEEN. 23 EXITING..."<<endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	
	sliceinfos_t sliceinfos; sliceinfos.slice[0] = sliceX; sliceinfos.slice[1] = sliceY;
	return sliceinfos;
}

sliceinfo_t acts_all::PROCESS_SPL_GETVTXSLICE(unsigned int id, unsigned int loc, globalparams_t globalparams){					
	#pragma HLS INLINE
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<">>> PROCESS_SPL_GETVTXMASK: ERROR DETECTED (23). loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE("<<globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE<<"). loc: "<<loc<<endl;
		#endif 
		loc = 0; }

	int s_begin = loc % NUM_PEs; //
	int llocA = UTIL_GETLOCALVID(loc, s_begin);
	int v_begin = llocA % 16; //
	int s_base = -1 * v_begin;
	int row = llocA / 16;

	int loc2 = ((loc / NUM_PEs) * NUM_PEs) + NUM_PEs;
	int s_beginB = 0;
	int llocB = llocA + 1;
	int v_beginB = llocB % 16;
	int s_baseB = -1 * v_beginB;
	int rowB = llocB / 16;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"PROCESS_SPL_GETVTXSLICES(A): loc: "<<loc<<", s_begin: "<<s_begin<<", llocA: "<<llocA<<", v_begin: "<<v_begin<<", s_base: "<<s_base<<", row: "<<row<<endl;
	cout<<"PROCESS_SPL_GETVTXSLICES(B): loc2: "<<loc2<<", s_beginB: "<<s_beginB<<", llocB: "<<llocB<<", v_beginB: "<<v_beginB<<", s_baseB: "<<s_baseB<<", rowB: "<<rowB<<" "<<endl;
	#endif 
	
	sliceinfo_t sliceX; sliceX.active = false; sliceX.beginoffset = 0; sliceX.endoffset = NUM_PEs;
	sliceinfo_t sliceY; sliceY.active = false; sliceY.beginoffset = 0; sliceY.endoffset = NUM_PEs;

	int s_baseX; 
	int s_baseY;
	
	int rowX; 
	int rowY;
	
	int s_baseA0 = s_base+0;
	int s_baseA1 = s_base+VECTOR2_SIZE;
	int s_baseA2 = s_base+VECTOR2_SIZE+VECTOR2_SIZE;
	
	int s_baseB0 = s_baseB+0;
	int s_baseB1 = s_baseB+VECTOR2_SIZE;
	int s_baseB2 = s_baseB+VECTOR2_SIZE+VECTOR2_SIZE;

	if(s_begin>=s_baseA0 && s_begin<s_baseA0+VECTOR2_SIZE){
		sliceX.active = true; sliceX.s_base = s_baseA0; sliceX.beginoffset = s_begin - s_baseA0; if(s_baseA0 + VECTOR2_SIZE > NUM_PEs){ sliceX.endoffset = NUM_PEs - s_baseA0; } else{ sliceX.endoffset = VECTOR2_SIZE; } sliceX.local_rowoffset = row; 
		sliceY.active = true; sliceY.s_base = s_baseA1; sliceY.beginoffset = 0; if(s_baseA1 + VECTOR2_SIZE > NUM_PEs){ sliceY.endoffset = NUM_PEs - s_baseA1; } else{ sliceY.endoffset = VECTOR2_SIZE; } sliceY.local_rowoffset = row; 
	} else if(s_begin>=s_baseA1 && s_begin<s_baseA1+VECTOR2_SIZE){
		sliceX.active = true; sliceX.s_base = s_baseA1; sliceX.beginoffset = s_begin - s_baseA1; if(s_baseA1 + VECTOR2_SIZE > NUM_PEs){ sliceX.endoffset = NUM_PEs - s_baseA1; } else { sliceX.endoffset = VECTOR2_SIZE; } sliceX.local_rowoffset = row; 
		sliceY.active = true; sliceY.s_base = s_baseA2; sliceY.beginoffset = 0; if(s_baseA2 + VECTOR2_SIZE > NUM_PEs){ sliceY.endoffset = NUM_PEs - s_baseA2; } else{ sliceY.endoffset = VECTOR2_SIZE; } sliceY.local_rowoffset = row; 
	} else if(s_begin>=s_baseA2 && s_begin<s_baseA2+VECTOR2_SIZE){
		sliceX.active = true; sliceX.s_base = s_baseA2; sliceX.beginoffset = s_begin - s_baseA2; if(s_baseA2 + VECTOR2_SIZE > NUM_PEs){ sliceX.endoffset = NUM_PEs - s_baseA2; } else { sliceX.endoffset = VECTOR2_SIZE; } sliceX.local_rowoffset = row; 
		sliceY.active = true; sliceY.s_base = s_baseB0; sliceY.beginoffset = s_beginB - s_baseB0; if(s_baseB0 + VECTOR2_SIZE > NUM_PEs){ sliceY.endoffset = NUM_PEs - s_baseB0; } else{ sliceY.endoffset = VECTOR2_SIZE; } sliceY.local_rowoffset = rowB; 
	} else {
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"PROCESS_SPL_GETVTXSLICES:: SHOULD NOT BE SEEN. 23 EXITING..."<<endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	
	sliceinfo_t sliceinfo;
	if(id==0){ sliceinfo = sliceX; } else { sliceinfo = sliceY; }
	return sliceinfo;
}

void acts_all::PROCESS_SPL1_GETVTXDATASET(unsigned int loc, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], unit1_type VMdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], globalparams_t globalparams){
	#pragma HLS INLINE
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<">>> PROCESS_SPL1_GETVTXDATASET: ERROR DETECTED (23). loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE("<<globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE<<"). loc: "<<loc<<endl;
		#endif 
		loc = 0; }

	for(unsigned int v=0; v<MAX_NUM_UNIQ_EDGES_PER_VEC; v++){
		unsigned int instid = (loc+v) % NUM_PEs;
		unsigned int lloc = UTIL_GETLOCALVID((loc+v), instid);
		unsigned int rowoffset = instid * (globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs); // TOO EXPENSIVE.
		
		unsigned int col = lloc % 16;
		unsigned int row = lloc / 16;
		unsigned int realcol = col;
		unsigned int realrow = rowoffset + row;
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("PROCESS_SPL3_GETVTXDATASET.realcol", realcol, VECTOR2_SIZE, realcol, loc, v);
		actsutilityobj->checkoutofbounds("PROCESS_SPL3_GETVTXDATASET.realrow", realrow, DOUBLE_BLOCKRAM_SIZE, realrow, loc, v);
		#endif
		// if(row % 2 == 0){ Vdatas[v] = UTIL_GETKV2(vbuffer[realcol][realrow]).key; } 
		// else { Vdatas[v] = UTIL_GETKV2(vbuffer[realcol][realrow]).value; }
		// VMdatas[v] = vmaskBITS[realcol][realrow];
		
		// VMdatas[v] = vmaskBITS[realcol][realrow];
		// keyvalue_t KV = UTIL_GETKV2(vbuffer[realcol][realrow/2]);
		// if(realrow%2==0){ Vdatas[v] = KV.key; } else { Vdatas[v] = KV.value; }

		VMdatas[v] = MEMCA_READFROMBUFFER_VMASK(realrow, vmaskBITS[realcol], 0);
		Vdatas[v] = MEMCA_READFROMBUFFER_VDATA(realrow, vbuffer[realcol], 0);
	}
	return;
}

void acts_all::PROCESS_SPL2_GETVTXDATASET(unsigned int loc, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], unit1_type VMdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], globalparams_t globalparams){					
	for(unsigned int v=0; v<MAX_NUM_UNIQ_EDGES_PER_VEC; v++){
		// unsigned int index = 16369;
		// unsigned int index = 16352; // loc + v
		// unsigned int index = 16144;
		// unsigned int index = loc + v;
		unsigned int index = loc + v;
		if(index >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE){ index = 0; }
		
		#if NUM_PEs==3
		NOT IMPLEMENTED.
		#elif NUM_PEs==12
		NOT IMPLEMENTED.
		#elif NUM_PEs==22
		NOT IMPLEMENTED.
		#elif NUM_PEs==24
		NOT IMPLEMENTED.
		#elif NUM_PEs==25
		NOT IMPLEMENTED.
		#elif NUM_PEs==32
			unsigned int col_dim32 = index % NUM_PEs;
			unsigned int row_dim32 = index / NUM_PEs; // OPTIMIZE: follow with processedges_splitdstvxs
			unsigned int basecol_dim16 = 0;
			unsigned int col_dim16 = index % VECTOR2_SIZE;
			unsigned int row_dim16 = index / VECTOR2_SIZE;
			
			unsigned int block = index / 512;
			unsigned int baserow_dim16 = ((index / 512) * 512) / VECTOR2_SIZE;
			unsigned int localrow_16 = row_dim16 - baserow_dim16;
			localrow_16 = localrow_16 / 2; if(col_dim32 < 16){ localrow_16 = localrow_16; } else{ localrow_16 = localrow_16 + 16; }
			unsigned int truerow_dim16 = baserow_dim16 + localrow_16;
		#else
		NOT IMPLEMENTED.
		#endif
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"PROCESS_SPL2_GETVTXDATASET[AFTER]:: index: "<<index<<", col_dim32: "<<col_dim32<<", row_dim32: "<<row_dim32<<", col_dim16: "<<col_dim16<<", baserow_dim16: "<<baserow_dim16<<", localrow_16: "<<localrow_16<<", truerow_dim16: "<<truerow_dim16<<endl;
		#endif 
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("PROCESS_SPL2_GETVTXDATASET.col_dim16", col_dim16, VECTOR2_SIZE, col_dim16, index, loc);
		actsutilityobj->checkoutofbounds("PROCESS_SPL2_GETVTXDATASET.truerow_dim16", truerow_dim16, DOUBLE_BLOCKRAM_SIZE, truerow_dim16, index, loc);
		#endif 
		// VMdatas[v] = vmaskBITS[col_dim16][truerow_dim16];
		// keyvalue_t KV = UTIL_GETKV2(vbuffer[col_dim16][truerow_dim16/2]);
		// if(truerow_dim16%2==0){ Vdatas[v] = KV.key; } else { Vdatas[v] = KV.value; }
		VMdatas[v] = MEMCA_READFROMBUFFER_VMASK(truerow_dim16, vmaskBITS[col_dim16], 0);
		Vdatas[v] = MEMCA_READFROMBUFFER_VDATA(truerow_dim16, vbuffer[col_dim16], 0);
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(VMdatas[v] == 1){ cout<<"PROCESS_SPL2_GETVTXDATASET: MASK=1 FOUND: VMdatas["<<v<<"]: "<<VMdatas[v]<<", vmaskBITS["<<col_dim16<<"]["<<truerow_dim16<<"]: "<<vmaskBITS[col_dim16][truerow_dim16]<<", (loc + v): "<<loc + v<<", loc: "<<loc<<", v: "<<v<<endl; }
		#endif
	}
	return;
}

unsigned int acts_all::PROCESS_SPL3_GETROW(unsigned int loc){					
	unsigned int col_dim32 = loc % NUM_PEs;
	unsigned int row_dim32 = loc / NUM_PEs; // OPTIMIZE: follow with processedges_splitdstvxs
	unsigned int basecol_dim16 = 0;
	unsigned int col_dim16 = loc % VECTOR2_SIZE;
	unsigned int row_dim16 = loc / VECTOR2_SIZE;
	
	unsigned int block = loc / 512;
	unsigned int baserow_dim16 = ((loc / 512) * 512) / VECTOR2_SIZE;
	unsigned int localrow_16 = row_dim16 - baserow_dim16;
	localrow_16 = localrow_16 / 2; if(col_dim32 < 16){ localrow_16 = localrow_16; } else{ localrow_16 = localrow_16 + 16; }
	unsigned int truerow_dim16 = baserow_dim16 + localrow_16;
	
	return truerow_dim16;
}

unsigned int acts_all::PROCESS_SPL3_GETVTXDATASET(unsigned int loc, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[VSETSZ], unit1_type VMdatas[VMSETSZ], globalparams_t globalparams){					
	unit1_type VMdatas_tmp[32];
	unit1_type VMdatas_tmpA[VECTOR2_SIZE];
	unit1_type VMdatas_tmpB[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=VMdatas_tmp complete
	#pragma HLS ARRAY_PARTITION variable=VMdatas_tmpA complete
	#pragma HLS ARRAY_PARTITION variable=VMdatas_tmpB complete
	value_t Vdatas_tmp[32];
	value_t Vdatas_tmpA[VECTOR2_SIZE];
	value_t Vdatas_tmpB[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=Vdatas_tmp complete
	#pragma HLS ARRAY_PARTITION variable=Vdatas_tmpA complete
	#pragma HLS ARRAY_PARTITION variable=Vdatas_tmpB complete
	
	unsigned int fcol_dim16 = loc % VECTOR2_SIZE;
	unsigned int col_dim16 = loc % VECTOR2_SIZE;
	
	unsigned int truerow1_dim16 = PROCESS_SPL3_GETROW(loc);
	unsigned int truerow2_dim16 = PROCESS_SPL3_GETROW((loc + (VECTOR2_SIZE - col_dim16)));

	// CRITICAL REMOVEME.
	#ifdef CONFIG_SEPERATEVMASKFROMVDATA
	if(truerow1_dim16 < globalparams.SIZEKVS2_REDUCEPARTITION){ MEMCA_READFROMBUFFER_VDATAS(truerow1_dim16, vbuffer, Vdatas_tmpA, 0); }
	if(truerow2_dim16 < globalparams.SIZEKVS2_REDUCEPARTITION){ MEMCA_READFROMBUFFER_VDATAS(truerow2_dim16, vbuffer, Vdatas_tmpB, 0); }
	if(truerow1_dim16 < globalparams.SIZEKVS2_REDUCEPARTITION){ MEMCA_READFROMBUFFER_VMASKS(truerow1_dim16, vmaskBITS, VMdatas_tmpA, 0); }
	if(truerow2_dim16 < globalparams.SIZEKVS2_REDUCEPARTITION){ MEMCA_READFROMBUFFER_VMASKS(truerow2_dim16, vmaskBITS, VMdatas_tmpB, 0); }
	#else 
	if(truerow1_dim16 < globalparams.SIZEKVS2_REDUCEPARTITION){ MEMCA_READFROMBUFFER_VDATASWITHVMASKS(truerow1_dim16, vbuffer, Vdatas_tmpA, VMdatas_tmpA, 0); }
	if(truerow2_dim16 < globalparams.SIZEKVS2_REDUCEPARTITION){ MEMCA_READFROMBUFFER_VDATASWITHVMASKS(truerow2_dim16, vbuffer, Vdatas_tmpB, VMdatas_tmpB, 0); }
	#endif 

	Vdatas[0] = Vdatas_tmpA[0]; 
	VMdatas[0] = VMdatas_tmpA[0];
	Vdatas[1] = Vdatas_tmpA[1]; 
	VMdatas[1] = VMdatas_tmpA[1];
	Vdatas[2] = Vdatas_tmpA[2]; 
	VMdatas[2] = VMdatas_tmpA[2];
	Vdatas[3] = Vdatas_tmpA[3]; 
	VMdatas[3] = VMdatas_tmpA[3];
	Vdatas[4] = Vdatas_tmpA[4]; 
	VMdatas[4] = VMdatas_tmpA[4];
	Vdatas[5] = Vdatas_tmpA[5]; 
	VMdatas[5] = VMdatas_tmpA[5];
	Vdatas[6] = Vdatas_tmpA[6]; 
	VMdatas[6] = VMdatas_tmpA[6];
	Vdatas[7] = Vdatas_tmpA[7]; 
	VMdatas[7] = VMdatas_tmpA[7];
	Vdatas[8] = Vdatas_tmpA[8]; 
	VMdatas[8] = VMdatas_tmpA[8];
	Vdatas[9] = Vdatas_tmpA[9]; 
	VMdatas[9] = VMdatas_tmpA[9];
	Vdatas[10] = Vdatas_tmpA[10]; 
	VMdatas[10] = VMdatas_tmpA[10];
	Vdatas[11] = Vdatas_tmpA[11]; 
	VMdatas[11] = VMdatas_tmpA[11];
	Vdatas[12] = Vdatas_tmpA[12]; 
	VMdatas[12] = VMdatas_tmpA[12];
	Vdatas[13] = Vdatas_tmpA[13]; 
	VMdatas[13] = VMdatas_tmpA[13];
	Vdatas[14] = Vdatas_tmpA[14]; 
	VMdatas[14] = VMdatas_tmpA[14];
	Vdatas[15] = Vdatas_tmpA[15]; 
	VMdatas[15] = VMdatas_tmpA[15];
	Vdatas[16] = Vdatas_tmpB[0];
	VMdatas[16] = VMdatas_tmpB[0]; 
	Vdatas[17] = Vdatas_tmpB[1];
	VMdatas[17] = VMdatas_tmpB[1]; 
	Vdatas[18] = Vdatas_tmpB[2];
	VMdatas[18] = VMdatas_tmpB[2]; 
	Vdatas[19] = Vdatas_tmpB[3];
	VMdatas[19] = VMdatas_tmpB[3]; 
	Vdatas[20] = Vdatas_tmpB[4];
	VMdatas[20] = VMdatas_tmpB[4]; 
	Vdatas[21] = Vdatas_tmpB[5];
	VMdatas[21] = VMdatas_tmpB[5]; 
	Vdatas[22] = Vdatas_tmpB[6];
	VMdatas[22] = VMdatas_tmpB[6]; 
	Vdatas[23] = Vdatas_tmpB[7];
	VMdatas[23] = VMdatas_tmpB[7]; 
	Vdatas[24] = Vdatas_tmpB[8];
	VMdatas[24] = VMdatas_tmpB[8]; 
	Vdatas[25] = Vdatas_tmpB[9];
	VMdatas[25] = VMdatas_tmpB[9]; 
	Vdatas[26] = Vdatas_tmpB[10];
	VMdatas[26] = VMdatas_tmpB[10]; 
	Vdatas[27] = Vdatas_tmpB[11];
	VMdatas[27] = VMdatas_tmpB[11]; 
	Vdatas[28] = Vdatas_tmpB[12];
	VMdatas[28] = VMdatas_tmpB[12]; 
	Vdatas[29] = Vdatas_tmpB[13];
	VMdatas[29] = VMdatas_tmpB[13]; 
	Vdatas[30] = Vdatas_tmpB[14];
	VMdatas[30] = VMdatas_tmpB[14]; 
	Vdatas[31] = Vdatas_tmpB[15];
	VMdatas[31] = VMdatas_tmpB[15]; 
	return fcol_dim16;
}

fetchmessage_t acts_all::PROCESS_SPL_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	
	analysis_type analysis_loop = BLOCKRAM_SIZE / 2;

	uint32_type E[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	bool_type en = ON;
	bool_type ens[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=ens complete
	unsigned int udata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=udata complete
	unit1_type mask[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=mask complete
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	unsigned int validbound = reducebuffersz * FETFACTOR * VECTOR2_SIZE;
	
	travstate.i_kvs = travstate.i_kvs / 2;
	travstate.end_kvs = travstate.end_kvs / 2;
	loffset_kvs = loffset_kvs / 2;
	buffer_type edgessize_kvs = size_kvs / 2;
	
	batch_type offset_kvs = goffset_kvs + loffset_kvs;
	
	value_t udataset[MAX_NUM_UNIQ_EDGES_PER_VEC];
	#pragma HLS ARRAY_PARTITION variable=udataset complete
	value_t maskset[MAX_NUM_UNIQ_EDGES_PER_VEC];
	#pragma HLS ARRAY_PARTITION variable=maskset complete
	vertex_t lvids[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=lvids complete
	keyvalue_t mykeyvalue[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=mykeyvalue complete
	
	unit4_type incr[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=incr complete
	unsigned int lsrcvids[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=lsrcvids complete
	unsigned int ldstvids[VECTOR2_SIZE];	
	#pragma HLS ARRAY_PARTITION variable=ldstvids complete
	value_t res[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=res complete
	
	value_t Vsets[2][VECTOR2_SIZE]; // DEPRECIATED.
	#pragma HLS ARRAY_PARTITION variable=Vset complete
	unit1_type VMsets[2][VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=VMset complete
	
	value_t Vset[VSETSZ];
	#pragma HLS ARRAY_PARTITION variable=Vset complete
	unit1_type VMset[VMSETSZ];
	#pragma HLS ARRAY_PARTITION variable=VMset complete
	
	sliceinfos_t sliceinfos;
	
	vertex_t srcvid_head;
	vertex_t lvid_head;
	
	batch_type vptrbaseoffset_kvs = globalparams.BASEOFFSETKVS_VERTEXPTR + (globalparams.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	batch_type voffset_kvs = sweepparams.source_partition * reducebuffersz * FETFACTOR;
	keyy_t nextbeginvptr;
	keyy_t nextbeginvptr_kvs;
	
	fetchmessage.chunksize_kvs = edgessize_kvs;
	fetchmessage.nextoffset_kvs = -1;
	buffer_type loadcount = 0;
	buffer_type activeloadcount = 0;
	buffer_type inactiveloadcount = 0;
	
	unsigned int debug_numinvalidheads = 0;
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	for(unsigned int v=0; v<MAX_NUM_UNIQ_EDGES_PER_VEC; v++){ udataset[v] = 0; }
	unsigned int offset = 0;
	keyvalue_t dummykeyvalue; dummykeyvalue.key = INVALIDDATA; dummykeyvalue.value = INVALIDDATA;
	
	incr[0] = 0;
	lsrcvids[0] = 0;
	ldstvids[0] = 0;	
	E[0] = 0;
	ens[0] = 0;
	udata[0] = 0;
	mask[0] = 0;
	incr[1] = 0;
	lsrcvids[1] = 0;
	ldstvids[1] = 0;	
	E[1] = 0;
	ens[1] = 0;
	udata[1] = 0;
	mask[1] = 0;
	incr[2] = 0;
	lsrcvids[2] = 0;
	ldstvids[2] = 0;	
	E[2] = 0;
	ens[2] = 0;
	udata[2] = 0;
	mask[2] = 0;
	incr[3] = 0;
	lsrcvids[3] = 0;
	ldstvids[3] = 0;	
	E[3] = 0;
	ens[3] = 0;
	udata[3] = 0;
	mask[3] = 0;
	incr[4] = 0;
	lsrcvids[4] = 0;
	ldstvids[4] = 0;	
	E[4] = 0;
	ens[4] = 0;
	udata[4] = 0;
	mask[4] = 0;
	incr[5] = 0;
	lsrcvids[5] = 0;
	ldstvids[5] = 0;	
	E[5] = 0;
	ens[5] = 0;
	udata[5] = 0;
	mask[5] = 0;
	incr[6] = 0;
	lsrcvids[6] = 0;
	ldstvids[6] = 0;	
	E[6] = 0;
	ens[6] = 0;
	udata[6] = 0;
	mask[6] = 0;
	incr[7] = 0;
	lsrcvids[7] = 0;
	ldstvids[7] = 0;	
	E[7] = 0;
	ens[7] = 0;
	udata[7] = 0;
	mask[7] = 0;
	incr[8] = 0;
	lsrcvids[8] = 0;
	ldstvids[8] = 0;	
	E[8] = 0;
	ens[8] = 0;
	udata[8] = 0;
	mask[8] = 0;
	incr[9] = 0;
	lsrcvids[9] = 0;
	ldstvids[9] = 0;	
	E[9] = 0;
	ens[9] = 0;
	udata[9] = 0;
	mask[9] = 0;
	incr[10] = 0;
	lsrcvids[10] = 0;
	ldstvids[10] = 0;	
	E[10] = 0;
	ens[10] = 0;
	udata[10] = 0;
	mask[10] = 0;
	incr[11] = 0;
	lsrcvids[11] = 0;
	ldstvids[11] = 0;	
	E[11] = 0;
	ens[11] = 0;
	udata[11] = 0;
	mask[11] = 0;
	incr[12] = 0;
	lsrcvids[12] = 0;
	ldstvids[12] = 0;	
	E[12] = 0;
	ens[12] = 0;
	udata[12] = 0;
	mask[12] = 0;
	incr[13] = 0;
	lsrcvids[13] = 0;
	ldstvids[13] = 0;	
	E[13] = 0;
	ens[13] = 0;
	udata[13] = 0;
	mask[13] = 0;
	incr[14] = 0;
	lsrcvids[14] = 0;
	ldstvids[14] = 0;	
	E[14] = 0;
	ens[14] = 0;
	udata[14] = 0;
	mask[14] = 0;
	incr[15] = 0;
	lsrcvids[15] = 0;
	ldstvids[15] = 0;	
	E[15] = 0;
	ens[15] = 0;
	udata[15] = 0;
	mask[15] = 0;
	
	buffer_type chunk_size = UTIL_getchunksize_kvs(edgessize_kvs, travstate, 0);
	READANDPROCESS_SPL_LOOP3C: for(buffer_type setoffset=0; setoffset<VECTOR2_SIZE; setoffset+=PE_SETSZ){
		READANDPROCESS_SPL_LOOP3D: for (buffer_type i=0; i<chunk_size; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop avg=analysis_loop	
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"--- PROCESS_SPL_readandprocess: setoffset: "<<setoffset<<", i: "<<i<<" (of setsize: "<<VECTOR2_SIZE<<", of "<<chunk_size<<")"<<endl;
			#endif

			#ifdef _WIDEWORD
			E[0] = edges[offset_kvs + i].range(31, 0); 
			E[1] = edges[offset_kvs + i].range(63, 32); 
			E[2] = edges[offset_kvs + i].range(95, 64); 
			E[3] = edges[offset_kvs + i].range(127, 96); 
			E[4] = edges[offset_kvs + i].range(159, 128); 
			E[5] = edges[offset_kvs + i].range(191, 160); 
			E[6] = edges[offset_kvs + i].range(223, 192); 
			E[7] = edges[offset_kvs + i].range(255, 224); 
			E[8] = edges[offset_kvs + i].range(287, 256); 
			E[9] = edges[offset_kvs + i].range(319, 288); 
			E[10] = edges[offset_kvs + i].range(351, 320); 
			E[11] = edges[offset_kvs + i].range(383, 352); 
			E[12] = edges[offset_kvs + i].range(415, 384); 
			E[13] = edges[offset_kvs + i].range(447, 416); 
			E[14] = edges[offset_kvs + i].range(479, 448); 
			E[15] = edges[offset_kvs + i].range(511, 480); 
			#else 
			E[0] = edges[offset_kvs + i].data[0].key; 
			E[1] = edges[offset_kvs + i].data[0].value;
			E[2] = edges[offset_kvs + i].data[1].key; 
			E[3] = edges[offset_kvs + i].data[1].value;
			E[4] = edges[offset_kvs + i].data[2].key; 
			E[5] = edges[offset_kvs + i].data[2].value;
			E[6] = edges[offset_kvs + i].data[3].key; 
			E[7] = edges[offset_kvs + i].data[3].value;
			E[8] = edges[offset_kvs + i].data[4].key; 
			E[9] = edges[offset_kvs + i].data[4].value;
			E[10] = edges[offset_kvs + i].data[5].key; 
			E[11] = edges[offset_kvs + i].data[5].value;
			E[12] = edges[offset_kvs + i].data[6].key; 
			E[13] = edges[offset_kvs + i].data[6].value;
			E[14] = edges[offset_kvs + i].data[7].key; 
			E[15] = edges[offset_kvs + i].data[7].value;
			#endif
			#ifdef _DEBUGMODE_CHECKS3
			PROCESS_SPL_debug(0, i, E, ens, mask, udataset, maskset, Vset, VMset, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
			#endif 
				
			srcvid_head = E[0];
			lvid_head = srcvid_head - travstate.i2;	
			
			parsededge_t parsed_edgeSTD0 = PARSE_EDGE(E[0]); unsigned int incrSTD0 = parsed_edgeSTD0.incr; unsigned int lsrcvidsSTD0 = lvid_head + incrSTD0; unsigned int ldstvidsSTD0 = parsed_edgeSTD0.dstvid; // STD0
			parsededge_t parsed_edgeSTD1 = PARSE_EDGE(E[1]); unsigned int incrSTD1 = parsed_edgeSTD1.incr; unsigned int lsrcvidsSTD1 = lvid_head + incrSTD1; unsigned int ldstvidsSTD1 = parsed_edgeSTD1.dstvid; // STD1
			parsededge_t parsed_edge0 = PARSE_EDGE(E[setoffset + 0]);
			incr[0] = parsed_edge0.incr;
			lsrcvids[0] = lvid_head + incr[0];
			ldstvids[0] = parsed_edge0.dstvid;	
			parsededge_t parsed_edge1 = PARSE_EDGE(E[setoffset + 1]);
			incr[1] = parsed_edge1.incr;
			lsrcvids[1] = lvid_head + incr[1];
			ldstvids[1] = parsed_edge1.dstvid;	
			parsededge_t parsed_edge2 = PARSE_EDGE(E[setoffset + 2]);
			incr[2] = parsed_edge2.incr;
			lsrcvids[2] = lvid_head + incr[2];
			ldstvids[2] = parsed_edge2.dstvid;	
			parsededge_t parsed_edge3 = PARSE_EDGE(E[setoffset + 3]);
			incr[3] = parsed_edge3.incr;
			lsrcvids[3] = lvid_head + incr[3];
			ldstvids[3] = parsed_edge3.dstvid;	
			parsededge_t parsed_edge4 = PARSE_EDGE(E[setoffset + 4]);
			incr[4] = parsed_edge4.incr;
			lsrcvids[4] = lvid_head + incr[4];
			ldstvids[4] = parsed_edge4.dstvid;	
			parsededge_t parsed_edge5 = PARSE_EDGE(E[setoffset + 5]);
			incr[5] = parsed_edge5.incr;
			lsrcvids[5] = lvid_head + incr[5];
			ldstvids[5] = parsed_edge5.dstvid;	
			parsededge_t parsed_edge6 = PARSE_EDGE(E[setoffset + 6]);
			incr[6] = parsed_edge6.incr;
			lsrcvids[6] = lvid_head + incr[6];
			ldstvids[6] = parsed_edge6.dstvid;	
			parsededge_t parsed_edge7 = PARSE_EDGE(E[setoffset + 7]);
			incr[7] = parsed_edge7.incr;
			lsrcvids[7] = lvid_head + incr[7];
			ldstvids[7] = parsed_edge7.dstvid;	
			parsededge_t parsed_edge8 = PARSE_EDGE(E[setoffset + 8]);
			incr[8] = parsed_edge8.incr;
			lsrcvids[8] = lvid_head + incr[8];
			ldstvids[8] = parsed_edge8.dstvid;	
			parsededge_t parsed_edge9 = PARSE_EDGE(E[setoffset + 9]);
			incr[9] = parsed_edge9.incr;
			lsrcvids[9] = lvid_head + incr[9];
			ldstvids[9] = parsed_edge9.dstvid;	
			parsededge_t parsed_edge10 = PARSE_EDGE(E[setoffset + 10]);
			incr[10] = parsed_edge10.incr;
			lsrcvids[10] = lvid_head + incr[10];
			ldstvids[10] = parsed_edge10.dstvid;	
			parsededge_t parsed_edge11 = PARSE_EDGE(E[setoffset + 11]);
			incr[11] = parsed_edge11.incr;
			lsrcvids[11] = lvid_head + incr[11];
			ldstvids[11] = parsed_edge11.dstvid;	
			parsededge_t parsed_edge12 = PARSE_EDGE(E[setoffset + 12]);
			incr[12] = parsed_edge12.incr;
			lsrcvids[12] = lvid_head + incr[12];
			ldstvids[12] = parsed_edge12.dstvid;	
			parsededge_t parsed_edge13 = PARSE_EDGE(E[setoffset + 13]);
			incr[13] = parsed_edge13.incr;
			lsrcvids[13] = lvid_head + incr[13];
			ldstvids[13] = parsed_edge13.dstvid;	
			parsededge_t parsed_edge14 = PARSE_EDGE(E[setoffset + 14]);
			incr[14] = parsed_edge14.incr;
			lsrcvids[14] = lvid_head + incr[14];
			ldstvids[14] = parsed_edge14.dstvid;	
			parsededge_t parsed_edge15 = PARSE_EDGE(E[setoffset + 15]);
			incr[15] = parsed_edge15.incr;
			lsrcvids[15] = lvid_head + incr[15];
			ldstvids[15] = parsed_edge15.dstvid;	
			
			bool_type en = ON;
			#ifdef _DEBUGMODE_CHECKS3
			PROCESS_SPL_debug(1, i, E, ens, mask, udataset, maskset, Vset, VMset, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
			#endif
			
			#if defined(CONFIG_READVDATA_SLIDE)
			NOT IMPLEMENTED.
			#elif defined(CONFIG_READVDATA_SLIDEANDREARRANGE)
			offset = PROCESS_SPL3_GETVTXDATASET(lvid_head, vbuffer, vmaskBITS, Vset, VMset, globalparams);
			#else
			PROCESS_SPL1_GETVTXDATASET(lvid_head, vbuffer, vmaskBITS, Vset, VMset, globalparams);
			#endif
			#ifdef _DEBUGMODE_CHECKS3
			PROCESS_SPL_debug(2, i, E, ens, mask, udataset, maskset, Vset, VMset, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
			#endif 
			
			bool_type ensSTD0 = ON; if(E[0] == INVALIDDATA || lsrcvidsSTD0 >= validbound){ ensSTD0 = OFF; } // STD0.
			bool_type ensSTD1 = ON; if(E[1] == INVALIDDATA || lsrcvidsSTD1 >= validbound){ ensSTD1 = OFF; } // STD1.
			ens[0] = ON; if(E[setoffset + 0] == INVALIDDATA || lsrcvids[0] >= validbound){ ens[0] = OFF; }
			ens[1] = ON; if(E[setoffset + 1] == INVALIDDATA || lsrcvids[1] >= validbound){ ens[1] = OFF; }
			ens[2] = ON; if(E[setoffset + 2] == INVALIDDATA || lsrcvids[2] >= validbound){ ens[2] = OFF; }
			ens[3] = ON; if(E[setoffset + 3] == INVALIDDATA || lsrcvids[3] >= validbound){ ens[3] = OFF; }
			ens[4] = ON; if(E[setoffset + 4] == INVALIDDATA || lsrcvids[4] >= validbound){ ens[4] = OFF; }
			ens[5] = ON; if(E[setoffset + 5] == INVALIDDATA || lsrcvids[5] >= validbound){ ens[5] = OFF; }
			ens[6] = ON; if(E[setoffset + 6] == INVALIDDATA || lsrcvids[6] >= validbound){ ens[6] = OFF; }
			ens[7] = ON; if(E[setoffset + 7] == INVALIDDATA || lsrcvids[7] >= validbound){ ens[7] = OFF; }
			ens[8] = ON; if(E[setoffset + 8] == INVALIDDATA || lsrcvids[8] >= validbound){ ens[8] = OFF; }
			ens[9] = ON; if(E[setoffset + 9] == INVALIDDATA || lsrcvids[9] >= validbound){ ens[9] = OFF; }
			ens[10] = ON; if(E[setoffset + 10] == INVALIDDATA || lsrcvids[10] >= validbound){ ens[10] = OFF; }
			ens[11] = ON; if(E[setoffset + 11] == INVALIDDATA || lsrcvids[11] >= validbound){ ens[11] = OFF; }
			ens[12] = ON; if(E[setoffset + 12] == INVALIDDATA || lsrcvids[12] >= validbound){ ens[12] = OFF; }
			ens[13] = ON; if(E[setoffset + 13] == INVALIDDATA || lsrcvids[13] >= validbound){ ens[13] = OFF; }
			ens[14] = ON; if(E[setoffset + 14] == INVALIDDATA || lsrcvids[14] >= validbound){ ens[14] = OFF; }
			ens[15] = ON; if(E[setoffset + 15] == INVALIDDATA || lsrcvids[15] >= validbound){ ens[15] = OFF; }
			#ifdef _DEBUGMODE_CHECKS3
			PROCESS_SPL_debug(3, i, E, ens, mask, udataset, maskset, Vset, VMset, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
			#endif 
			
			unsigned int udataSTD0 = 0; unsigned int maskSTD0 = 0; // STD0.
			unsigned int udataSTD1 = 0; unsigned int maskSTD1 = 0; // STD1.
 mask[0] = 0;  mask[1] = 0;  mask[2] = 0;  mask[3] = 0;  mask[4] = 0;  mask[5] = 0;  mask[6] = 0;  mask[7] = 0;  mask[8] = 0;  mask[9] = 0;  mask[10] = 0;  mask[11] = 0;  mask[12] = 0;  mask[13] = 0;  mask[14] = 0;  mask[15] = 0; 			if(GraphAlgo == PAGERANK){  mask[0] = 1;  mask[1] = 1;  mask[2] = 1;  mask[3] = 1;  mask[4] = 1;  mask[5] = 1;  mask[6] = 1;  mask[7] = 1;  mask[8] = 1;  mask[9] = 1;  mask[10] = 1;  mask[11] = 1;  mask[12] = 1;  mask[13] = 1;  mask[14] = 1;  mask[15] = 1;  } 
			else {
				udataSTD0 = Vset[offset + incrSTD0]; // STD0.
				maskSTD0 = VMset[offset + incrSTD0]; // STD0.
				udataSTD1 = Vset[offset + incrSTD1]; // STD1.
				maskSTD1 = VMset[offset + incrSTD1]; // STD1.
				udata[0] = Vset[offset + incr[0]]; 
				mask[0] = VMset[offset + incr[0]];
				udata[1] = Vset[offset + incr[1]]; 
				mask[1] = VMset[offset + incr[1]];
				udata[2] = Vset[offset + incr[2]]; 
				mask[2] = VMset[offset + incr[2]];
				udata[3] = Vset[offset + incr[3]]; 
				mask[3] = VMset[offset + incr[3]];
				udata[4] = Vset[offset + incr[4]]; 
				mask[4] = VMset[offset + incr[4]];
				udata[5] = Vset[offset + incr[5]]; 
				mask[5] = VMset[offset + incr[5]];
				udata[6] = Vset[offset + incr[6]]; 
				mask[6] = VMset[offset + incr[6]];
				udata[7] = Vset[offset + incr[7]]; 
				mask[7] = VMset[offset + incr[7]];
				udata[8] = Vset[offset + incr[8]]; 
				mask[8] = VMset[offset + incr[8]];
				udata[9] = Vset[offset + incr[9]]; 
				mask[9] = VMset[offset + incr[9]];
				udata[10] = Vset[offset + incr[10]]; 
				mask[10] = VMset[offset + incr[10]];
				udata[11] = Vset[offset + incr[11]]; 
				mask[11] = VMset[offset + incr[11]];
				udata[12] = Vset[offset + incr[12]]; 
				mask[12] = VMset[offset + incr[12]];
				udata[13] = Vset[offset + incr[13]]; 
				mask[13] = VMset[offset + incr[13]];
				udata[14] = Vset[offset + incr[14]]; 
				mask[14] = VMset[offset + incr[14]];
				udata[15] = Vset[offset + incr[15]]; 
				mask[15] = VMset[offset + incr[15]];
			}
			maskSTD0 = 0; // STD0
			ensSTD0 = OFF; // STD0
			#ifdef _DEBUGMODE_CHECKS3
			PROCESS_SPL_debug(4, i, E, ens, mask, udataset, maskset, Vset, VMset, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
			#endif
			
			res[0] = PROCESS_SPL_processfunc(udata[0], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			res[1] = PROCESS_SPL_processfunc(udata[1], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			res[2] = PROCESS_SPL_processfunc(udata[2], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			res[3] = PROCESS_SPL_processfunc(udata[3], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			res[4] = PROCESS_SPL_processfunc(udata[4], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			res[5] = PROCESS_SPL_processfunc(udata[5], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			res[6] = PROCESS_SPL_processfunc(udata[6], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			res[7] = PROCESS_SPL_processfunc(udata[7], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			res[8] = PROCESS_SPL_processfunc(udata[8], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			res[9] = PROCESS_SPL_processfunc(udata[9], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			res[10] = PROCESS_SPL_processfunc(udata[10], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			res[11] = PROCESS_SPL_processfunc(udata[11], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			res[12] = PROCESS_SPL_processfunc(udata[12], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			res[13] = PROCESS_SPL_processfunc(udata[13], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			res[14] = PROCESS_SPL_processfunc(udata[14], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			res[15] = PROCESS_SPL_processfunc(udata[15], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			if(ens[0] == ON && mask[0] == 1){ mykeyvalue[0].key = ldstvids[0]; mykeyvalue[0].value = res[0]; } else { mykeyvalue[0].key = INVALIDDATA; mykeyvalue[0].value = INVALIDDATA;  }
			if(ens[1] == ON && mask[1] == 1){ mykeyvalue[1].key = ldstvids[1]; mykeyvalue[1].value = res[1]; } else { mykeyvalue[1].key = INVALIDDATA; mykeyvalue[1].value = INVALIDDATA;  }
			if(ens[2] == ON && mask[2] == 1){ mykeyvalue[2].key = ldstvids[2]; mykeyvalue[2].value = res[2]; } else { mykeyvalue[2].key = INVALIDDATA; mykeyvalue[2].value = INVALIDDATA;  }
			if(ens[3] == ON && mask[3] == 1){ mykeyvalue[3].key = ldstvids[3]; mykeyvalue[3].value = res[3]; } else { mykeyvalue[3].key = INVALIDDATA; mykeyvalue[3].value = INVALIDDATA;  }
			if(ens[4] == ON && mask[4] == 1){ mykeyvalue[4].key = ldstvids[4]; mykeyvalue[4].value = res[4]; } else { mykeyvalue[4].key = INVALIDDATA; mykeyvalue[4].value = INVALIDDATA;  }
			if(ens[5] == ON && mask[5] == 1){ mykeyvalue[5].key = ldstvids[5]; mykeyvalue[5].value = res[5]; } else { mykeyvalue[5].key = INVALIDDATA; mykeyvalue[5].value = INVALIDDATA;  }
			if(ens[6] == ON && mask[6] == 1){ mykeyvalue[6].key = ldstvids[6]; mykeyvalue[6].value = res[6]; } else { mykeyvalue[6].key = INVALIDDATA; mykeyvalue[6].value = INVALIDDATA;  }
			if(ens[7] == ON && mask[7] == 1){ mykeyvalue[7].key = ldstvids[7]; mykeyvalue[7].value = res[7]; } else { mykeyvalue[7].key = INVALIDDATA; mykeyvalue[7].value = INVALIDDATA;  }
			if(ens[8] == ON && mask[8] == 1){ mykeyvalue[8].key = ldstvids[8]; mykeyvalue[8].value = res[8]; } else { mykeyvalue[8].key = INVALIDDATA; mykeyvalue[8].value = INVALIDDATA;  }
			if(ens[9] == ON && mask[9] == 1){ mykeyvalue[9].key = ldstvids[9]; mykeyvalue[9].value = res[9]; } else { mykeyvalue[9].key = INVALIDDATA; mykeyvalue[9].value = INVALIDDATA;  }
			if(ens[10] == ON && mask[10] == 1){ mykeyvalue[10].key = ldstvids[10]; mykeyvalue[10].value = res[10]; } else { mykeyvalue[10].key = INVALIDDATA; mykeyvalue[10].value = INVALIDDATA;  }
			if(ens[11] == ON && mask[11] == 1){ mykeyvalue[11].key = ldstvids[11]; mykeyvalue[11].value = res[11]; } else { mykeyvalue[11].key = INVALIDDATA; mykeyvalue[11].value = INVALIDDATA;  }
			if(ens[12] == ON && mask[12] == 1){ mykeyvalue[12].key = ldstvids[12]; mykeyvalue[12].value = res[12]; } else { mykeyvalue[12].key = INVALIDDATA; mykeyvalue[12].value = INVALIDDATA;  }
			if(ens[13] == ON && mask[13] == 1){ mykeyvalue[13].key = ldstvids[13]; mykeyvalue[13].value = res[13]; } else { mykeyvalue[13].key = INVALIDDATA; mykeyvalue[13].value = INVALIDDATA;  }
			if(ens[14] == ON && mask[14] == 1){ mykeyvalue[14].key = ldstvids[14]; mykeyvalue[14].value = res[14]; } else { mykeyvalue[14].key = INVALIDDATA; mykeyvalue[14].value = INVALIDDATA;  }
			if(ens[15] == ON && mask[15] == 1){ mykeyvalue[15].key = ldstvids[15]; mykeyvalue[15].value = res[15]; } else { mykeyvalue[15].key = INVALIDDATA; mykeyvalue[15].value = INVALIDDATA;  }
			#ifdef _DEBUGMODE_CHECKS3
			PROCESS_SPL_debug(5, i, E, ens, mask, udataset, maskset, Vset, VMset, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
			#endif 
			
			#if PE_SETSZ==8
			buffer[0][loadcount] = UTIL_GETKV(mykeyvalue[0]);
			buffer[1][loadcount] = UTIL_GETKV(mykeyvalue[1]);
			buffer[2][loadcount] = UTIL_GETKV(mykeyvalue[2]);
			buffer[3][loadcount] = UTIL_GETKV(mykeyvalue[3]);
			buffer[4][loadcount] = UTIL_GETKV(mykeyvalue[4]);
			buffer[5][loadcount] = UTIL_GETKV(mykeyvalue[5]);
			buffer[6][loadcount] = UTIL_GETKV(mykeyvalue[6]);
			buffer[7][loadcount] = UTIL_GETKV(mykeyvalue[7]);
	
			if(setoffset == 0){ buffer[0][loadcount] = UTIL_GETKV(dummykeyvalue); } // STD0
			if(ensSTD1 == ON && maskSTD1 == 1){ loadcount += 1; } 
			#else
			buffer[0][loadcount] = UTIL_GETKV(mykeyvalue[0]);
			buffer[1][loadcount] = UTIL_GETKV(mykeyvalue[1]);
			buffer[2][loadcount] = UTIL_GETKV(mykeyvalue[2]);
			buffer[3][loadcount] = UTIL_GETKV(mykeyvalue[3]);
			buffer[4][loadcount] = UTIL_GETKV(mykeyvalue[4]);
			buffer[5][loadcount] = UTIL_GETKV(mykeyvalue[5]);
			buffer[6][loadcount] = UTIL_GETKV(mykeyvalue[6]);
			buffer[7][loadcount] = UTIL_GETKV(mykeyvalue[7]);
	
			buffer[0][loadcount + 1] = UTIL_GETKV(mykeyvalue[8]);
			buffer[1][loadcount + 1] = UTIL_GETKV(mykeyvalue[9]);
			buffer[2][loadcount + 1] = UTIL_GETKV(mykeyvalue[10]);
			buffer[3][loadcount + 1] = UTIL_GETKV(mykeyvalue[11]);
			buffer[4][loadcount + 1] = UTIL_GETKV(mykeyvalue[12]);
			buffer[5][loadcount + 1] = UTIL_GETKV(mykeyvalue[13]);
			buffer[6][loadcount + 1] = UTIL_GETKV(mykeyvalue[14]);
			buffer[7][loadcount + 1] = UTIL_GETKV(mykeyvalue[15]);
			buffer[0][loadcount] = UTIL_GETKV(dummykeyvalue); // STD0
			if(ensSTD1 == ON && maskSTD1 == 1){ loadcount += 2; }
			#endif
	
			#ifdef _DEBUGMODE_CHECKS3
			PROCESS_SPL_debug(6, i, E, ens, mask, udataset, maskset, Vset, VMset, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
			#endif
		}
	}
	// cout<<">>> readandprocess: loadcount: "<<loadcount<<", activeloadcount: "<<activeloadcount<<", inactiveloadcount: "<<inactiveloadcount<<endl;
	// exit(EXIT_SUCCESS); ///
	fetchmessage.chunksize_kvs = loadcount;
	return fetchmessage;
}



