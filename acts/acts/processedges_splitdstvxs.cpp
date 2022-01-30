#include "processedges_splitdstvxs.h"
using namespace std;

// => VDATA (to read)
// 16:[0,32,...,480] | [1,33,...,481] | ... | [31,63,...,511]

// => VMASK (to read)
// 32:[0,32,...,480,...,992] | [1,33,...,481,...,993] | ... | [31,63,...,511,...,1023]

// int programglobalvar_numvtxsprocessed = 0; // program global variables

#ifdef SW
processedges_splitdstvxs::processedges_splitdstvxs(mydebug * _mydebugobj){ 
	actsutilityobj = new actsutility(); 
	acts_utilobj = new acts_util(_mydebugobj); 
	mem_accessobj = new mem_access(_mydebugobj);
	mydebugobj = _mydebugobj;
}
processedges_splitdstvxs::~processedges_splitdstvxs(){}
#endif

//
value_t acts_all::PROCESS_SPL_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo){
	value_t res = 0;
	/* if(GraphAlgo == PAGERANK){
		res = udata;
	} else if(GraphAlgo == BFS){
		res = NAp;
	} else if(GraphAlgo == SSSP){
		res = udata + edgew;
	} else {
		res = NAp;
	} */
	
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
		value_t udataset[MAX_NUM_UNIQ_EDGES_PER_VEC], value_t maskset[VECTOR2_SIZE], value_t Vset[2][VECTOR2_SIZE], unit1_type VMset[2][VECTOR2_SIZE], vertex_t lvids[VECTOR2_SIZE],
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
		if(ens[0] == ON && mask[0] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[0].2. i: "<<i<<", mask[0]: "<<mask[0]<<", incr[0]: "<<incr[0]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[0]: "<<mask[0]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[0]: "<<mask[0]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			exit(EXIT_FAILURE); 
		}
		if(ens[1] == ON && mask[1] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[1].2. i: "<<i<<", mask[1]: "<<mask[1]<<", incr[1]: "<<incr[1]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[1]: "<<mask[1]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[1]: "<<mask[1]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			exit(EXIT_FAILURE); 
		}
		if(ens[2] == ON && mask[2] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[2].2. i: "<<i<<", mask[2]: "<<mask[2]<<", incr[2]: "<<incr[2]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[2]: "<<mask[2]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[2]: "<<mask[2]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			exit(EXIT_FAILURE); 
		}
		if(ens[3] == ON && mask[3] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[3].2. i: "<<i<<", mask[3]: "<<mask[3]<<", incr[3]: "<<incr[3]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[3]: "<<mask[3]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[3]: "<<mask[3]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			exit(EXIT_FAILURE); 
		}
		if(ens[4] == ON && mask[4] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[4].2. i: "<<i<<", mask[4]: "<<mask[4]<<", incr[4]: "<<incr[4]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[4]: "<<mask[4]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[4]: "<<mask[4]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			exit(EXIT_FAILURE); 
		}
		if(ens[5] == ON && mask[5] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[5].2. i: "<<i<<", mask[5]: "<<mask[5]<<", incr[5]: "<<incr[5]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[5]: "<<mask[5]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[5]: "<<mask[5]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			exit(EXIT_FAILURE); 
		}
		if(ens[6] == ON && mask[6] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[6].2. i: "<<i<<", mask[6]: "<<mask[6]<<", incr[6]: "<<incr[6]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[6]: "<<mask[6]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[6]: "<<mask[6]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			exit(EXIT_FAILURE); 
		}
		if(ens[7] == ON && mask[7] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[7].2. i: "<<i<<", mask[7]: "<<mask[7]<<", incr[7]: "<<incr[7]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[7]: "<<mask[7]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[7]: "<<mask[7]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			exit(EXIT_FAILURE); 
		}
		if(ens[8] == ON && mask[8] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[8].2. i: "<<i<<", mask[8]: "<<mask[8]<<", incr[8]: "<<incr[8]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[8]: "<<mask[8]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[8]: "<<mask[8]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			exit(EXIT_FAILURE); 
		}
		if(ens[9] == ON && mask[9] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[9].2. i: "<<i<<", mask[9]: "<<mask[9]<<", incr[9]: "<<incr[9]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[9]: "<<mask[9]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[9]: "<<mask[9]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			exit(EXIT_FAILURE); 
		}
		if(ens[10] == ON && mask[10] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[10].2. i: "<<i<<", mask[10]: "<<mask[10]<<", incr[10]: "<<incr[10]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[10]: "<<mask[10]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[10]: "<<mask[10]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			exit(EXIT_FAILURE); 
		}
		if(ens[11] == ON && mask[11] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[11].2. i: "<<i<<", mask[11]: "<<mask[11]<<", incr[11]: "<<incr[11]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[11]: "<<mask[11]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[11]: "<<mask[11]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			exit(EXIT_FAILURE); 
		}
		if(ens[12] == ON && mask[12] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[12].2. i: "<<i<<", mask[12]: "<<mask[12]<<", incr[12]: "<<incr[12]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[12]: "<<mask[12]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[12]: "<<mask[12]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			exit(EXIT_FAILURE); 
		}
		if(ens[13] == ON && mask[13] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[13].2. i: "<<i<<", mask[13]: "<<mask[13]<<", incr[13]: "<<incr[13]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[13]: "<<mask[13]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[13]: "<<mask[13]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			exit(EXIT_FAILURE); 
		}
		if(ens[14] == ON && mask[14] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[14].2. i: "<<i<<", mask[14]: "<<mask[14]<<", incr[14]: "<<incr[14]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[14]: "<<mask[14]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[14]: "<<mask[14]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
			exit(EXIT_FAILURE); 
		}
		if(ens[15] == ON && mask[15] > 2){ 
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::mask[15].2. i: "<<i<<", mask[15]: "<<mask[15]<<", incr[15]: "<<incr[15]<<endl;
 cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[0]: "<<E[0]<<", lvid_head: "<<lvid_head<<", lsrcvids[0]: "<<lsrcvids[0]<<", ldstvids[0]: "<<ldstvids[0]<<", incr[0]: "<<incr[0]<<", mask[15]: "<<mask[15]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[1]: "<<E[1]<<", lvid_head: "<<lvid_head<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", incr[1]: "<<incr[1]<<", mask[15]: "<<mask[15]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[2]: "<<E[2]<<", lvid_head: "<<lvid_head<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", incr[2]: "<<incr[2]<<", mask[15]: "<<mask[15]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[3]: "<<E[3]<<", lvid_head: "<<lvid_head<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", incr[3]: "<<incr[3]<<", mask[15]: "<<mask[15]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[4]: "<<E[4]<<", lvid_head: "<<lvid_head<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", incr[4]: "<<incr[4]<<", mask[15]: "<<mask[15]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[5]: "<<E[5]<<", lvid_head: "<<lvid_head<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", incr[5]: "<<incr[5]<<", mask[15]: "<<mask[15]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[6]: "<<E[6]<<", lvid_head: "<<lvid_head<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", incr[6]: "<<incr[6]<<", mask[15]: "<<mask[15]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[7]: "<<E[7]<<", lvid_head: "<<lvid_head<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", incr[7]: "<<incr[7]<<", mask[15]: "<<mask[15]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[8]: "<<E[8]<<", lvid_head: "<<lvid_head<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", incr[8]: "<<incr[8]<<", mask[15]: "<<mask[15]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[9]: "<<E[9]<<", lvid_head: "<<lvid_head<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", incr[9]: "<<incr[9]<<", mask[15]: "<<mask[15]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[10]: "<<E[10]<<", lvid_head: "<<lvid_head<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", incr[10]: "<<incr[10]<<", mask[15]: "<<mask[15]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[11]: "<<E[11]<<", lvid_head: "<<lvid_head<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", incr[11]: "<<incr[11]<<", mask[15]: "<<mask[15]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[12]: "<<E[12]<<", lvid_head: "<<lvid_head<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", incr[12]: "<<incr[12]<<", mask[15]: "<<mask[15]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[13]: "<<E[13]<<", lvid_head: "<<lvid_head<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", incr[13]: "<<incr[13]<<", mask[15]: "<<mask[15]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[14]: "<<E[14]<<", lvid_head: "<<lvid_head<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", incr[14]: "<<incr[14]<<", mask[15]: "<<mask[15]<<endl;  cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: E[15]: "<<E[15]<<", lvid_head: "<<lvid_head<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", incr[15]: "<<incr[15]<<", mask[15]: "<<mask[15]<<endl;  
			for(unsigned int r=0; r<MAX_NUM_UNIQ_EDGES_PER_VEC; r++){ cout<<">>> readandprocess(14)::DEBUG CODE 4:: mask: udataset["<<r<<"]: "<<udataset[r]<<", maskset["<<r<<"]: "<<maskset[r]<<endl; }
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[0].beginoffset: "<<sliceinfos.slice[0].beginoffset<<", sliceinfos.slice[0].endoffset: "<<sliceinfos.slice[0].endoffset<<endl;
			cout<<"ERROR @ readandprocess(14)::DEBUG CODE 4::. sliceinfos.slice[1].beginoffset: "<<sliceinfos.slice[1].beginoffset<<", sliceinfos.slice[1].endoffset: "<<sliceinfos.slice[1].endoffset<<endl;
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

void acts_all::PROCESS_SPL_getslice(sliceinfo_t slice, int id, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[VECTOR2_SIZE], unit1_type VMdatas[VECTOR2_SIZE], unsigned int depths[16], globalparams_t globalparams){
	// if(slice.active == false){ return; }
	int s_base_row = slice.s_base * (globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs);
	
	int s_0 = slice.s_base + 0;
	bool en0=false; if(0>=slice.beginoffset && 0<slice.endoffset){ en0 = true; } else { en0 = false; }
	int rowoffseti0 = 0; if(en0==true){ rowoffseti0 = s_base_row + depths[0]; } 
	// int rowoffseti0 = s_base_row + depths[0];
	unsigned int row0 = rowoffseti0 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en0==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row0", row0, DOUBLE_BLOCKRAM_SIZE, rowoffseti0, slice.local_rowoffset, NAp); }
	#endif 
	int s_1 = slice.s_base + 1;
	bool en1=false; if(1>=slice.beginoffset && 1<slice.endoffset){ en1 = true; } else { en1 = false; }
	int rowoffseti1 = 0; if(en1==true){ rowoffseti1 = s_base_row + depths[1]; } 
	// int rowoffseti1 = s_base_row + depths[1];
	unsigned int row1 = rowoffseti1 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en1==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row1", row1, DOUBLE_BLOCKRAM_SIZE, rowoffseti1, slice.local_rowoffset, NAp); }
	#endif 
	int s_2 = slice.s_base + 2;
	bool en2=false; if(2>=slice.beginoffset && 2<slice.endoffset){ en2 = true; } else { en2 = false; }
	int rowoffseti2 = 0; if(en2==true){ rowoffseti2 = s_base_row + depths[2]; } 
	// int rowoffseti2 = s_base_row + depths[2];
	unsigned int row2 = rowoffseti2 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en2==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row2", row2, DOUBLE_BLOCKRAM_SIZE, rowoffseti2, slice.local_rowoffset, NAp); }
	#endif 
	int s_3 = slice.s_base + 3;
	bool en3=false; if(3>=slice.beginoffset && 3<slice.endoffset){ en3 = true; } else { en3 = false; }
	int rowoffseti3 = 0; if(en3==true){ rowoffseti3 = s_base_row + depths[3]; } 
	// int rowoffseti3 = s_base_row + depths[3];
	unsigned int row3 = rowoffseti3 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en3==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row3", row3, DOUBLE_BLOCKRAM_SIZE, rowoffseti3, slice.local_rowoffset, NAp); }
	#endif 
	int s_4 = slice.s_base + 4;
	bool en4=false; if(4>=slice.beginoffset && 4<slice.endoffset){ en4 = true; } else { en4 = false; }
	int rowoffseti4 = 0; if(en4==true){ rowoffseti4 = s_base_row + depths[4]; } 
	// int rowoffseti4 = s_base_row + depths[4];
	unsigned int row4 = rowoffseti4 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en4==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row4", row4, DOUBLE_BLOCKRAM_SIZE, rowoffseti4, slice.local_rowoffset, NAp); }
	#endif 
	int s_5 = slice.s_base + 5;
	bool en5=false; if(5>=slice.beginoffset && 5<slice.endoffset){ en5 = true; } else { en5 = false; }
	int rowoffseti5 = 0; if(en5==true){ rowoffseti5 = s_base_row + depths[5]; } 
	// int rowoffseti5 = s_base_row + depths[5];
	unsigned int row5 = rowoffseti5 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en5==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row5", row5, DOUBLE_BLOCKRAM_SIZE, rowoffseti5, slice.local_rowoffset, NAp); }
	#endif 
	int s_6 = slice.s_base + 6;
	bool en6=false; if(6>=slice.beginoffset && 6<slice.endoffset){ en6 = true; } else { en6 = false; }
	int rowoffseti6 = 0; if(en6==true){ rowoffseti6 = s_base_row + depths[6]; } 
	// int rowoffseti6 = s_base_row + depths[6];
	unsigned int row6 = rowoffseti6 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en6==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row6", row6, DOUBLE_BLOCKRAM_SIZE, rowoffseti6, slice.local_rowoffset, NAp); }
	#endif 
	int s_7 = slice.s_base + 7;
	bool en7=false; if(7>=slice.beginoffset && 7<slice.endoffset){ en7 = true; } else { en7 = false; }
	int rowoffseti7 = 0; if(en7==true){ rowoffseti7 = s_base_row + depths[7]; } 
	// int rowoffseti7 = s_base_row + depths[7];
	unsigned int row7 = rowoffseti7 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en7==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row7", row7, DOUBLE_BLOCKRAM_SIZE, rowoffseti7, slice.local_rowoffset, NAp); }
	#endif 
	int s_8 = slice.s_base + 8;
	bool en8=false; if(8>=slice.beginoffset && 8<slice.endoffset){ en8 = true; } else { en8 = false; }
	int rowoffseti8 = 0; if(en8==true){ rowoffseti8 = s_base_row + depths[8]; } 
	// int rowoffseti8 = s_base_row + depths[8];
	unsigned int row8 = rowoffseti8 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en8==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row8", row8, DOUBLE_BLOCKRAM_SIZE, rowoffseti8, slice.local_rowoffset, NAp); }
	#endif 
	int s_9 = slice.s_base + 9;
	bool en9=false; if(9>=slice.beginoffset && 9<slice.endoffset){ en9 = true; } else { en9 = false; }
	int rowoffseti9 = 0; if(en9==true){ rowoffseti9 = s_base_row + depths[9]; } 
	// int rowoffseti9 = s_base_row + depths[9];
	unsigned int row9 = rowoffseti9 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en9==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row9", row9, DOUBLE_BLOCKRAM_SIZE, rowoffseti9, slice.local_rowoffset, NAp); }
	#endif 
	int s_10 = slice.s_base + 10;
	bool en10=false; if(10>=slice.beginoffset && 10<slice.endoffset){ en10 = true; } else { en10 = false; }
	int rowoffseti10 = 0; if(en10==true){ rowoffseti10 = s_base_row + depths[10]; } 
	// int rowoffseti10 = s_base_row + depths[10];
	unsigned int row10 = rowoffseti10 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en10==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row10", row10, DOUBLE_BLOCKRAM_SIZE, rowoffseti10, slice.local_rowoffset, NAp); }
	#endif 
	int s_11 = slice.s_base + 11;
	bool en11=false; if(11>=slice.beginoffset && 11<slice.endoffset){ en11 = true; } else { en11 = false; }
	int rowoffseti11 = 0; if(en11==true){ rowoffseti11 = s_base_row + depths[11]; } 
	// int rowoffseti11 = s_base_row + depths[11];
	unsigned int row11 = rowoffseti11 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en11==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row11", row11, DOUBLE_BLOCKRAM_SIZE, rowoffseti11, slice.local_rowoffset, NAp); }
	#endif 
	int s_12 = slice.s_base + 12;
	bool en12=false; if(12>=slice.beginoffset && 12<slice.endoffset){ en12 = true; } else { en12 = false; }
	int rowoffseti12 = 0; if(en12==true){ rowoffseti12 = s_base_row + depths[12]; } 
	// int rowoffseti12 = s_base_row + depths[12];
	unsigned int row12 = rowoffseti12 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en12==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row12", row12, DOUBLE_BLOCKRAM_SIZE, rowoffseti12, slice.local_rowoffset, NAp); }
	#endif 
	int s_13 = slice.s_base + 13;
	bool en13=false; if(13>=slice.beginoffset && 13<slice.endoffset){ en13 = true; } else { en13 = false; }
	int rowoffseti13 = 0; if(en13==true){ rowoffseti13 = s_base_row + depths[13]; } 
	// int rowoffseti13 = s_base_row + depths[13];
	unsigned int row13 = rowoffseti13 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en13==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row13", row13, DOUBLE_BLOCKRAM_SIZE, rowoffseti13, slice.local_rowoffset, NAp); }
	#endif 
	int s_14 = slice.s_base + 14;
	bool en14=false; if(14>=slice.beginoffset && 14<slice.endoffset){ en14 = true; } else { en14 = false; }
	int rowoffseti14 = 0; if(en14==true){ rowoffseti14 = s_base_row + depths[14]; } 
	// int rowoffseti14 = s_base_row + depths[14];
	unsigned int row14 = rowoffseti14 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en14==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row14", row14, DOUBLE_BLOCKRAM_SIZE, rowoffseti14, slice.local_rowoffset, NAp); }
	#endif 
	int s_15 = slice.s_base + 15;
	bool en15=false; if(15>=slice.beginoffset && 15<slice.endoffset){ en15 = true; } else { en15 = false; }
	int rowoffseti15 = 0; if(en15==true){ rowoffseti15 = s_base_row + depths[15]; } 
	// int rowoffseti15 = s_base_row + depths[15];
	unsigned int row15 = rowoffseti15 + slice.local_rowoffset;
	#ifdef _DEBUGMODE_CHECKS2
	if(en15==true){ actsutilityobj->checkoutofbounds("PROCESS_SPL_getslicee. row15", row15, DOUBLE_BLOCKRAM_SIZE, rowoffseti15, slice.local_rowoffset, NAp); }
	#endif 
	
	VMdatas[0] = vmaskBITS[0][row0];	
	keyvalue_t V0; if(en0==true){ V0 = UTIL_GETKV2(vbuffer[0][row0/2]); }
	if(en0==true){ if(s_0%2==0){ Vdatas[0] = V0.key; } else { Vdatas[0] = V0.value; }}
	VMdatas[1] = vmaskBITS[1][row1];	
	keyvalue_t V1; if(en1==true){ V1 = UTIL_GETKV2(vbuffer[1][row1/2]); }
	if(en1==true){ if(s_1%2==0){ Vdatas[1] = V1.key; } else { Vdatas[1] = V1.value; }}
	VMdatas[2] = vmaskBITS[2][row2];	
	keyvalue_t V2; if(en2==true){ V2 = UTIL_GETKV2(vbuffer[2][row2/2]); }
	if(en2==true){ if(s_2%2==0){ Vdatas[2] = V2.key; } else { Vdatas[2] = V2.value; }}
	VMdatas[3] = vmaskBITS[3][row3];	
	keyvalue_t V3; if(en3==true){ V3 = UTIL_GETKV2(vbuffer[3][row3/2]); }
	if(en3==true){ if(s_3%2==0){ Vdatas[3] = V3.key; } else { Vdatas[3] = V3.value; }}
	VMdatas[4] = vmaskBITS[4][row4];	
	keyvalue_t V4; if(en4==true){ V4 = UTIL_GETKV2(vbuffer[4][row4/2]); }
	if(en4==true){ if(s_4%2==0){ Vdatas[4] = V4.key; } else { Vdatas[4] = V4.value; }}
	VMdatas[5] = vmaskBITS[5][row5];	
	keyvalue_t V5; if(en5==true){ V5 = UTIL_GETKV2(vbuffer[5][row5/2]); }
	if(en5==true){ if(s_5%2==0){ Vdatas[5] = V5.key; } else { Vdatas[5] = V5.value; }}
	VMdatas[6] = vmaskBITS[6][row6];	
	keyvalue_t V6; if(en6==true){ V6 = UTIL_GETKV2(vbuffer[6][row6/2]); }
	if(en6==true){ if(s_6%2==0){ Vdatas[6] = V6.key; } else { Vdatas[6] = V6.value; }}
	VMdatas[7] = vmaskBITS[7][row7];	
	keyvalue_t V7; if(en7==true){ V7 = UTIL_GETKV2(vbuffer[7][row7/2]); }
	if(en7==true){ if(s_7%2==0){ Vdatas[7] = V7.key; } else { Vdatas[7] = V7.value; }}
	VMdatas[8] = vmaskBITS[8][row8];	
	keyvalue_t V8; if(en8==true){ V8 = UTIL_GETKV2(vbuffer[8][row8/2]); }
	if(en8==true){ if(s_8%2==0){ Vdatas[8] = V8.key; } else { Vdatas[8] = V8.value; }}
	VMdatas[9] = vmaskBITS[9][row9];	
	keyvalue_t V9; if(en9==true){ V9 = UTIL_GETKV2(vbuffer[9][row9/2]); }
	if(en9==true){ if(s_9%2==0){ Vdatas[9] = V9.key; } else { Vdatas[9] = V9.value; }}
	VMdatas[10] = vmaskBITS[10][row10];	
	keyvalue_t V10; if(en10==true){ V10 = UTIL_GETKV2(vbuffer[10][row10/2]); }
	if(en10==true){ if(s_10%2==0){ Vdatas[10] = V10.key; } else { Vdatas[10] = V10.value; }}
	VMdatas[11] = vmaskBITS[11][row11];	
	keyvalue_t V11; if(en11==true){ V11 = UTIL_GETKV2(vbuffer[11][row11/2]); }
	if(en11==true){ if(s_11%2==0){ Vdatas[11] = V11.key; } else { Vdatas[11] = V11.value; }}
	VMdatas[12] = vmaskBITS[12][row12];	
	keyvalue_t V12; if(en12==true){ V12 = UTIL_GETKV2(vbuffer[12][row12/2]); }
	if(en12==true){ if(s_12%2==0){ Vdatas[12] = V12.key; } else { Vdatas[12] = V12.value; }}
	VMdatas[13] = vmaskBITS[13][row13];	
	keyvalue_t V13; if(en13==true){ V13 = UTIL_GETKV2(vbuffer[13][row13/2]); }
	if(en13==true){ if(s_13%2==0){ Vdatas[13] = V13.key; } else { Vdatas[13] = V13.value; }}
	VMdatas[14] = vmaskBITS[14][row14];	
	keyvalue_t V14; if(en14==true){ V14 = UTIL_GETKV2(vbuffer[14][row14/2]); }
	if(en14==true){ if(s_14%2==0){ Vdatas[14] = V14.key; } else { Vdatas[14] = V14.value; }}
	VMdatas[15] = vmaskBITS[15][row15];	
	keyvalue_t V15; if(en15==true){ V15 = UTIL_GETKV2(vbuffer[15][row15/2]); }
	if(en15==true){ if(s_15%2==0){ Vdatas[15] = V15.key; } else { Vdatas[15] = V15.value; }}
	return;
}

//
value_t acts_all::PROCESS_SPL_GETVTXDATA(keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
	#pragma HLS INLINE
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<">>> PROCESS_SPL_GETVTXDATA: ERROR DETECTED (23). loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE("<<globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE<<"). loc: "<<loc<<endl;
		#endif 
		loc = 0; }
	
	unsigned int instid = loc % NUM_PEs;
	unsigned int lloc = UTIL_GETLOCALVID(loc, instid);
	unsigned int rowoffset = instid * (globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs); // TOO EXPENSIVE.
	
	unsigned int col = lloc % 16;
	unsigned int row = lloc / 16;
	unsigned int realcol = col;
	unsigned int realrow = (rowoffset + row) / 2;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<">>> PROCESS_SPL_GETVTXDATA: loc: "<<loc<<", instid: "<<instid<<", lloc: "<<lloc<<", col: "<<col<<", row: "<<row<<", realrow: "<<realrow<<", rowoffset: "<<rowoffset<<", (globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs): "<<globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs<<endl;
	#endif 
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("PROCESS_SPL_GETVTXDATA.lloc", lloc, (globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs) * VECTOR2_SIZE, loc, rowoffset, NAp); 
	actsutilityobj->checkoutofbounds("PROCESS_SPL_GETVTXDATA.rowoffset", rowoffset, REDUCEPARTITIONSZ_KVS2, loc, rowoffset, NAp); 
	actsutilityobj->checkoutofbounds("PROCESS_SPL_GETVTXDATA.realcol", realcol, VDATA_PACKINGSIZE, loc, rowoffset, NAp);
	actsutilityobj->checkoutofbounds("PROCESS_SPL_GETVTXDATA.realrow", realrow, BLOCKRAM_SIZE, loc, rowoffset, NAp);
	actsutilityobj->checkoutofbounds("PROCESS_SPL_GETVTXDATA.row", rowoffset + row, REDUCEPARTITIONSZ_KVS2, loc, rowoffset, NAp);
	#endif 
	
	value_t data = 0;
	if(row % 2 == 0){ data = UTIL_GETKV2(vbuffer[realcol][realrow]).key; } 
	else { data = UTIL_GETKV2(vbuffer[realcol][realrow]).value; }
	return data;
}

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

value_t acts_all::PROCESS_SPL_GETVTXDATA_SLIDED(keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
	#pragma HLS INLINE
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<">>> PROCESS_SPL_GETVTXDATA_SLIDED: ERROR DETECTED (23). loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE("<<globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE<<"). loc: "<<loc<<endl;
		#endif 
		loc = 0; }
	
	unsigned int s = loc % NUM_PEs;
	unsigned int lloc = UTIL_GETLOCALVID(loc, s);
	unsigned int rowoffset = s * (globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs); // TOO EXPENSIVE.
	
	unsigned int col = lloc % 16;
	unsigned int row = lloc / 16;
	unsigned int realcol = col;
	unsigned int rrealcol = (s + realcol) % 16;
	unsigned int realrow = (rowoffset + row) / 2;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<">>> PROCESS_SPL_GETVTXDATA_SLIDED: loc: "<<loc<<", instid: "<<s<<", lloc: "<<lloc<<", col: "<<col<<", row: "<<row<<", realrow: "<<realrow<<", rowoffset: "<<rowoffset<<", (globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs): "<<globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs<<endl;
	#endif 
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("PROCESS_SPL_GETVTXDATA_SLIDED.lloc", lloc, (globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs) * VECTOR2_SIZE, loc, rowoffset, NAp); 
	actsutilityobj->checkoutofbounds("PROCESS_SPL_GETVTXDATA_SLIDED.rowoffset", rowoffset, REDUCEPARTITIONSZ_KVS2, loc, rowoffset, NAp); 
	actsutilityobj->checkoutofbounds("PROCESS_SPL_GETVTXDATA_SLIDED.realcol", realcol, VDATA_PACKINGSIZE, loc, rowoffset, NAp);
	actsutilityobj->checkoutofbounds("PROCESS_SPL_GETVTXDATA_SLIDED.rrealcol", rrealcol, VDATA_PACKINGSIZE, loc, rowoffset, NAp);
	actsutilityobj->checkoutofbounds("PROCESS_SPL_GETVTXDATA_SLIDED.realrow", realrow, BLOCKRAM_SIZE, loc, rowoffset, NAp);
	actsutilityobj->checkoutofbounds("PROCESS_SPL_GETVTXDATA_SLIDED.row", rowoffset + row, REDUCEPARTITIONSZ_KVS2, loc, rowoffset, NAp);
	#endif 
	
	value_t data = 0;
	if(row % 2 == 0){ data = UTIL_GETKV2(vbuffer[rrealcol][realrow]).key; } 
	else { data = UTIL_GETKV2(vbuffer[rrealcol][realrow]).value; }
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

void acts_all::PROCESS_SPL_GETVTXDATASET(sliceinfo_t slice, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[VECTOR2_SIZE], unit1_type VMdatas[VECTOR2_SIZE], unsigned int depths[16], globalparams_t globalparams){					
	#pragma HLS INLINE
	PROCESS_SPL_getslice(slice, 0, vbuffer, vmaskBITS, Vdatas, VMdatas, depths, globalparams);
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int t=0; t<VECTOR2_SIZE; t++){ cout<<"PROCESS_SPL_GETVTXDATASET:: VMdatas["<<t<<"]: "<<VMdatas[t]<<endl; }
	for(unsigned int t=0; t<VECTOR2_SIZE; t++){ cout<<"PROCESS_SPL_GETVTXDATASET:: VMdatas["<<t<<"]: "<<VMdatas[t]<<endl; }
	#endif
	return;
}

