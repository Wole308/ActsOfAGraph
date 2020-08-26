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
using namespace std;

creategraph::creategraph(){}
creategraph::~creategraph(){} 

void creategraph::create2Dgraph(unsigned int algorithmid, unsigned int datasetid){
	algorithm * algorithmobj = new algorithm();
	heuristics * heuristicsobj = new heuristics();
	graph * graphobj = new graph(algorithmobj, datasetid, heuristicsobj->getdefaultnumvertexbanks(), heuristicsobj->getdefaultnumedgebanks());
	
	cout<<"graphobj->getnumedgebanks(): "<<graphobj->getnumedgebanks()<<endl;
	if(graphobj->getnumedgebanks() == 16){		
		create2Dgraph * create2Dgraphobj = new create2Dgraph(graphobj, 0, (graphobj->getnumedgebanks() / 4));
		create2Dgraphobj->start();
		
		create2Dgraphobj->reload((graphobj->getnumedgebanks() / 4), (graphobj->getnumedgebanks() / 4));
		create2Dgraphobj->start();
		
		create2Dgraphobj->reload((graphobj->getnumedgebanks() / 2), (graphobj->getnumedgebanks() / 4));
		create2Dgraphobj->start();
		
		create2Dgraphobj->reload(((graphobj->getnumedgebanks() * 3) / 4), (graphobj->getnumedgebanks() / 4));
		create2Dgraphobj->start();
		
	} else if (graphobj->getnumedgebanks() == 4){		
		create2Dgraph * create2Dgraphobj = new create2Dgraph(graphobj, 0, (graphobj->getnumedgebanks() / 2));
		create2Dgraphobj->start();
		
		create2Dgraphobj->reload((graphobj->getnumedgebanks() / 2), (graphobj->getnumedgebanks() / 2));
		create2Dgraphobj->start();
		
	} else if (graphobj->getnumedgebanks() == 1){
		create2Dgraph * create2Dgraphobj = new create2Dgraph(graphobj, 0, graphobj->getnumedgebanks());
		create2Dgraphobj->start();
	}	
	cout<<"creategraph:: finished generating 2D graph."<<endl;
}


