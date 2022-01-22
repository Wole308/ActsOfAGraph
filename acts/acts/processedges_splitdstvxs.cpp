#include "processedges_splitdstvxs.h"
using namespace std;

// => VDATA (to read)
// 16:[0,32,...,480] | [1,33,...,481] | ... | [31,63,...,511]

// => VMASK (to read)
// 32:[0,32,...,480,...,992] | [1,33,...,481,...,993] | ... | [31,63,...,511,...,1023]

#ifdef SW
processedges_splitdstvxs::processedges_splitdstvxs(){ 
	actsutilityobj = new actsutility(); 
	acts_utilobj = new acts_util(); 
	mem_accessobj = new mem_access();
}
processedges_splitdstvxs::~processedges_splitdstvxs(){}
#endif

//
value_t 
	#ifdef SW 
	processedges_splitdstvxs::
	#endif 
PROCESS_SPL_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo){
	value_t res = 0;
	if(GraphAlgo == PAGERANK){
		res = udata;
	} else if(GraphAlgo == BFS){
		res = NAp;
	} else if(GraphAlgo == SSSP){
		res = udata + edgew;
	} else {
		res = NAp;
	}
	return res;
}

void  
	#ifdef SW 
	processedges_splitdstvxs::
	#endif 
PROCESS_SPL_debug(unsigned int debugid,
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
		// if(ens[0] == ON && mask[0] == 1){ kernelglobal_numvtxsprocessed }	
		if(ens[1] == ON && mask[1] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[1]: "<<lsrcvids[1]<<", ldstvids[1]: "<<ldstvids[1]<<", udata: "<<udataset[incr[1]]<<"], [ens[1]: "<<ens[1]<<", mask[1]: "<<mask[1]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		// if(ens[1] == ON && mask[1] == 1){ kernelglobal_numvtxsprocessed }	
		if(ens[2] == ON && mask[2] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[2]: "<<lsrcvids[2]<<", ldstvids[2]: "<<ldstvids[2]<<", udata: "<<udataset[incr[2]]<<"], [ens[2]: "<<ens[2]<<", mask[2]: "<<mask[2]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		// if(ens[2] == ON && mask[2] == 1){ kernelglobal_numvtxsprocessed }	
		if(ens[3] == ON && mask[3] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[3]: "<<lsrcvids[3]<<", ldstvids[3]: "<<ldstvids[3]<<", udata: "<<udataset[incr[3]]<<"], [ens[3]: "<<ens[3]<<", mask[3]: "<<mask[3]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		// if(ens[3] == ON && mask[3] == 1){ kernelglobal_numvtxsprocessed }	
		if(ens[4] == ON && mask[4] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[4]: "<<lsrcvids[4]<<", ldstvids[4]: "<<ldstvids[4]<<", udata: "<<udataset[incr[4]]<<"], [ens[4]: "<<ens[4]<<", mask[4]: "<<mask[4]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		// if(ens[4] == ON && mask[4] == 1){ kernelglobal_numvtxsprocessed }	
		if(ens[5] == ON && mask[5] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[5]: "<<lsrcvids[5]<<", ldstvids[5]: "<<ldstvids[5]<<", udata: "<<udataset[incr[5]]<<"], [ens[5]: "<<ens[5]<<", mask[5]: "<<mask[5]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		// if(ens[5] == ON && mask[5] == 1){ kernelglobal_numvtxsprocessed }	
		if(ens[6] == ON && mask[6] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[6]: "<<lsrcvids[6]<<", ldstvids[6]: "<<ldstvids[6]<<", udata: "<<udataset[incr[6]]<<"], [ens[6]: "<<ens[6]<<", mask[6]: "<<mask[6]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		// if(ens[6] == ON && mask[6] == 1){ kernelglobal_numvtxsprocessed }	
		if(ens[7] == ON && mask[7] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[7]: "<<lsrcvids[7]<<", ldstvids[7]: "<<ldstvids[7]<<", udata: "<<udataset[incr[7]]<<"], [ens[7]: "<<ens[7]<<", mask[7]: "<<mask[7]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		// if(ens[7] == ON && mask[7] == 1){ kernelglobal_numvtxsprocessed }	
		if(ens[8] == ON && mask[8] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[8]: "<<lsrcvids[8]<<", ldstvids[8]: "<<ldstvids[8]<<", udata: "<<udataset[incr[8]]<<"], [ens[8]: "<<ens[8]<<", mask[8]: "<<mask[8]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		// if(ens[8] == ON && mask[8] == 1){ kernelglobal_numvtxsprocessed }	
		if(ens[9] == ON && mask[9] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[9]: "<<lsrcvids[9]<<", ldstvids[9]: "<<ldstvids[9]<<", udata: "<<udataset[incr[9]]<<"], [ens[9]: "<<ens[9]<<", mask[9]: "<<mask[9]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		// if(ens[9] == ON && mask[9] == 1){ kernelglobal_numvtxsprocessed }	
		if(ens[10] == ON && mask[10] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[10]: "<<lsrcvids[10]<<", ldstvids[10]: "<<ldstvids[10]<<", udata: "<<udataset[incr[10]]<<"], [ens[10]: "<<ens[10]<<", mask[10]: "<<mask[10]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		// if(ens[10] == ON && mask[10] == 1){ kernelglobal_numvtxsprocessed }	
		if(ens[11] == ON && mask[11] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[11]: "<<lsrcvids[11]<<", ldstvids[11]: "<<ldstvids[11]<<", udata: "<<udataset[incr[11]]<<"], [ens[11]: "<<ens[11]<<", mask[11]: "<<mask[11]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		// if(ens[11] == ON && mask[11] == 1){ kernelglobal_numvtxsprocessed }	
		if(ens[12] == ON && mask[12] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[12]: "<<lsrcvids[12]<<", ldstvids[12]: "<<ldstvids[12]<<", udata: "<<udataset[incr[12]]<<"], [ens[12]: "<<ens[12]<<", mask[12]: "<<mask[12]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		// if(ens[12] == ON && mask[12] == 1){ kernelglobal_numvtxsprocessed }	
		if(ens[13] == ON && mask[13] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[13]: "<<lsrcvids[13]<<", ldstvids[13]: "<<ldstvids[13]<<", udata: "<<udataset[incr[13]]<<"], [ens[13]: "<<ens[13]<<", mask[13]: "<<mask[13]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		// if(ens[13] == ON && mask[13] == 1){ kernelglobal_numvtxsprocessed }	
		if(ens[14] == ON && mask[14] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[14]: "<<lsrcvids[14]<<", ldstvids[14]: "<<ldstvids[14]<<", udata: "<<udataset[incr[14]]<<"], [ens[14]: "<<ens[14]<<", mask[14]: "<<mask[14]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		// if(ens[14] == ON && mask[14] == 1){ kernelglobal_numvtxsprocessed }	
		if(ens[15] == ON && mask[15] == 1){ cout<<"readandprocess(15)::DEBUG CODE 5:: [i: "<<i<<", lsrcvids[15]: "<<lsrcvids[15]<<", ldstvids[15]: "<<ldstvids[15]<<", udata: "<<udataset[incr[15]]<<"], [ens[15]: "<<ens[15]<<", mask[15]: "<<mask[15]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }			
		// if(ens[15] == ON && mask[15] == 1){ kernelglobal_numvtxsprocessed }	
	
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

void  
	#ifdef SW 
	processedges_splitdstvxs::
	#endif 
PROCESS_SPL_getslice(sliceinfo_t slice, int id, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[VECTOR2_SIZE], unit1_type VMdatas[VECTOR2_SIZE], unsigned int depths[64], globalparams_t globalparams){
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(slice.active == false){ cout<<"ENTIRE SLICE "<<id<<" IGNORED."<<endl; exit(EXIT_FAILURE); }
	#endif 
	if(slice.active == false){ return; }
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"ENTIRE SLICE "<<id<<" IS ACTIVE: VALID BEGIN OFFSET: "<<slice.beginoffset<<", END OFFSET: "<<slice.endoffset<<endl;
	#endif 
	for(int t=0; t<VECTOR2_SIZE; t++){
	#pragma HLS UNROLL
		int s_ = slice.s_base + t;
		
		// int rowoffseti = s_ * (globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs);
		#ifdef _DEBUGMODE_CHECKS2
		if((slice.beginoffset<0 || slice.endoffset>64)){ cout<<"PROCESS_SPL_getslice: ERROR(2): slice.beginoffset<0 || slice.endoffset>64. s_: "<<s_<<", slice.s_base: "<<slice.s_base<<", slice.beginoffset: "<<slice.beginoffset<<", slice.endoffset: "<<slice.endoffset<<endl; exit(EXIT_FAILURE); }
		#endif 
		bool en=false; if(t>=slice.beginoffset && t<slice.endoffset){ en = true; } else { en = false; }
		int rowoffseti; if(en==true){ rowoffseti = depths[s_]; }
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(en==true){ cout<<"s:"<<s_<<": v:"<<t<<" [slice:a2]: vmaskBITS["<<t<<"]["<<rowoffseti<<" + "<<slice.local_rowoffset<<"]: "<<vmaskBITS[t][rowoffseti + slice.local_rowoffset]<<endl; } else { cout<<"s:"<<s_<<": v:"<<t<<" [slice:a2][INVALID]"<<endl; }
		#endif
		#ifdef _DEBUGMODE_CHECKS3
		if(en==true){
			actsutilityobj->checkoutofbounds("PROCESS_SPL_getslice. t", t, VMASK_PACKINGSIZE, t, NAp, NAp); 
			actsutilityobj->checkoutofbounds("PROCESS_SPL_getslice. t", t, VECTOR2_SIZE, t, NAp, NAp); 
			actsutilityobj->checkoutofbounds("PROCESS_SPL_getslice. rowoffseti + slice.local_rowoffset", rowoffseti + slice.local_rowoffset, DOUBLE_BLOCKRAM_SIZE, rowoffseti, slice.local_rowoffset, s_); 
			actsutilityobj->checkoutofbounds("PROCESS_SPL_getslice. (rowoffseti + slice.local_rowoffset)/2", (rowoffseti + slice.local_rowoffset)/2, BLOCKRAM_SIZE, rowoffseti, slice.local_rowoffset, s_); 
			actsutilityobj->checkoutofbounds("PROCESS_SPL_getslice. s_", s_, 64, s_, NAp, NAp); 
			if((s_<0 || s_>64)){ cout<<"PROCESS_SPL_getslice: ERROR(2): s_<0 || s_>=64. s_: "<<s_<<", slice.s_base: "<<slice.s_base<<", t: "<<t<<endl; exit(EXIT_FAILURE); }
		}
		#endif 
		
		if(en==true){ VMdatas[t] = vmaskBITS[t][rowoffseti + slice.local_rowoffset]; } else { VMdatas[t] = 999; }
		if(en==true){ if(s_%2==0){ Vdatas[t] = vbuffer[t][(rowoffseti + slice.local_rowoffset)/2].key; } else { Vdatas[t] = vbuffer[t][(rowoffseti + slice.local_rowoffset)/2].value; }} else { Vdatas[t] = 888; }
	}
	return;
}

//
value_t 
	#ifdef SW 
	processedges_splitdstvxs::
	#endif 
PROCESS_SPL_GETVTXDATA(keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
	#pragma HLS INLINE
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<">>> PROCESS_SPL_GETVTXDATA: ERROR DETECTED (23). loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE("<<globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE<<"). loc: "<<loc<<endl;
		#endif 
		loc = 0; }
	
	unsigned int instid = loc % NUM_PEs;
	unsigned int lloc = acts_utilobj->UTIL_GETLOCALVID(loc, instid);
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
	if(row % 2 == 0){ data = acts_utilobj->UTIL_GETKV2(vbuffer[realcol][realrow]).key; } 
	else { data = acts_utilobj->UTIL_GETKV2(vbuffer[realcol][realrow]).value; }
	return data;
}

value_t 
	#ifdef SW 
	processedges_splitdstvxs::
	#endif 
PROCESS_SPL_GETVTXMASK(unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
	#pragma HLS INLINE
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<">>> PROCESS_SPL_GETVTXMASK: ERROR DETECTED (23). loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE("<<globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE<<"). loc: "<<loc<<endl;
		#endif 
		loc = 0; }
		
	unsigned int instid = loc % NUM_PEs;
	unsigned int lloc = acts_utilobj->UTIL_GETLOCALVID(loc, instid);
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

value_t 
	#ifdef SW 
	processedges_splitdstvxs::
	#endif 
PROCESS_SPL_GETVTXDATA_SLIDED(keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
	#pragma HLS INLINE
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<">>> PROCESS_SPL_GETVTXDATA_SLIDED: ERROR DETECTED (23). loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE("<<globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE<<"). loc: "<<loc<<endl;
		#endif 
		loc = 0; }
	
	unsigned int s = loc % NUM_PEs;
	unsigned int lloc = acts_utilobj->UTIL_GETLOCALVID(loc, s);
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
	if(row % 2 == 0){ data = acts_utilobj->UTIL_GETKV2(vbuffer[rrealcol][realrow]).key; } 
	else { data = acts_utilobj->UTIL_GETKV2(vbuffer[rrealcol][realrow]).value; }
	return data;
}

value_t 
	#ifdef SW 
	processedges_splitdstvxs::
	#endif 
PROCESS_SPL_GETVTXMASK_SLIDED(unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unsigned int loc, globalparams_t globalparams){
	#pragma HLS INLINE
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<">>> PROCESS_SPL_GETVTXMASK: ERROR DETECTED (23). loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE("<<globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE<<"). loc: "<<loc<<endl;
		#endif 
		loc = 0; }
		
	unsigned int s = loc % NUM_PEs;
	unsigned int lloc = acts_utilobj->UTIL_GETLOCALVID(loc, s);
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

sliceinfos_t 
	#ifdef SW 
	processedges_splitdstvxs::
	#endif 
PROCESS_SPL_GETVTXSLICES(unsigned int loc, globalparams_t globalparams){					
	#pragma HLS INLINE
	if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE){ 
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<">>> PROCESS_SPL_GETVTXMASK: ERROR DETECTED (23). loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE("<<globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE<<"). loc: "<<loc<<endl;
		#endif 
		loc = 0; }

	int s_begin = loc % NUM_PEs; //
	int llocA = acts_utilobj->UTIL_GETLOCALVID(loc, s_begin);
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
		cout<<"PROCESS_SPL_GETVTXSLICES:: SHOULD NOT BE SEEN. 23 EXITING..."<<endl;
		exit(EXIT_FAILURE);
	}
	
	sliceinfos_t sliceinfos; sliceinfos.slice[0] = sliceX; sliceinfos.slice[1] = sliceY;
	return sliceinfos;
}

void  
	#ifdef SW 
	processedges_splitdstvxs::
	#endif 
PROCESS_SPL_GETVTXDATASET(sliceinfo_t slice, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], value_t Vdatas[VECTOR2_SIZE], unit1_type VMdatas[VECTOR2_SIZE], unsigned int depths[64], globalparams_t globalparams){					
	#pragma HLS INLINE
	PROCESS_SPL_getslice(slice, 0, vbuffer, vmaskBITS, Vdatas, VMdatas, depths, globalparams);
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int t=0; t<VECTOR2_SIZE; t++){ cout<<"PROCESS_SPL_GETVTXDATASET:: VMdatas["<<t<<"]: "<<VMdatas[t]<<endl; }
	for(unsigned int t=0; t<VECTOR2_SIZE; t++){ cout<<"PROCESS_SPL_GETVTXDATASET:: VMdatas["<<t<<"]: "<<VMdatas[t]<<endl; }
	#endif
	return;
}

fetchmessage_t 
	#ifdef SW 
	processedges_splitdstvxs::
	#endif 
PROCESS_SPL_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	
	analysis_type analysis_loop = BLOCKRAM_SIZE / 2;
	analysis_type analysis_loop1 = SUBPMASKFACTOR;
	analysis_type analysis_loop2 = 16384 / SUBPMASKFACTOR;
	
	value_t E[VECTOR2_SIZE];
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
	
	unsigned int incr[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=incr complete
	unsigned int lsrcvids[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=lsrcvids complete
	unsigned int ldstvids[VECTOR2_SIZE];	
	#pragma HLS ARRAY_PARTITION variable=ldstvids complete
	value_t res[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=res complete
	
	value_t Vset[2][VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=Vset complete
	unit1_type VMset[2][VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=VMset complete
	sliceinfos_t sliceinfos;
	unsigned int edgevectorisactive[BLOCKRAM_SIZE];
	
	bool_type enPs[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=enPs complete
	bool_type _FILTERX; bool_type _FILTERY;
	unsigned int enPcounts[2][3];
	for(unsigned int j=0; j<2; j++){ for(unsigned int k=0; k<3; k++){ enPcounts[j][k] = 0; }}
	
	vertex_t srcvid_head;
	vertex_t lvid_head;
	
	int diff1;
	int diff2;
	
	batch_type vptrbaseoffset_kvs = globalparams.BASEOFFSETKVS_VERTEXPTR + (globalparams.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	batch_type voffset_kvs = sweepparams.source_partition * reducebuffersz * FETFACTOR;
	keyy_t nextbeginvptr;
	keyy_t nextbeginvptr_kvs;
	
	fetchmessage.chunksize_kvs = edgessize_kvs;
	fetchmessage.nextoffset_kvs = -1;
	buffer_type loadcount = 0;
	buffer_type loadcounts[2];
	buffer_type activeloadcount = 0;
	buffer_type inactiveloadcount = 0;
	
	unsigned int debug_numinvalidheads = 0;
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	
	unsigned int depths[64];
	for(int t=0; t<64; t++){ depths[t] = t * (globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs); }
	#ifdef CONFIG_PROCESSEDGESSPLITDSTVTXS_PARALLELVTXSACCESS_II1
		#if defined(CONFIG_PROCESSEDGESSPLITDSTVTXS_PARALLELVTXSACCESS_II1)
		unsigned int num_passes = 2;
		#elif defined(CONFIG_PROCESSEDGESSPLITDSTVTXS_PARALLELVTXSACCESS_II2)
		unsigned int num_passes = 1;
		#else 
		unsigned int num_passes = 1;
		#endif 
	#else 
	unsigned int num_passes = 1;
	#endif 
	buffer_type chunk_size = acts_utilobj->UTIL_getchunksize_kvs(edgessize_kvs, travstate, 0);
	
	READANDPROCESS_SPL_LOOP1A:  for (buffer_type m=0; m<num_passes; m++){
		loadcount = 0;
		debug_numinvalidheads = 0;
		READANDPROCESS_SPL_LOOP1B: for (buffer_type i=0; i<chunk_size; i++){
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
			PROCESS_SPL_debug(0, i, E, ens, mask, udataset, maskset, Vset, VMset, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
			#endif 
				
			srcvid_head = E[0];
			lvid_head = srcvid_head - travstate.i2;
			incr[0] = acts_utilobj->UTIL_READFROM_UINT(E[0], 28, 4);
			lsrcvids[0] = lvid_head + incr[0];
			ldstvids[0] = acts_utilobj->UTIL_READFROM_UINT(E[0], 0, 28);
			incr[1] = acts_utilobj->UTIL_READFROM_UINT(E[1], 28, 4);
			lsrcvids[1] = lvid_head + incr[1];
			ldstvids[1] = acts_utilobj->UTIL_READFROM_UINT(E[1], 0, 28);
			incr[2] = acts_utilobj->UTIL_READFROM_UINT(E[2], 28, 4);
			lsrcvids[2] = lvid_head + incr[2];
			ldstvids[2] = acts_utilobj->UTIL_READFROM_UINT(E[2], 0, 28);
			incr[3] = acts_utilobj->UTIL_READFROM_UINT(E[3], 28, 4);
			lsrcvids[3] = lvid_head + incr[3];
			ldstvids[3] = acts_utilobj->UTIL_READFROM_UINT(E[3], 0, 28);
			incr[4] = acts_utilobj->UTIL_READFROM_UINT(E[4], 28, 4);
			lsrcvids[4] = lvid_head + incr[4];
			ldstvids[4] = acts_utilobj->UTIL_READFROM_UINT(E[4], 0, 28);
			incr[5] = acts_utilobj->UTIL_READFROM_UINT(E[5], 28, 4);
			lsrcvids[5] = lvid_head + incr[5];
			ldstvids[5] = acts_utilobj->UTIL_READFROM_UINT(E[5], 0, 28);
			incr[6] = acts_utilobj->UTIL_READFROM_UINT(E[6], 28, 4);
			lsrcvids[6] = lvid_head + incr[6];
			ldstvids[6] = acts_utilobj->UTIL_READFROM_UINT(E[6], 0, 28);
			incr[7] = acts_utilobj->UTIL_READFROM_UINT(E[7], 28, 4);
			lsrcvids[7] = lvid_head + incr[7];
			ldstvids[7] = acts_utilobj->UTIL_READFROM_UINT(E[7], 0, 28);
			incr[8] = acts_utilobj->UTIL_READFROM_UINT(E[8], 28, 4);
			lsrcvids[8] = lvid_head + incr[8];
			ldstvids[8] = acts_utilobj->UTIL_READFROM_UINT(E[8], 0, 28);
			incr[9] = acts_utilobj->UTIL_READFROM_UINT(E[9], 28, 4);
			lsrcvids[9] = lvid_head + incr[9];
			ldstvids[9] = acts_utilobj->UTIL_READFROM_UINT(E[9], 0, 28);
			incr[10] = acts_utilobj->UTIL_READFROM_UINT(E[10], 28, 4);
			lsrcvids[10] = lvid_head + incr[10];
			ldstvids[10] = acts_utilobj->UTIL_READFROM_UINT(E[10], 0, 28);
			incr[11] = acts_utilobj->UTIL_READFROM_UINT(E[11], 28, 4);
			lsrcvids[11] = lvid_head + incr[11];
			ldstvids[11] = acts_utilobj->UTIL_READFROM_UINT(E[11], 0, 28);
			incr[12] = acts_utilobj->UTIL_READFROM_UINT(E[12], 28, 4);
			lsrcvids[12] = lvid_head + incr[12];
			ldstvids[12] = acts_utilobj->UTIL_READFROM_UINT(E[12], 0, 28);
			incr[13] = acts_utilobj->UTIL_READFROM_UINT(E[13], 28, 4);
			lsrcvids[13] = lvid_head + incr[13];
			ldstvids[13] = acts_utilobj->UTIL_READFROM_UINT(E[13], 0, 28);
			incr[14] = acts_utilobj->UTIL_READFROM_UINT(E[14], 28, 4);
			lsrcvids[14] = lvid_head + incr[14];
			ldstvids[14] = acts_utilobj->UTIL_READFROM_UINT(E[14], 0, 28);
			incr[15] = acts_utilobj->UTIL_READFROM_UINT(E[15], 28, 4);
			lsrcvids[15] = lvid_head + incr[15];
			ldstvids[15] = acts_utilobj->UTIL_READFROM_UINT(E[15], 0, 28);
			
			bool_type en = ON;
			#ifdef _DEBUGMODE_CHECKS3
			PROCESS_SPL_debug(1, i, E, ens, mask, udataset, maskset, Vset, VMset, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
			#endif ;
			
			#ifdef CONFIG_ACTSPROCESSEDGES_SPREADVTXREAD	
				#if defined(CONFIG_PROCESSEDGESSPLITDSTVTXS_PARALLELVTXSACCESS_II1)
				sliceinfos = PROCESS_SPL_GETVTXSLICES(lvid_head, globalparams);
				if(m==0){ PROCESS_SPL_GETVTXDATASET(sliceinfos.slice[0], vbuffer, vmaskBITS, Vset[0], VMset[0], depths, globalparams); }
				else{ PROCESS_SPL_GETVTXDATASET(sliceinfos.slice[1], vbuffer, vmaskBITS, Vset[1], VMset[1], depths, globalparams); }
				#elif defined(CONFIG_PROCESSEDGESSPLITDSTVTXS_PARALLELVTXSACCESS_II2)
				sliceinfos = PROCESS_SPL_GETVTXSLICES(lvid_head, globalparams);
				PROCESS_SPL_GETVTXDATASET(sliceinfos.slice[0], vbuffer, vmaskBITS, Vset[0], VMset[0], depths, globalparams);
				PROCESS_SPL_GETVTXDATASET(sliceinfos.slice[1], vbuffer, vmaskBITS, Vset[1], VMset[1], depths, globalparams);
				#else 
 
				udataset[0] = PROCESS_SPL_GETVTXDATA_SLIDED(vbuffer, lvid_head + 0, globalparams);
 
				udataset[1] = PROCESS_SPL_GETVTXDATA_SLIDED(vbuffer, lvid_head + 1, globalparams);
 
				udataset[2] = PROCESS_SPL_GETVTXDATA_SLIDED(vbuffer, lvid_head + 2, globalparams);
 
				udataset[3] = PROCESS_SPL_GETVTXDATA_SLIDED(vbuffer, lvid_head + 3, globalparams);
 
				udataset[4] = PROCESS_SPL_GETVTXDATA_SLIDED(vbuffer, lvid_head + 4, globalparams);
 
				udataset[5] = PROCESS_SPL_GETVTXDATA_SLIDED(vbuffer, lvid_head + 5, globalparams);
 
				udataset[6] = PROCESS_SPL_GETVTXDATA_SLIDED(vbuffer, lvid_head + 6, globalparams);
 
				udataset[7] = PROCESS_SPL_GETVTXDATA_SLIDED(vbuffer, lvid_head + 7, globalparams);
				maskset[0] = PROCESS_SPL_GETVTXMASK_SLIDED(vmaskBITS, lvid_head + 0, globalparams);
				maskset[1] = PROCESS_SPL_GETVTXMASK_SLIDED(vmaskBITS, lvid_head + 1, globalparams);
				maskset[2] = PROCESS_SPL_GETVTXMASK_SLIDED(vmaskBITS, lvid_head + 2, globalparams);
				maskset[3] = PROCESS_SPL_GETVTXMASK_SLIDED(vmaskBITS, lvid_head + 3, globalparams);
				maskset[4] = PROCESS_SPL_GETVTXMASK_SLIDED(vmaskBITS, lvid_head + 4, globalparams);
				maskset[5] = PROCESS_SPL_GETVTXMASK_SLIDED(vmaskBITS, lvid_head + 5, globalparams);
				maskset[6] = PROCESS_SPL_GETVTXMASK_SLIDED(vmaskBITS, lvid_head + 6, globalparams);
				maskset[7] = PROCESS_SPL_GETVTXMASK_SLIDED(vmaskBITS, lvid_head + 7, globalparams);
				#endif 
			#else 
 
				udataset[0] = PROCESS_SPL_GETVTXDATA(vbuffer, lvid_head + 0, globalparams);
 
				udataset[1] = PROCESS_SPL_GETVTXDATA(vbuffer, lvid_head + 1, globalparams);
 
				udataset[2] = PROCESS_SPL_GETVTXDATA(vbuffer, lvid_head + 2, globalparams);
 
				udataset[3] = PROCESS_SPL_GETVTXDATA(vbuffer, lvid_head + 3, globalparams);
 
				udataset[4] = PROCESS_SPL_GETVTXDATA(vbuffer, lvid_head + 4, globalparams);
 
				udataset[5] = PROCESS_SPL_GETVTXDATA(vbuffer, lvid_head + 5, globalparams);
 
				udataset[6] = PROCESS_SPL_GETVTXDATA(vbuffer, lvid_head + 6, globalparams);
 
				udataset[7] = PROCESS_SPL_GETVTXDATA(vbuffer, lvid_head + 7, globalparams);
				maskset[0] = PROCESS_SPL_GETVTXMASK(vmaskBITS, lvid_head + 0, globalparams);
				maskset[1] = PROCESS_SPL_GETVTXMASK(vmaskBITS, lvid_head + 1, globalparams);
				maskset[2] = PROCESS_SPL_GETVTXMASK(vmaskBITS, lvid_head + 2, globalparams);
				maskset[3] = PROCESS_SPL_GETVTXMASK(vmaskBITS, lvid_head + 3, globalparams);
				maskset[4] = PROCESS_SPL_GETVTXMASK(vmaskBITS, lvid_head + 4, globalparams);
				maskset[5] = PROCESS_SPL_GETVTXMASK(vmaskBITS, lvid_head + 5, globalparams);
				maskset[6] = PROCESS_SPL_GETVTXMASK(vmaskBITS, lvid_head + 6, globalparams);
				maskset[7] = PROCESS_SPL_GETVTXMASK(vmaskBITS, lvid_head + 7, globalparams);
			#endif
			#ifdef _DEBUGMODE_CHECKS3
			PROCESS_SPL_debug(2, i, E, ens, mask, udataset, maskset, Vset, VMset, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
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
			#ifdef CONFIG_ACTSPROCESSEDGES_SPREADVTXREAD
			diff1 = sliceinfos.slice[0].endoffset - sliceinfos.slice[0].beginoffset;
			diff2 = sliceinfos.slice[1].endoffset - sliceinfos.slice[1].beginoffset;
			unit16_type posX0 = sliceinfos.slice[0].beginoffset + incr[0];
			unit16_type posY0 = sliceinfos.slice[1].beginoffset + (incr[0] - diff1);
			unit16_type posX1 = sliceinfos.slice[0].beginoffset + incr[1];
			unit16_type posY1 = sliceinfos.slice[1].beginoffset + (incr[1] - diff1);
			unit16_type posX2 = sliceinfos.slice[0].beginoffset + incr[2];
			unit16_type posY2 = sliceinfos.slice[1].beginoffset + (incr[2] - diff1);
			unit16_type posX3 = sliceinfos.slice[0].beginoffset + incr[3];
			unit16_type posY3 = sliceinfos.slice[1].beginoffset + (incr[3] - diff1);
			unit16_type posX4 = sliceinfos.slice[0].beginoffset + incr[4];
			unit16_type posY4 = sliceinfos.slice[1].beginoffset + (incr[4] - diff1);
			unit16_type posX5 = sliceinfos.slice[0].beginoffset + incr[5];
			unit16_type posY5 = sliceinfos.slice[1].beginoffset + (incr[5] - diff1);
			unit16_type posX6 = sliceinfos.slice[0].beginoffset + incr[6];
			unit16_type posY6 = sliceinfos.slice[1].beginoffset + (incr[6] - diff1);
			unit16_type posX7 = sliceinfos.slice[0].beginoffset + incr[7];
			unit16_type posY7 = sliceinfos.slice[1].beginoffset + (incr[7] - diff1);
			unit16_type posX8 = sliceinfos.slice[0].beginoffset + incr[8];
			unit16_type posY8 = sliceinfos.slice[1].beginoffset + (incr[8] - diff1);
			unit16_type posX9 = sliceinfos.slice[0].beginoffset + incr[9];
			unit16_type posY9 = sliceinfos.slice[1].beginoffset + (incr[9] - diff1);
			unit16_type posX10 = sliceinfos.slice[0].beginoffset + incr[10];
			unit16_type posY10 = sliceinfos.slice[1].beginoffset + (incr[10] - diff1);
			unit16_type posX11 = sliceinfos.slice[0].beginoffset + incr[11];
			unit16_type posY11 = sliceinfos.slice[1].beginoffset + (incr[11] - diff1);
			unit16_type posX12 = sliceinfos.slice[0].beginoffset + incr[12];
			unit16_type posY12 = sliceinfos.slice[1].beginoffset + (incr[12] - diff1);
			unit16_type posX13 = sliceinfos.slice[0].beginoffset + incr[13];
			unit16_type posY13 = sliceinfos.slice[1].beginoffset + (incr[13] - diff1);
			unit16_type posX14 = sliceinfos.slice[0].beginoffset + incr[14];
			unit16_type posY14 = sliceinfos.slice[1].beginoffset + (incr[14] - diff1);
			unit16_type posX15 = sliceinfos.slice[0].beginoffset + incr[15];
			unit16_type posY15 = sliceinfos.slice[1].beginoffset + (incr[15] - diff1);
	
				#if defined(CONFIG_PROCESSEDGESSPLITDSTVTXS_PARALLELVTXSACCESS_II1)
	
				if(m==0){ if(incr[0] >= 0 && incr[0] < diff1){ enPs[0] = ON; } else { enPs[0] = OFF; }} 
				else if(m==1){ if(incr[0] >= diff1 && incr[0] < (diff1+diff2)){ enPs[0] = ON; } else { enPs[0] = OFF; }} 
				else {}
	
				if(m==0){ if(incr[1] >= 0 && incr[1] < diff1){ enPs[1] = ON; } else { enPs[1] = OFF; }} 
				else if(m==1){ if(incr[1] >= diff1 && incr[1] < (diff1+diff2)){ enPs[1] = ON; } else { enPs[1] = OFF; }} 
				else {}
	
				if(m==0){ if(incr[2] >= 0 && incr[2] < diff1){ enPs[2] = ON; } else { enPs[2] = OFF; }} 
				else if(m==1){ if(incr[2] >= diff1 && incr[2] < (diff1+diff2)){ enPs[2] = ON; } else { enPs[2] = OFF; }} 
				else {}
	
				if(m==0){ if(incr[3] >= 0 && incr[3] < diff1){ enPs[3] = ON; } else { enPs[3] = OFF; }} 
				else if(m==1){ if(incr[3] >= diff1 && incr[3] < (diff1+diff2)){ enPs[3] = ON; } else { enPs[3] = OFF; }} 
				else {}
	
				if(m==0){ if(incr[4] >= 0 && incr[4] < diff1){ enPs[4] = ON; } else { enPs[4] = OFF; }} 
				else if(m==1){ if(incr[4] >= diff1 && incr[4] < (diff1+diff2)){ enPs[4] = ON; } else { enPs[4] = OFF; }} 
				else {}
	
				if(m==0){ if(incr[5] >= 0 && incr[5] < diff1){ enPs[5] = ON; } else { enPs[5] = OFF; }} 
				else if(m==1){ if(incr[5] >= diff1 && incr[5] < (diff1+diff2)){ enPs[5] = ON; } else { enPs[5] = OFF; }} 
				else {}
	
				if(m==0){ if(incr[6] >= 0 && incr[6] < diff1){ enPs[6] = ON; } else { enPs[6] = OFF; }} 
				else if(m==1){ if(incr[6] >= diff1 && incr[6] < (diff1+diff2)){ enPs[6] = ON; } else { enPs[6] = OFF; }} 
				else {}
	
				if(m==0){ if(incr[7] >= 0 && incr[7] < diff1){ enPs[7] = ON; } else { enPs[7] = OFF; }} 
				else if(m==1){ if(incr[7] >= diff1 && incr[7] < (diff1+diff2)){ enPs[7] = ON; } else { enPs[7] = OFF; }} 
				else {}
	
				if(m==0){ if(incr[8] >= 0 && incr[8] < diff1){ enPs[8] = ON; } else { enPs[8] = OFF; }} 
				else if(m==1){ if(incr[8] >= diff1 && incr[8] < (diff1+diff2)){ enPs[8] = ON; } else { enPs[8] = OFF; }} 
				else {}
	
				if(m==0){ if(incr[9] >= 0 && incr[9] < diff1){ enPs[9] = ON; } else { enPs[9] = OFF; }} 
				else if(m==1){ if(incr[9] >= diff1 && incr[9] < (diff1+diff2)){ enPs[9] = ON; } else { enPs[9] = OFF; }} 
				else {}
	
				if(m==0){ if(incr[10] >= 0 && incr[10] < diff1){ enPs[10] = ON; } else { enPs[10] = OFF; }} 
				else if(m==1){ if(incr[10] >= diff1 && incr[10] < (diff1+diff2)){ enPs[10] = ON; } else { enPs[10] = OFF; }} 
				else {}
	
				if(m==0){ if(incr[11] >= 0 && incr[11] < diff1){ enPs[11] = ON; } else { enPs[11] = OFF; }} 
				else if(m==1){ if(incr[11] >= diff1 && incr[11] < (diff1+diff2)){ enPs[11] = ON; } else { enPs[11] = OFF; }} 
				else {}
	
				if(m==0){ if(incr[12] >= 0 && incr[12] < diff1){ enPs[12] = ON; } else { enPs[12] = OFF; }} 
				else if(m==1){ if(incr[12] >= diff1 && incr[12] < (diff1+diff2)){ enPs[12] = ON; } else { enPs[12] = OFF; }} 
				else {}
	
				if(m==0){ if(incr[13] >= 0 && incr[13] < diff1){ enPs[13] = ON; } else { enPs[13] = OFF; }} 
				else if(m==1){ if(incr[13] >= diff1 && incr[13] < (diff1+diff2)){ enPs[13] = ON; } else { enPs[13] = OFF; }} 
				else {}
	
				if(m==0){ if(incr[14] >= 0 && incr[14] < diff1){ enPs[14] = ON; } else { enPs[14] = OFF; }} 
				else if(m==1){ if(incr[14] >= diff1 && incr[14] < (diff1+diff2)){ enPs[14] = ON; } else { enPs[14] = OFF; }} 
				else {}
	
				if(m==0){ if(incr[15] >= 0 && incr[15] < diff1){ enPs[15] = ON; } else { enPs[15] = OFF; }} 
				else if(m==1){ if(incr[15] >= diff1 && incr[15] < (diff1+diff2)){ enPs[15] = ON; } else { enPs[15] = OFF; }} 
				else {}
				#elif defined(CONFIG_PROCESSEDGESSPLITDSTVTXS_PARALLELVTXSACCESS_II2)
				enPs[0] = ON;
				enPs[1] = ON;
				enPs[2] = ON;
				enPs[3] = ON;
				enPs[4] = ON;
				enPs[5] = ON;
				enPs[6] = ON;
				enPs[7] = ON;
				enPs[8] = ON;
				enPs[9] = ON;
				enPs[10] = ON;
				enPs[11] = ON;
				enPs[12] = ON;
				enPs[13] = ON;
				enPs[14] = ON;
				enPs[15] = ON;
				#else 
				enPs[0] = ON;
				enPs[1] = ON;
				enPs[2] = ON;
				enPs[3] = ON;
				enPs[4] = ON;
				enPs[5] = ON;
				enPs[6] = ON;
				enPs[7] = ON;
				enPs[8] = ON;
				enPs[9] = ON;
				enPs[10] = ON;
				enPs[11] = ON;
				enPs[12] = ON;
				enPs[13] = ON;
				enPs[14] = ON;
				enPs[15] = ON;
	
				#endif 
			#endif 
			#ifdef _DEBUGMODE_CHECKS3
			PROCESS_SPL_debug(3, i, E, ens, mask, udataset, maskset, Vset, VMset, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
			#endif 
			
 mask[0] = 0;  mask[1] = 0;  mask[2] = 0;  mask[3] = 0;  mask[4] = 0;  mask[5] = 0;  mask[6] = 0;  mask[7] = 0;  mask[8] = 0;  mask[9] = 0;  mask[10] = 0;  mask[11] = 0;  mask[12] = 0;  mask[13] = 0;  mask[14] = 0;  mask[15] = 0; 			if(GraphAlgo == PAGERANK){  mask[0] = 1;  mask[1] = 1;  mask[2] = 1;  mask[3] = 1;  mask[4] = 1;  mask[5] = 1;  mask[6] = 1;  mask[7] = 1;  mask[8] = 1;  mask[9] = 1;  mask[10] = 1;  mask[11] = 1;  mask[12] = 1;  mask[13] = 1;  mask[14] = 1;  mask[15] = 1;  } 
			else {
				#ifdef CONFIG_ACTSPROCESSEDGES_SPREADVTXREAD	
					#if defined(CONFIG_PROCESSEDGESSPLITDSTVTXS_PARALLELVTXSACCESS_II1)	
					if(enPs[0] == ON){ udata[0] = Vset[m][posX0]; mask[0] = VMset[m][posX0]; } else { mask[0] = 0; }
					if(enPs[1] == ON){ udata[1] = Vset[m][posX1]; mask[1] = VMset[m][posX1]; } else { mask[1] = 0; }
					if(enPs[2] == ON){ udata[2] = Vset[m][posX2]; mask[2] = VMset[m][posX2]; } else { mask[2] = 0; }
					if(enPs[3] == ON){ udata[3] = Vset[m][posX3]; mask[3] = VMset[m][posX3]; } else { mask[3] = 0; }
					if(enPs[4] == ON){ udata[4] = Vset[m][posX4]; mask[4] = VMset[m][posX4]; } else { mask[4] = 0; }
					if(enPs[5] == ON){ udata[5] = Vset[m][posX5]; mask[5] = VMset[m][posX5]; } else { mask[5] = 0; }
					if(enPs[6] == ON){ udata[6] = Vset[m][posX6]; mask[6] = VMset[m][posX6]; } else { mask[6] = 0; }
					if(enPs[7] == ON){ udata[7] = Vset[m][posX7]; mask[7] = VMset[m][posX7]; } else { mask[7] = 0; }
					if(enPs[8] == ON){ udata[8] = Vset[m][posX8]; mask[8] = VMset[m][posX8]; } else { mask[8] = 0; }
					if(enPs[9] == ON){ udata[9] = Vset[m][posX9]; mask[9] = VMset[m][posX9]; } else { mask[9] = 0; }
					if(enPs[10] == ON){ udata[10] = Vset[m][posX10]; mask[10] = VMset[m][posX10]; } else { mask[10] = 0; }
					if(enPs[11] == ON){ udata[11] = Vset[m][posX11]; mask[11] = VMset[m][posX11]; } else { mask[11] = 0; }
					if(enPs[12] == ON){ udata[12] = Vset[m][posX12]; mask[12] = VMset[m][posX12]; } else { mask[12] = 0; }
					if(enPs[13] == ON){ udata[13] = Vset[m][posX13]; mask[13] = VMset[m][posX13]; } else { mask[13] = 0; }
					if(enPs[14] == ON){ udata[14] = Vset[m][posX14]; mask[14] = VMset[m][posX14]; } else { mask[14] = 0; }
					if(enPs[15] == ON){ udata[15] = Vset[m][posX15]; mask[15] = VMset[m][posX15]; } else { mask[15] = 0; }
					#elif defined(CONFIG_PROCESSEDGESSPLITDSTVTXS_PARALLELVTXSACCESS_II2)
					if(incr[0] >= 0 && incr[0] < diff1){ udata[0] = Vset[0][posX0]; mask[0] = VMset[0][posX0]; }
					else if(incr[0] >= diff1 && incr[0] < (diff1+diff2)){ udata[0] = Vset[1][posY0]; mask[0] = VMset[1][posY0]; }
					else { mask[0] = 0; }
					if(incr[1] >= 0 && incr[1] < diff1){ udata[1] = Vset[0][posX1]; mask[1] = VMset[0][posX1]; }
					else if(incr[1] >= diff1 && incr[1] < (diff1+diff2)){ udata[1] = Vset[1][posY1]; mask[1] = VMset[1][posY1]; }
					else { mask[1] = 0; }
					if(incr[2] >= 0 && incr[2] < diff1){ udata[2] = Vset[0][posX2]; mask[2] = VMset[0][posX2]; }
					else if(incr[2] >= diff1 && incr[2] < (diff1+diff2)){ udata[2] = Vset[1][posY2]; mask[2] = VMset[1][posY2]; }
					else { mask[2] = 0; }
					if(incr[3] >= 0 && incr[3] < diff1){ udata[3] = Vset[0][posX3]; mask[3] = VMset[0][posX3]; }
					else if(incr[3] >= diff1 && incr[3] < (diff1+diff2)){ udata[3] = Vset[1][posY3]; mask[3] = VMset[1][posY3]; }
					else { mask[3] = 0; }
					if(incr[4] >= 0 && incr[4] < diff1){ udata[4] = Vset[0][posX4]; mask[4] = VMset[0][posX4]; }
					else if(incr[4] >= diff1 && incr[4] < (diff1+diff2)){ udata[4] = Vset[1][posY4]; mask[4] = VMset[1][posY4]; }
					else { mask[4] = 0; }
					if(incr[5] >= 0 && incr[5] < diff1){ udata[5] = Vset[0][posX5]; mask[5] = VMset[0][posX5]; }
					else if(incr[5] >= diff1 && incr[5] < (diff1+diff2)){ udata[5] = Vset[1][posY5]; mask[5] = VMset[1][posY5]; }
					else { mask[5] = 0; }
					if(incr[6] >= 0 && incr[6] < diff1){ udata[6] = Vset[0][posX6]; mask[6] = VMset[0][posX6]; }
					else if(incr[6] >= diff1 && incr[6] < (diff1+diff2)){ udata[6] = Vset[1][posY6]; mask[6] = VMset[1][posY6]; }
					else { mask[6] = 0; }
					if(incr[7] >= 0 && incr[7] < diff1){ udata[7] = Vset[0][posX7]; mask[7] = VMset[0][posX7]; }
					else if(incr[7] >= diff1 && incr[7] < (diff1+diff2)){ udata[7] = Vset[1][posY7]; mask[7] = VMset[1][posY7]; }
					else { mask[7] = 0; }
					if(incr[8] >= 0 && incr[8] < diff1){ udata[8] = Vset[0][posX8]; mask[8] = VMset[0][posX8]; }
					else if(incr[8] >= diff1 && incr[8] < (diff1+diff2)){ udata[8] = Vset[1][posY8]; mask[8] = VMset[1][posY8]; }
					else { mask[8] = 0; }
					if(incr[9] >= 0 && incr[9] < diff1){ udata[9] = Vset[0][posX9]; mask[9] = VMset[0][posX9]; }
					else if(incr[9] >= diff1 && incr[9] < (diff1+diff2)){ udata[9] = Vset[1][posY9]; mask[9] = VMset[1][posY9]; }
					else { mask[9] = 0; }
					if(incr[10] >= 0 && incr[10] < diff1){ udata[10] = Vset[0][posX10]; mask[10] = VMset[0][posX10]; }
					else if(incr[10] >= diff1 && incr[10] < (diff1+diff2)){ udata[10] = Vset[1][posY10]; mask[10] = VMset[1][posY10]; }
					else { mask[10] = 0; }
					if(incr[11] >= 0 && incr[11] < diff1){ udata[11] = Vset[0][posX11]; mask[11] = VMset[0][posX11]; }
					else if(incr[11] >= diff1 && incr[11] < (diff1+diff2)){ udata[11] = Vset[1][posY11]; mask[11] = VMset[1][posY11]; }
					else { mask[11] = 0; }
					if(incr[12] >= 0 && incr[12] < diff1){ udata[12] = Vset[0][posX12]; mask[12] = VMset[0][posX12]; }
					else if(incr[12] >= diff1 && incr[12] < (diff1+diff2)){ udata[12] = Vset[1][posY12]; mask[12] = VMset[1][posY12]; }
					else { mask[12] = 0; }
					if(incr[13] >= 0 && incr[13] < diff1){ udata[13] = Vset[0][posX13]; mask[13] = VMset[0][posX13]; }
					else if(incr[13] >= diff1 && incr[13] < (diff1+diff2)){ udata[13] = Vset[1][posY13]; mask[13] = VMset[1][posY13]; }
					else { mask[13] = 0; }
					if(incr[14] >= 0 && incr[14] < diff1){ udata[14] = Vset[0][posX14]; mask[14] = VMset[0][posX14]; }
					else if(incr[14] >= diff1 && incr[14] < (diff1+diff2)){ udata[14] = Vset[1][posY14]; mask[14] = VMset[1][posY14]; }
					else { mask[14] = 0; }
					if(incr[15] >= 0 && incr[15] < diff1){ udata[15] = Vset[0][posX15]; mask[15] = VMset[0][posX15]; }
					else if(incr[15] >= diff1 && incr[15] < (diff1+diff2)){ udata[15] = Vset[1][posY15]; mask[15] = VMset[1][posY15]; }
					else { mask[15] = 0; }
					#else
					if(incr[0] >= 0 && incr[0] < diff1){ udata[0] = Vset[0][posX0]; mask[0] = VMset[0][posX0]; }
					else if(incr[0] >= diff1 && incr[0] < (diff1+diff2)){ udata[0] = Vset[1][posY0]; mask[0] = VMset[1][posY0]; }
					else { mask[0] = 0; }
					if(incr[1] >= 0 && incr[1] < diff1){ udata[1] = Vset[0][posX1]; mask[1] = VMset[0][posX1]; }
					else if(incr[1] >= diff1 && incr[1] < (diff1+diff2)){ udata[1] = Vset[1][posY1]; mask[1] = VMset[1][posY1]; }
					else { mask[1] = 0; }
					if(incr[2] >= 0 && incr[2] < diff1){ udata[2] = Vset[0][posX2]; mask[2] = VMset[0][posX2]; }
					else if(incr[2] >= diff1 && incr[2] < (diff1+diff2)){ udata[2] = Vset[1][posY2]; mask[2] = VMset[1][posY2]; }
					else { mask[2] = 0; }
					if(incr[3] >= 0 && incr[3] < diff1){ udata[3] = Vset[0][posX3]; mask[3] = VMset[0][posX3]; }
					else if(incr[3] >= diff1 && incr[3] < (diff1+diff2)){ udata[3] = Vset[1][posY3]; mask[3] = VMset[1][posY3]; }
					else { mask[3] = 0; }
					if(incr[4] >= 0 && incr[4] < diff1){ udata[4] = Vset[0][posX4]; mask[4] = VMset[0][posX4]; }
					else if(incr[4] >= diff1 && incr[4] < (diff1+diff2)){ udata[4] = Vset[1][posY4]; mask[4] = VMset[1][posY4]; }
					else { mask[4] = 0; }
					if(incr[5] >= 0 && incr[5] < diff1){ udata[5] = Vset[0][posX5]; mask[5] = VMset[0][posX5]; }
					else if(incr[5] >= diff1 && incr[5] < (diff1+diff2)){ udata[5] = Vset[1][posY5]; mask[5] = VMset[1][posY5]; }
					else { mask[5] = 0; }
					if(incr[6] >= 0 && incr[6] < diff1){ udata[6] = Vset[0][posX6]; mask[6] = VMset[0][posX6]; }
					else if(incr[6] >= diff1 && incr[6] < (diff1+diff2)){ udata[6] = Vset[1][posY6]; mask[6] = VMset[1][posY6]; }
					else { mask[6] = 0; }
					if(incr[7] >= 0 && incr[7] < diff1){ udata[7] = Vset[0][posX7]; mask[7] = VMset[0][posX7]; }
					else if(incr[7] >= diff1 && incr[7] < (diff1+diff2)){ udata[7] = Vset[1][posY7]; mask[7] = VMset[1][posY7]; }
					else { mask[7] = 0; }
					if(incr[8] >= 0 && incr[8] < diff1){ udata[8] = Vset[0][posX8]; mask[8] = VMset[0][posX8]; }
					else if(incr[8] >= diff1 && incr[8] < (diff1+diff2)){ udata[8] = Vset[1][posY8]; mask[8] = VMset[1][posY8]; }
					else { mask[8] = 0; }
					if(incr[9] >= 0 && incr[9] < diff1){ udata[9] = Vset[0][posX9]; mask[9] = VMset[0][posX9]; }
					else if(incr[9] >= diff1 && incr[9] < (diff1+diff2)){ udata[9] = Vset[1][posY9]; mask[9] = VMset[1][posY9]; }
					else { mask[9] = 0; }
					if(incr[10] >= 0 && incr[10] < diff1){ udata[10] = Vset[0][posX10]; mask[10] = VMset[0][posX10]; }
					else if(incr[10] >= diff1 && incr[10] < (diff1+diff2)){ udata[10] = Vset[1][posY10]; mask[10] = VMset[1][posY10]; }
					else { mask[10] = 0; }
					if(incr[11] >= 0 && incr[11] < diff1){ udata[11] = Vset[0][posX11]; mask[11] = VMset[0][posX11]; }
					else if(incr[11] >= diff1 && incr[11] < (diff1+diff2)){ udata[11] = Vset[1][posY11]; mask[11] = VMset[1][posY11]; }
					else { mask[11] = 0; }
					if(incr[12] >= 0 && incr[12] < diff1){ udata[12] = Vset[0][posX12]; mask[12] = VMset[0][posX12]; }
					else if(incr[12] >= diff1 && incr[12] < (diff1+diff2)){ udata[12] = Vset[1][posY12]; mask[12] = VMset[1][posY12]; }
					else { mask[12] = 0; }
					if(incr[13] >= 0 && incr[13] < diff1){ udata[13] = Vset[0][posX13]; mask[13] = VMset[0][posX13]; }
					else if(incr[13] >= diff1 && incr[13] < (diff1+diff2)){ udata[13] = Vset[1][posY13]; mask[13] = VMset[1][posY13]; }
					else { mask[13] = 0; }
					if(incr[14] >= 0 && incr[14] < diff1){ udata[14] = Vset[0][posX14]; mask[14] = VMset[0][posX14]; }
					else if(incr[14] >= diff1 && incr[14] < (diff1+diff2)){ udata[14] = Vset[1][posY14]; mask[14] = VMset[1][posY14]; }
					else { mask[14] = 0; }
					if(incr[15] >= 0 && incr[15] < diff1){ udata[15] = Vset[0][posX15]; mask[15] = VMset[0][posX15]; }
					else if(incr[15] >= diff1 && incr[15] < (diff1+diff2)){ udata[15] = Vset[1][posY15]; mask[15] = VMset[1][posY15]; }
					else { mask[15] = 0; }
	
					#endif 
				#else 
				udata[0] = udataset[incr[0]];
				mask[0] = maskset[incr[0]];
				udata[1] = udataset[incr[1]];
				mask[1] = maskset[incr[1]];
				udata[2] = udataset[incr[2]];
				mask[2] = maskset[incr[2]];
				udata[3] = udataset[incr[3]];
				mask[3] = maskset[incr[3]];
				udata[4] = udataset[incr[4]];
				mask[4] = maskset[incr[4]];
				udata[5] = udataset[incr[5]];
				mask[5] = maskset[incr[5]];
				udata[6] = udataset[incr[6]];
				mask[6] = maskset[incr[6]];
				udata[7] = udataset[incr[7]];
				mask[7] = maskset[incr[7]];
				udata[8] = udataset[incr[8]];
				mask[8] = maskset[incr[8]];
				udata[9] = udataset[incr[9]];
				mask[9] = maskset[incr[9]];
				udata[10] = udataset[incr[10]];
				mask[10] = maskset[incr[10]];
				udata[11] = udataset[incr[11]];
				mask[11] = maskset[incr[11]];
				udata[12] = udataset[incr[12]];
				mask[12] = maskset[incr[12]];
				udata[13] = udataset[incr[13]];
				mask[13] = maskset[incr[13]];
				udata[14] = udataset[incr[14]];
				mask[14] = maskset[incr[14]];
				udata[15] = udataset[incr[15]];
				mask[15] = maskset[incr[15]];
		
				#endif 
			}
			mask[0] = 0; 
			ens[0] = OFF; 
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
			mykeyvalue[0].key = INVALIDDATA;
			mykeyvalue[0].value = INVALIDDATA;
			#ifdef _DEBUGMODE_CHECKS3
			PROCESS_SPL_debug(5, i, E, ens, mask, udataset, maskset, Vset, VMset, lvids, incr, lsrcvids, ldstvids, res, mykeyvalue, sweepparams, globalparams, lvid_head, srcvid_head, travstate, chunk_size, sliceinfos, &activeloadcount, &inactiveloadcount, &debug_numinvalidheads);
			#endif 
			
			if(enPs[0] = ON){ buffer[0][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue[0]); }
			if(enPs[1] = ON){ buffer[1][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue[1]); }
			if(enPs[2] = ON){ buffer[2][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue[2]); }
			if(enPs[3] = ON){ buffer[3][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue[3]); }
			if(enPs[4] = ON){ buffer[4][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue[4]); }
			if(enPs[5] = ON){ buffer[5][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue[5]); }
			if(enPs[6] = ON){ buffer[6][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue[6]); }
			if(enPs[7] = ON){ buffer[7][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue[7]); }
			if(enPs[8] = ON){ buffer[0][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue[8]); }
			if(enPs[9] = ON){ buffer[1][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue[9]); }
			if(enPs[10] = ON){ buffer[2][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue[10]); }
			if(enPs[11] = ON){ buffer[3][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue[11]); }
			if(enPs[12] = ON){ buffer[4][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue[12]); }
			if(enPs[13] = ON){ buffer[5][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue[13]); }
			if(enPs[14] = ON){ buffer[6][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue[14]); }
			if(enPs[15] = ON){ buffer[7][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue[15]); }
	
			
			#ifdef CONFIG_ACTSPROCESSEDGES_SPREADVTXREAD
				#if defined(CONFIG_PROCESSEDGESSPLITDSTVTXS_PARALLELVTXSACCESS_II1)
				if(m==0){ if(ens[1] == ON && mask[1] == 1){ loadcount += 2; edgevectorisactive[i] = ON; } else{ edgevectorisactive[i] = OFF; }}
				else{ if(edgevectorisactive[i] == ON){ loadcount += 2; }}
				#elif defined(CONFIG_PROCESSEDGESSPLITDSTVTXS_PARALLELVTXSACCESS_II2)
				if(ens[1] == ON && mask[1] == 1){ loadcount += 2; }
				#else 
				if(ens[1] == ON && mask[1] == 1){ loadcount += 2; }
				#endif 
			#else 
			if(ens[1] == ON && mask[1] == 1){ loadcount += 2; }
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