void acts_all::PROCESS_SPL1_GETVTXDATASET(unsigned int loc, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], unit1_type VMdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], globalparams_t globalparams){
	#pragma HLS INLINE
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<">>> PROCESS_SPL_GETVTXDATA: ERROR DETECTED (23). loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE("<<globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE<<"). loc: "<<loc<<endl;
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
		if(row % 2 == 0){ Vdatas[v] = UTIL_GETKV2(vbuffer[realcol][realrow]).key; } 
		else { Vdatas[v] = UTIL_GETKV2(vbuffer[realcol][realrow]).value; }
		VMdatas[v] = vmaskBITS[realcol][realrow];
		
		VMdatas[v] = vmaskBITS[realcol][realrow];
		keyvalue_t KV = UTIL_GETKV2(vbuffer[realcol][realrow/2]);
		if(realrow%2==0){ Vdatas[v] = KV.key; } else { Vdatas[v] = KV.value; }
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
		VMdatas[v] = vmaskBITS[col_dim16][truerow_dim16];
		keyvalue_t KV = UTIL_GETKV2(vbuffer[col_dim16][truerow_dim16/2]);
		if(truerow_dim16%2==0){ Vdatas[v] = KV.key; } else { Vdatas[v] = KV.value; }
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(VMdatas[v] == 1){ cout<<"PROCESS_SPL2_GETVTXDATASET: MASK=1 FOUND: VMdatas["<<v<<"]: "<<VMdatas[v]<<", vmaskBITS["<<col_dim16<<"]["<<truerow_dim16<<"]: "<<vmaskBITS[col_dim16][truerow_dim16]<<", (loc + v): "<<loc + v<<", loc: "<<loc<<", v: "<<v<<endl; }
		#endif
	}
	return;
}

