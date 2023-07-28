#include <algorithm>
#include <iostream>
#include "graphRD.h"

void GraphRD::read ( FILE *input, string name ) {
	Tid tid = 0; 
	char command;
	command = Utility1::readcommand ( input );
	
	while ( !feof ( input ) ) {
		GraphInforPtr graph = new GraphInfor ( tid );
	    if ( name == "qs" ) {
	        querys.push_back ( graph );
	    } else if ( name == "d" ) {
		    D.push_back ( graph );
	    }
		int nodessize = 0, edgessize = 0;
		
		Tid idq;
		while ( command == 't' ) {
			idq = Utility1::readint ( input );
	        if ( tid != idq ) {
		        cerr << "Error - The disorder of input file." << endl;
                exit( 1 );		
	        }
            command = Utility1::readcommand ( input );
		}
		
		int dummy;
		InputNodeLabel inputnodelabel;
	    while ( command == 'v' ) {
		    dummy = Utility1::readint ( input );
		    inputnodelabel = Utility1::readint ( input );
		
		    graph->qnodes.push_back ( dummy );
		    graph->qnodelabels.push_back ( inputnodelabel );
		
	        if ( dummy != nodessize ) {
			    cerr << "Error reading input file - node number does not correspond to its position." << endl;
			    exit (1);
		    }
		    nodessize++;
			
		    command = Utility1::readcommand ( input );
	    }
		
		vector<double> aggnodelabels;
	    for ( int i = 0; i < nodessize; i++ ) {
		    aggnodelabels.push_back ( graph->qnodelabels[i] + 1 ); 
	    }
	    graph->aggnodelabels = aggnodelabels;
	
	    graph->numv = nodessize;
		
		if ( graph->adj.size () != nodessize ) {
		    graph->adj.resize ( nodessize );
	    }
	    for ( int i = 0; i < nodessize; i++ ) {
	        graph->adj[i].resize ( nodessize ); 			
	    }
	
        InputEdgeLabel inputedgelabel;
        InputNodeId nodeid1, nodeid2;
	    while ( !feof ( input ) && command == 'e' ) {
		    nodeid1 = Utility1::readint ( input );
		    nodeid2 = Utility1::readint ( input );
		    inputedgelabel = Utility1::readint ( input ) + 1;
		
		    graph->qfromnodes.push_back ( nodeid1 );
		    graph->qtonodes.push_back ( nodeid2 );
		    graph->qedgelabels.push_back ( inputedgelabel );
			
			graph->adj[nodeid1][nodeid2] = inputedgelabel;
		    graph->adj[nodeid2][nodeid1] = inputedgelabel;
		
		    edgessize++;
		
		    command = Utility1::readcommand ( input );
	    }
	
	    graph->nume = edgessize;
		
		for ( int m = 0; m < graph->numv; m++ ) {
		    graph->degrees.push_back ( 0 );
	    }	
	    for ( int n = 0; n < graph->nume; n++ ) {
		    graph->degrees[graph->qfromnodes[n]] = graph->degrees[graph->qfromnodes[n]] + graph->qedgelabels[n];
		    graph->degrees[graph->qtonodes[n]] = graph->degrees[graph->qtonodes[n]] + graph->qedgelabels[n];
	    }
	
		tid++;
	}		
}
