#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/heuristics/heuristics.h"
#include "../../include/common.h"
#include "creategraph.h"
#include "create2Dgraph.h"
using namespace std;

creategraph::creategraph(){}
creategraph::~creategraph(){} 

void creategraph::create2Dgraf(unsigned int algorithmid, unsigned int datasetid){
	algorithm * algorithmobj = new algorithm();
	heuristics * heuristicsobj = new heuristics();
	graph * graphobj = new graph(algorithmobj, datasetid, heuristicsobj->getdefaultnumvertexbanks(), heuristicsobj->getdefaultnumedgebanks());
	
	create2Dgraph * create2Dgraphobj = new create2Dgraph(graphobj, NAp);
	create2Dgraphobj->run();

	cout<<"creategraph:: finished generating 2D graph."<<endl;
}

void creategraph::analyzegraf(unsigned int algorithmid, unsigned int datasetid){
	algorithm * algorithmobj = new algorithm();
	heuristics * heuristicsobj = new heuristics();
	graph * graphobj = new graph(algorithmobj, datasetid, heuristicsobj->getdefaultnumvertexbanks(), heuristicsobj->getdefaultnumedgebanks());
	
	cout<<"graphobj->getnumedgebanks(): "<<graphobj->getnumedgebanks()<<endl;
	create2Dgraph * create2Dgraphobj = new create2Dgraph(graphobj);
	create2Dgraphobj->analyzegraph();
	create2Dgraphobj->transformgraph();
	cout<<"creategraph:: finished analyzing graph."<<endl;
}