/** void acts_all::PROCESS_SPL2_GETVTXDATASET(unsigned int loc, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], unit1_type VMdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], globalparams_t globalparams){					
	// if(index >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE){ index = 0; }
	
	//
	int index = 0;
	
	//
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
			
	for(unsigned int v=col_dim16; v<VECTOR2_SIZE; v++){
		if(index < MAX_NUM_UNIQ_EDGES_PER_VEC && loc+v-col_dim16 < (globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE)){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("PROCESS_SPL2_GETVTXDATASET(A).col_dim16", col_dim16, VECTOR2_SIZE, col_dim16, v, loc);
			actsutilityobj->checkoutofbounds("PROCESS_SPL2_GETVTXDATASET(A).truerow_dim16", truerow_dim16, DOUBLE_BLOCKRAM_SIZE, truerow_dim16, v, loc);
			#endif
			VMdatas[index] = vmaskBITS[v][truerow_dim16];
			keyvalue_t KV = UTIL_GETKV2(vbuffer[v][truerow_dim16/2]);
			if(truerow_dim16%2==0){ Vdatas[index] = KV.key; } else { Vdatas[index] = KV.value; }
		}
		index += 1;
	}
	
	//
	loc = loc + (VECTOR2_SIZE - col_dim16);
	#ifdef _DEBUGMODE_CHECKS2
	if(loc % VECTOR2_SIZE != 0){ cout<<"PROCESS_SPL2_GETVTXDATASET:: loc("<<loc<<") % VECTOR2_SIZE != 0"<<endl; exit(EXIT_FAILURE); }
	#endif 
	
	//
	col_dim32 = loc % NUM_PEs;
	row_dim32 = loc / NUM_PEs; // OPTIMIZE: follow with processedges_splitdstvxs
	basecol_dim16 = 0;
	col_dim16 = loc % VECTOR2_SIZE;
	row_dim16 = loc / VECTOR2_SIZE;
	
	block = loc / 512;
	baserow_dim16 = ((loc / 512) * 512) / VECTOR2_SIZE;
	localrow_16 = row_dim16 - baserow_dim16;
	localrow_16 = localrow_16 / 2; if(col_dim32 < 16){ localrow_16 = localrow_16; } else{ localrow_16 = localrow_16 + 16; }
	truerow_dim16 = baserow_dim16 + localrow_16;
	
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){
		if(index < MAX_NUM_UNIQ_EDGES_PER_VEC && loc+v < (globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE)){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("PROCESS_SPL2_GETVTXDATASET(B).col_dim16", col_dim16, VECTOR2_SIZE, col_dim16, v, loc+v);
			actsutilityobj->checkoutofbounds("PROCESS_SPL2_GETVTXDATASET(B).truerow_dim16", truerow_dim16, DOUBLE_BLOCKRAM_SIZE, truerow_dim16, v, loc+v);
			#endif
			VMdatas[index] = vmaskBITS[v][truerow_dim16];
			keyvalue_t KV = UTIL_GETKV2(vbuffer[v][truerow_dim16/2]);
			if(truerow_dim16%2==0){ Vdatas[index] = KV.key; } else { Vdatas[index] = KV.value; }
		}
		index += 1;
	}
	
	// for(unsigned int v=0; v<MAX_NUM_UNIQ_EDGES_PER_VEC; v++){
		// VMdatas[v] = vmaskBITS[col_dim16][truerow_dim16];
		// keyvalue_t KV = UTIL_GETKV2(vbuffer[col_dim16][truerow_dim16/2]);
		// if(truerow_dim16%2==0){ Vdatas[v] = KV.key; } else { Vdatas[v] = KV.value; }
	// }
	return;
} */
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
void acts_all::PROCESS_SPL3_GETSET(keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type VMdatas_tmp[32], value_t Vdatas_tmp[32], unsigned int row, unsigned int offset, globalparams_t globalparams){					
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){
		if(row < globalparams.SIZEKVS2_REDUCEPARTITION){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("PROCESS_SPL2_GETVTXDATASET(A).row", row, DOUBLE_BLOCKRAM_SIZE, row, v, NAp);
			#endif
			VMdatas_tmp[offset + v] = vmaskBITS[v][row];
			keyvalue_t KV = UTIL_GETKV2(vbuffer[v][row/2]);
			if(row%2==0){ Vdatas_tmp[offset + v] = KV.key; } else { Vdatas_tmp[offset + v] = KV.value; }
		}
	}
	return;
}
void acts_all::PROCESS_SPL3_GETVTXDATASET(unsigned int loc, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], unit1_type VMdatas[MAX_NUM_UNIQ_EDGES_PER_VEC], globalparams_t globalparams){					
	unit1_type VMdatas_tmp[32];
	#pragma HLS ARRAY_PARTITION variable=VMdatas_tmp complete
	value_t Vdatas_tmp[32];
	#pragma HLS ARRAY_PARTITION variable=Vdatas_tmp complete
	
	unsigned int fcol_dim16 = loc % VECTOR2_SIZE;
	unsigned int col_dim16 = loc % VECTOR2_SIZE;
	
	unsigned int truerow1_dim16 = PROCESS_SPL3_GETROW(loc);
	unsigned int truerow2_dim16 = PROCESS_SPL3_GETROW((loc + (VECTOR2_SIZE - col_dim16)));
	PROCESS_SPL3_GETSET(vbuffer, vmaskBITS, VMdatas_tmp, Vdatas_tmp, truerow1_dim16, 0, globalparams);
	PROCESS_SPL3_GETSET(vbuffer, vmaskBITS, VMdatas_tmp, Vdatas_tmp, truerow2_dim16, VECTOR2_SIZE, globalparams);
	
	unsigned int index = 0;
	for(unsigned int v=fcol_dim16; v<fcol_dim16 + MAX_NUM_UNIQ_EDGES_PER_VEC; v++){
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("PROCESS_SPL2_GETVTXDATASET(C).fcol_dim16", index, 32, v, NAp, NAp);
		#endif
		VMdatas[index] = VMdatas_tmp[v];
		Vdatas[index] = Vdatas_tmp[v];
		index += 1;
	}
	return;
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
	
	value_t Vset[MAX_NUM_UNIQ_EDGES_PER_VEC];
	#pragma HLS ARRAY_PARTITION variable=Vset complete
	unit1_type VMset[MAX_NUM_UNIQ_EDGES_PER_VEC];
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

	unsigned int num_passes = 1;
	buffer_type chunk_size = UTIL_getchunksize_kvs(edgessize_kvs, travstate, 0);
	READANDPROCESS_SPL_LOOP3A:  for(buffer_type part=0; part<1; part++){
		READANDPROCESS_SPL_LOOP3B:  for(buffer_type m=0; m<num_passes; m++){
			loadcount = 0;
			debug_numinvalidheads = 0;
			READANDPROCESS_SPL_LOOP3C: for (buffer_type i=0; i<chunk_size; i++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop avg=analysis_loop	
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"--- PROCESS_SPL_readandprocess: m: "<<m<<", i: "<<i<<" (of "<<chunk_size<<")"<<endl;
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
				PROCESS_SPL_debug(0, i, E, ens, mask, udataset, maskset, Vsets, VMsets, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
				#endif 
					
				srcvid_head = E[0];
				lvid_head = srcvid_head - travstate.i2;	
				parsededge_t parsed_edge0 = PARSE_EDGE(E[0]);
				incr[0] = parsed_edge0.incr;
				lsrcvids[0] = lvid_head + incr[0];
				ldstvids[0] = parsed_edge0.dstvid;	
				parsededge_t parsed_edge1 = PARSE_EDGE(E[1]);
				incr[1] = parsed_edge1.incr;
				lsrcvids[1] = lvid_head + incr[1];
				ldstvids[1] = parsed_edge1.dstvid;	
				parsededge_t parsed_edge2 = PARSE_EDGE(E[2]);
				incr[2] = parsed_edge2.incr;
				lsrcvids[2] = lvid_head + incr[2];
				ldstvids[2] = parsed_edge2.dstvid;	
				parsededge_t parsed_edge3 = PARSE_EDGE(E[3]);
				incr[3] = parsed_edge3.incr;
				lsrcvids[3] = lvid_head + incr[3];
				ldstvids[3] = parsed_edge3.dstvid;	
				parsededge_t parsed_edge4 = PARSE_EDGE(E[4]);
				incr[4] = parsed_edge4.incr;
				lsrcvids[4] = lvid_head + incr[4];
				ldstvids[4] = parsed_edge4.dstvid;	
				parsededge_t parsed_edge5 = PARSE_EDGE(E[5]);
				incr[5] = parsed_edge5.incr;
				lsrcvids[5] = lvid_head + incr[5];
				ldstvids[5] = parsed_edge5.dstvid;	
				parsededge_t parsed_edge6 = PARSE_EDGE(E[6]);
				incr[6] = parsed_edge6.incr;
				lsrcvids[6] = lvid_head + incr[6];
				ldstvids[6] = parsed_edge6.dstvid;	
				parsededge_t parsed_edge7 = PARSE_EDGE(E[7]);
				incr[7] = parsed_edge7.incr;
				lsrcvids[7] = lvid_head + incr[7];
				ldstvids[7] = parsed_edge7.dstvid;	
				parsededge_t parsed_edge8 = PARSE_EDGE(E[8]);
				incr[8] = parsed_edge8.incr;
				lsrcvids[8] = lvid_head + incr[8];
				ldstvids[8] = parsed_edge8.dstvid;	
				parsededge_t parsed_edge9 = PARSE_EDGE(E[9]);
				incr[9] = parsed_edge9.incr;
				lsrcvids[9] = lvid_head + incr[9];
				ldstvids[9] = parsed_edge9.dstvid;	
				parsededge_t parsed_edge10 = PARSE_EDGE(E[10]);
				incr[10] = parsed_edge10.incr;
				lsrcvids[10] = lvid_head + incr[10];
				ldstvids[10] = parsed_edge10.dstvid;	
				parsededge_t parsed_edge11 = PARSE_EDGE(E[11]);
				incr[11] = parsed_edge11.incr;
				lsrcvids[11] = lvid_head + incr[11];
				ldstvids[11] = parsed_edge11.dstvid;	
				parsededge_t parsed_edge12 = PARSE_EDGE(E[12]);
				incr[12] = parsed_edge12.incr;
				lsrcvids[12] = lvid_head + incr[12];
				ldstvids[12] = parsed_edge12.dstvid;	
				parsededge_t parsed_edge13 = PARSE_EDGE(E[13]);
				incr[13] = parsed_edge13.incr;
				lsrcvids[13] = lvid_head + incr[13];
				ldstvids[13] = parsed_edge13.dstvid;	
				parsededge_t parsed_edge14 = PARSE_EDGE(E[14]);
				incr[14] = parsed_edge14.incr;
				lsrcvids[14] = lvid_head + incr[14];
				ldstvids[14] = parsed_edge14.dstvid;	
				parsededge_t parsed_edge15 = PARSE_EDGE(E[15]);
				incr[15] = parsed_edge15.incr;
				lsrcvids[15] = lvid_head + incr[15];
				ldstvids[15] = parsed_edge15.dstvid;	
				
				bool_type en = ON;
				#ifdef _DEBUGMODE_CHECKS3
				PROCESS_SPL_debug(1, i, E, ens, mask, udataset, maskset, Vsets, VMsets, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
				#endif
				
				#ifdef CONFIG_READVDATA_SLIDE
					#ifdef CONFIG_READVDATA_SLIDEANDREARRANGE
					// PROCESS_SPL2_GETVTXDATASET(lvid_head, vbuffer, vmaskBITS, Vset, VMset, globalparams);
					PROCESS_SPL3_GETVTXDATASET(lvid_head, vbuffer, vmaskBITS, Vset, VMset, globalparams);
					#else 
					NOT IMPLEMENTED.
					#endif 
				#else 
					PROCESS_SPL1_GETVTXDATASET(lvid_head, vbuffer, vmaskBITS, Vset, VMset, globalparams);
				#endif 
				#ifdef _DEBUGMODE_CHECKS3
				PROCESS_SPL_debug(2, i, E, ens, mask, udataset, maskset, Vsets, VMsets, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
				#endif 
				
				ens[0] = ON; if(E[0] == INVALIDDATA || lsrcvids[0] >= validbound){ ens[0] = OFF; }
				ens[1] = ON; if(E[1] == INVALIDDATA || lsrcvids[1] >= validbound){ ens[1] = OFF; }
				ens[2] = ON; if(E[2] == INVALIDDATA || lsrcvids[2] >= validbound){ ens[2] = OFF; }
				ens[3] = ON; if(E[3] == INVALIDDATA || lsrcvids[3] >= validbound){ ens[3] = OFF; }
				ens[4] = ON; if(E[4] == INVALIDDATA || lsrcvids[4] >= validbound){ ens[4] = OFF; }
				ens[5] = ON; if(E[5] == INVALIDDATA || lsrcvids[5] >= validbound){ ens[5] = OFF; }
				ens[6] = ON; if(E[6] == INVALIDDATA || lsrcvids[6] >= validbound){ ens[6] = OFF; }
				ens[7] = ON; if(E[7] == INVALIDDATA || lsrcvids[7] >= validbound){ ens[7] = OFF; }
				ens[8] = ON; if(E[8] == INVALIDDATA || lsrcvids[8] >= validbound){ ens[8] = OFF; }
				ens[9] = ON; if(E[9] == INVALIDDATA || lsrcvids[9] >= validbound){ ens[9] = OFF; }
				ens[10] = ON; if(E[10] == INVALIDDATA || lsrcvids[10] >= validbound){ ens[10] = OFF; }
				ens[11] = ON; if(E[11] == INVALIDDATA || lsrcvids[11] >= validbound){ ens[11] = OFF; }
				ens[12] = ON; if(E[12] == INVALIDDATA || lsrcvids[12] >= validbound){ ens[12] = OFF; }
				ens[13] = ON; if(E[13] == INVALIDDATA || lsrcvids[13] >= validbound){ ens[13] = OFF; }
				ens[14] = ON; if(E[14] == INVALIDDATA || lsrcvids[14] >= validbound){ ens[14] = OFF; }
				ens[15] = ON; if(E[15] == INVALIDDATA || lsrcvids[15] >= validbound){ ens[15] = OFF; }
				#ifdef _DEBUGMODE_CHECKS3
				PROCESS_SPL_debug(3, i, E, ens, mask, udataset, maskset, Vsets, VMsets, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
				#endif 
				
 mask[0] = 0;  mask[1] = 0;  mask[2] = 0;  mask[3] = 0;  mask[4] = 0;  mask[5] = 0;  mask[6] = 0;  mask[7] = 0;  mask[8] = 0;  mask[9] = 0;  mask[10] = 0;  mask[11] = 0;  mask[12] = 0;  mask[13] = 0;  mask[14] = 0;  mask[15] = 0; 				if(GraphAlgo == PAGERANK){  mask[0] = 1;  mask[1] = 1;  mask[2] = 1;  mask[3] = 1;  mask[4] = 1;  mask[5] = 1;  mask[6] = 1;  mask[7] = 1;  mask[8] = 1;  mask[9] = 1;  mask[10] = 1;  mask[11] = 1;  mask[12] = 1;  mask[13] = 1;  mask[14] = 1;  mask[15] = 1;  } 
				else {
					udata[0] = Vset[incr[0]]; 
					mask[0] = VMset[incr[0]];
					udata[1] = Vset[incr[1]]; 
					mask[1] = VMset[incr[1]];
					udata[2] = Vset[incr[2]]; 
					mask[2] = VMset[incr[2]];
					udata[3] = Vset[incr[3]]; 
					mask[3] = VMset[incr[3]];
					udata[4] = Vset[incr[4]]; 
					mask[4] = VMset[incr[4]];
					udata[5] = Vset[incr[5]]; 
					mask[5] = VMset[incr[5]];
					udata[6] = Vset[incr[6]]; 
					mask[6] = VMset[incr[6]];
					udata[7] = Vset[incr[7]]; 
					mask[7] = VMset[incr[7]];
					udata[8] = Vset[incr[8]]; 
					mask[8] = VMset[incr[8]];
					udata[9] = Vset[incr[9]]; 
					mask[9] = VMset[incr[9]];
					udata[10] = Vset[incr[10]]; 
					mask[10] = VMset[incr[10]];
					udata[11] = Vset[incr[11]]; 
					mask[11] = VMset[incr[11]];
					udata[12] = Vset[incr[12]]; 
					mask[12] = VMset[incr[12]];
					udata[13] = Vset[incr[13]]; 
					mask[13] = VMset[incr[13]];
					udata[14] = Vset[incr[14]]; 
					mask[14] = VMset[incr[14]];
					udata[15] = Vset[incr[15]]; 
					mask[15] = VMset[incr[15]];
				}
				mask[0] = 0; 
				ens[0] = OFF;
				#ifdef _DEBUGMODE_CHECKS3
				PROCESS_SPL_debug(4, i, E, ens, mask, udataset, maskset, Vsets, VMsets, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
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
				mykeyvalue[0].key = INVALIDDATA;
				mykeyvalue[0].value = INVALIDDATA;
				#ifdef _DEBUGMODE_CHECKS3
				PROCESS_SPL_debug(5, i, E, ens, mask, udataset, maskset, Vsets, VMsets, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
				#endif 
		
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
	
				if(ens[1] == ON && mask[1] == 1){ loadcount += 2; }
				#ifdef _DEBUGMODE_CHECKS3
				PROCESS_SPL_debug(6, i, E, ens, mask, udataset, maskset, Vsets, VMsets, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
				#endif 
			}
		}
	}
	// cout<<">>> readandprocess: loadcount: "<<loadcount<<", activeloadcount: "<<activeloadcount<<", inactiveloadcount: "<<inactiveloadcount<<endl;
	// exit(EXIT_SUCCESS); ///
	fetchmessage.chunksize_kvs = loadcount;
	return fetchmessage;
}

