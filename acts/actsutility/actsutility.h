#ifndef ACTSUTILITY_H
#define ACTSUTILITY_H
#include "../../include/config_params.h"
#ifdef SW
#include "../../src/utility/utility.h"
#endif 
#include "../include/actscommon.h"
#include "../../include/common.h"
using namespace std;

#define MYSTATSYSIZE 64 // 4//8//16//64

class actsutility {
public:

	actsutility();
	~actsutility();
	
	void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3);
	void checkforequal(string message, unsigned int data1, unsigned int data2, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3);
	void checkforequal(string message, unsigned int * data1, unsigned int * data2, unsigned int size, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3);
	void hcheckforequal(string message, unsigned int * data1, unsigned int * data2, unsigned int size, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3);
	void checkfornotequal(string message, unsigned int data1, unsigned int data2, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3);
	void checkfornotequalbyerrorwindow(string message, unsigned int data1, unsigned int data2, unsigned int errorwindow);
	void checkforoverlap(string message, keyvalue_t * keyvalues, unsigned int size);
	void checkforgreaterthan(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size);
	void checkforgreaterthan(string message, value_t * values1, value_t * values2, unsigned int size);
	void checkforequal(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size);
	void checkforlessthanthan(string message, unsigned int data1, unsigned int data2);
	void checkforlessthanthan(string message, unsigned int data1, unsigned int data2, unsigned int tolerance);
	void checkfordivisibleby(string message, unsigned int data, unsigned int n);
	void print1(string messagea, unsigned int dataa);
	void print2(string messagea, string messageb, unsigned int dataa, unsigned int datab);
	void print4(string messagea, string messageb, string messagec, string messaged, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad);
	void print5(string messagea, string messageb, string messagec, string messaged, string messagee, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae);
	void print6(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int datef);
	void print7(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, string messageg, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int dataf, unsigned int datag);
	void print8(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, string messageg, string messageh, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int dataf, unsigned int datag, unsigned int datah);
	void printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size);
	void printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size, unsigned int skipsize);
	void printkeyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size);
	void printkeyvalues(string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int size);
	void printkeyvalues(string message, uint512_dt * keyvalues, unsigned int size_kvs);
	void printvaluecount(string message, keyvalue_t * keyvalues, unsigned int size);
	void printvalues(string message, unsigned int * values, unsigned int size);
	void printvalueslessthan(string message, value_t * values, unsigned int size, unsigned int data);
	void printparameters();
	void printglobalvars();
	void printglobalparameters(string message, globalparams_t globalparams);
	void printpartitionresult(unsigned int enable, uint512_dt * kvdram, keyvalue_t * globaldestoffsets, keyvalue_t * globalstatsbuffer, sweepparams_t sweepparams);
	void printpartitionresult2(unsigned int enable, uint512_dt * kvdram, keyvalue_t * globalstatsbuffer, sweepparams_t sweepparams);
	
	unsigned int ugetvaluecount(keyvalue_t * keyvalues, unsigned int size);
	unsigned int getvaluecountexcept(keyvalue_t * keyvalues, unsigned int size, unsigned int exceptvalue);
	unsigned int getvaluecountexcept(string message, keyvalue_t * keyvalues, unsigned int size, unsigned int exceptvalue);
	void setkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size, keyvalue_t keyvalue);
	void clearglobalvars();
	void IsEqual(keyvalue_t ** data1, keyvalue_t ** data2, unsigned int _1stdimsize, unsigned int _2nddimsize);
	void scankeyvalues(string message, keyvalue_t * keyvalues, keyvalue_t * stats, unsigned int numberofpartitions, unsigned int rangeperpartition, unsigned int upperlimit);
	unsigned int geterrorkeyvalues(keyvalue_t * keyvalues, unsigned int begin, unsigned int end, unsigned int lowerrangeindex, unsigned int upperrangeindex);
	void setstructs(config_t _config, sweepparams_t _sweepparams, travstate_t _travstate);
	config_t getconfig();
	sweepparams_t getsweepparams();
	travstate_t gettravstate();
	unsigned int countvalues(string message, value_t * values, unsigned int size);
	void countvalueslessthan(string message, value_t * values, unsigned int size, unsigned int data);
	void checkgraph(keyvalue_t * vertexptrs, keyvalue_t * edges, unsigned int edgessize);
	void checkptr(unsigned int beginsrcvid, unsigned int endsrcvid, unsigned int beginvptr, unsigned int endvptr, keyvalue_t * edges);
	void checkptr(unsigned int beginsrcvid, unsigned int endsrcvid, unsigned int beginvptr, unsigned int endvptr, keyvalue_t * edges, unsigned int numedges);
	void resetkeyvalues(keyvalue_t * keyvalues, unsigned int size);
	void resetvalues(keyvalue_t * keyvalues, unsigned int size);
	void copykeyvalues(keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size);
	
	void globalstats_countkvstatsread(unsigned int count);
	void globalvar_collectstats_counttotalkvsread(unsigned int count);
	void globalvar_extractcapsules_counttotalkvsread(unsigned int count);
	void globalvar_organizekeyvalues_counttotalkvsread(unsigned int count);
	void globalvar_savestats_counttotalstatswritten(unsigned int count);
	void globalvar_savepartitions_countinvalids(unsigned int count);
	void globalvar_inmemory_counttotalvalidkeyvalues(unsigned int count);
	void globalstats_countkvsread(unsigned int count);
	void globalstats_countkvswritten(unsigned int count);
	void globalstats_countkvspartitionswritten(unsigned int count);
	void globalstats_countkvspartitionswritten_actual(unsigned int count);
	void globalstats_countkvspartitioned(unsigned int count);
	void globalstats_countkvsreduced(unsigned int count);
	void globalstats_reduce_countvalidkvsreduced(unsigned int count);
	void globalstats_countkvsreducewritten(unsigned int count);
	void globalstats_countkvsprocessed(unsigned int count);
	void globalstats_processedges_countvalidkvsprocessed(unsigned int count);
	void globalstats_countkvsreadV(unsigned int count);
	void globalstats_counterrorsingetpartition(unsigned int count);
	void globalstats_counterrorsinreduce(unsigned int count);
	void globalstats_counterrorsinprocessedges(unsigned int count);
	void globalstats_countkvsmerged(unsigned int count);
	unsigned int globalstats_getcounterrorsinreduce();
	unsigned int globalstats_getcounterrorsinprocessedges();
	unsigned int globalstats_getcountnumvalidprocessedges();
	
	partition_type getpartition(keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow);
	void checkn(string message, keyvalue_t * kvA0, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int n);
	void checkn2(string message, keyvalue_t * kvA0, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int n);
	void concatenate2keyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, skeyvalue_t * stats, keyvalue_t * BIGkeyvalues, skeyvalue_t * BIGstats, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow);
	void concatenate4keyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, keyvalue_t * keyvalues3, keyvalue_t * keyvalues4, skeyvalue_t * stats, keyvalue_t * BIGkeyvalues, skeyvalue_t * BIGstats, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow);						
	void concatenate8keyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, keyvalue_t * keyvalues3, keyvalue_t * keyvalues4, keyvalue_t * keyvalues5, keyvalue_t * keyvalues6, keyvalue_t * keyvalues7, keyvalue_t * keyvalues8, skeyvalue_t * stats, keyvalue_t * BIGkeyvalues, skeyvalue_t * BIGstats, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow);
	
	void printprofileso1(string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[8][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]);
	void printprofileso2(string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[4][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]);
	void printprofileso4(string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[2][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]);
	void printprofileso8(string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]);
	void printprofile(string message, keyvalue_t * keyvalues, skeyvalue_t * stats, unsigned int size, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]);
	void printprofile(string message, keyvalue_t * buffer, unsigned int size, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow);
	
	void getprofileso1(string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[8][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]);
	void getprofileso2(string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[4][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]);
	void getprofileso4(string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[2][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]);
	void getprofileso8(string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]);
	void checkprofile(string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int size_kvs, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int factor, unsigned int totalnum);
	void checkbufferprofile(string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t stats[NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow);
	void compareprofiles(string message, keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow);
	unsigned int countkeysbelongingtopartition(unsigned int p, keyvalue_t * buffer, unsigned int size, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow);
						
	void collectstats(keyvalue_t sourcebuffer[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int chunksize_kvs, step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int x, unsigned int y);
	void collectstats(keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t stats[NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int x, unsigned int y);
	void collectstats(keyvalue_t * keyvalues, keyvalue_t localstats[NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int x, unsigned int y);
	void collectstats(keyvalue_t localcapsule[NUM_PARTITIONS], unsigned int x, unsigned int y);
	void collectstats(keyvalue_t * kvdram, unsigned int size, step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int x, unsigned int y);
	unsigned int * getstats(unsigned int x, unsigned int y);
	void clearallstats();
	void clearstats(unsigned int x);
	keyvalue_t * getmykeyvalues(unsigned int x);
	unsigned int getmincutoffseen();
	unsigned int getmaxcutoffseen();
	void updatemincutoffseen(unsigned int val);
	unsigned int updatemaxcutoffseen(unsigned int val);
	
	void intrarunpipelinecheck_shifting(string message, 
							keyvalue_t bufferA[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t buffer1capsule[8][NUM_PARTITIONS], 
							keyvalue_t bufferB[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferBcapsule[4][NUM_PARTITIONS],
							keyvalue_t bufferC[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferCcapsule[2][NUM_PARTITIONS],
							keyvalue_t bufferD[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferDcapsule[NUM_PARTITIONS],
							unsigned int partitioncountso1[NUM_PARTITIONS], 
							unsigned int partitioncountso2[NUM_PARTITIONS],
							unsigned int partitioncountso4[NUM_PARTITIONS],
							unsigned int partitioncountso8[NUM_PARTITIONS],
							unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow);
	void intrapartitioncheck();
	void postpartitioncheck(uint512_dt * kvdram, keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], travstate_t ptravstate, sweepparams_t sweepparams, globalparams_t globalparams);
	
private:
	#ifdef SW 
	utility * utilityobj;
	#endif 
	
	unsigned int globalvar_totalkvstatsread;
	unsigned int globalvar_totalkvsreadV;
	unsigned int globalvar_collectstats_totalkvsread;
	unsigned int globalvar_extractcapsules_totalkvsread;
	unsigned int globalvar_organizekeyvalues_totalkvsread;
	unsigned int globalvar_savestats_totalstatswritten;
	unsigned int globalvar_savepartitions_invalids;
	unsigned int globalvar_inmemory_totalvalidkeyvalues;
	unsigned int globalvar_totalkvsread;
	unsigned int globalstats_totalkvswritten;
	unsigned int globalstats_totalkvspartitioned;
	unsigned int globalstats_totalkvspartitionswritten;
	unsigned int globalstats_totalkvspartitionswritten_actual;
	unsigned int globalstats_totalkvsreduced;
	unsigned int globalstats_reduce_validkvsreduced;
	unsigned int globalstats_totalkvsprocessed;
	unsigned int globalstats_processedges_validkvsprocessed;
	unsigned int globalstats_totalkvsreducewritten;
	unsigned int globalvar_errorsingetpartition;
	unsigned int globalvar_errorsinreduce;
	unsigned int globalvar_errorsinprocessedges;
	unsigned int globalstats_totalkvsmerged;
	
	config_t config;
	sweepparams_t sweepparams;
	travstate_t travstate;
	
	unsigned int mystats[8][MYSTATSYSIZE][NUM_PARTITIONS];
	keyvalue_t mykeyvalues[8][NUM_PARTITIONS];
	unsigned int mincutoffseen;
	unsigned int maxcutoffseen;
};
#endif







