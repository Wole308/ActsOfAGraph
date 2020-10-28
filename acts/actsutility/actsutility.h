#ifndef ACTSUTILITY_H
#define ACTSUTILITY_H
#include "../../include/config_params.h"
#ifdef SW
#include "../../src/utility/utility.h"
#endif 
#include "../include/actscommon.h"
#include "../../include/common.h"
using namespace std;

class actsutility {
public:

	actsutility();
	~actsutility();
	
	void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3);
	void checkforequal(string message, unsigned int data1, unsigned int data2);
	void checkfornotequal(string message, unsigned int data1, unsigned int data2);
	void checkfornotequalbyerrorwindow(string message, unsigned int data1, unsigned int data2, unsigned int errorwindow);
	void checkforoverlap(string message, keyvalue_t * keyvalues, unsigned int size);
	void checkforgreaterthan(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size);
	void print1(string messagea, unsigned int dataa);
	void print2(string messagea, string messageb, unsigned int dataa, unsigned int datab);
	void print4(string messagea, string messageb, string messagec, string messaged, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad);
	void print5(string messagea, string messageb, string messagec, string messaged, string messagee, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae);
	void print6(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int datef);
	void print7(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, string messageg, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int dataf, unsigned int datag);
	void printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size);
	void printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size, unsigned int skipsize);
	void printkeyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size);
	void printkeyvalues(string message, uint512_dt * keyvalues, unsigned int size_kvs);
	void printvaluecount(string message, keyvalue_t * keyvalues, unsigned int size);
	void printvalues(string message, unsigned int * values, unsigned int size);
	void printparameters();
	void printglobalvars();
	void printglobalparameters(string message, globalparams_t globalparams);
	void printpartitionresult(unsigned int enable, uint512_dt * kvdram, keyvalue_t * globaldestoffsets, keyvalue_t * globalstatsbuffer, sweepparams_t sweepparams);
	void printpartitionresult2(unsigned int enable, uint512_dt * kvdram, keyvalue_t * globalstatsbuffer, sweepparams_t sweepparams);
	
	unsigned int ugetvaluecount(keyvalue_t * keyvalues, unsigned int size);
	unsigned int getvaluecountexcept(keyvalue_t * keyvalues, unsigned int size, unsigned int exceptvalue);
	void setkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size, keyvalue_t keyvalue);
	void clearglobalvars();
	void IsEqual(keyvalue_t ** data1, keyvalue_t ** data2, unsigned int _1stdimsize, unsigned int _2nddimsize);
	void scankeyvalues(string message, keyvalue_t * keyvalues, keyvalue_t * stats, unsigned int numberofpartitions, unsigned int rangeperpartition, unsigned int upperlimit);
	unsigned int geterrorkeyvalues(keyvalue_t * keyvalues, unsigned int begin, unsigned int end, unsigned int lowerrangeindex, unsigned int upperrangeindex);
	void setstructs(config_t _config, sweepparams_t _sweepparams, travstate_t _travstate);
	config_t getconfig();
	sweepparams_t getsweepparams();
	travstate_t gettravstate();

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
	
	void scankeyvalues(keyvalue_t * volume, keyvalue_t * stats);

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
};
#endif







