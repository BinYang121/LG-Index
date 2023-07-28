// data.cpp
// Bin Yang, July 2021
#include <iostream>
#include <algorithm>
#include <math.h>
#include "data.h"	

void Data::readf ( FILE *input ) {
	Tid tid2 = 0;
	graphs.resize ( 4 );
	sorted_graphs.resize ( 4 );
	graphs_qsi.resize ( 4 );
	sorted_graphs_qsi.resize ( 4 );
	sortdgraphsidx.resize ( 4 );
	sortdgraphsidx_nums.resize ( 4 );
    
	char command;
	command = Utility1::readcommand ( input );
	
	while ( !feof ( input ) ) {
		readFeature ( input, tid2, command );
		tid2++;	
	}
}

void Data::readFeature ( FILE *input, Tid tid, char command ) {
	
	DFeatureInforPtr graph = new DFeatureInfor ( tid );
	
	int id;
	int nodessize = 0, edgessize = 0;
	while ( command == 't' ) {
		id = Utility1::readint ( input );
	    if ( tid != id ) {
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
		
		graph->nodes.push_back ( dummy );
		graph->nodelabels.push_back ( inputnodelabel );
		
	    if ( dummy != nodessize ) {
			cerr << "Error reading input file - node number does not correspond to its position." << endl;
			exit (1);
		}
		nodessize++;
			
		command = Utility1::readcommand ( input );
	}
	
	graph->numv = nodessize;
	if ( graph->adj.size () != nodessize ) {
		graph->adj.resize ( nodessize );
	}
	for ( int i = 0; i < nodessize; i++ ) {
	    graph->adj[i].resize ( nodessize ); 			
	}
	
    InputEdgeLabel inputedgelabel;
    InputNodeId nodeid1, nodeid2;
	while ( command == 'e' ) {
		nodeid1 = Utility1::readint ( input );
		nodeid2 = Utility1::readint ( input );
		inputedgelabel = Utility1::readint ( input ) + 1;
		
		graph->fromnodes.push_back ( nodeid1 );
		graph->tonodes.push_back ( nodeid2 );
		graph->edgelabels.push_back ( inputedgelabel );
		
		graph->adj[nodeid1][nodeid2] = inputedgelabel;
		graph->adj[nodeid2][nodeid1] = inputedgelabel;
		
		edgessize++;
		
		command = Utility1::readcommand ( input );
	}
	graph->nume = edgessize;
	
	vector<vector<double> > hidden_rep;
	hidden_rep = GINNet::net_ffm ( graph->nodelabels, graph->adj, para_ffm.model_para );
	double wlvalue = GINNet::ffm_reg ( hidden_rep, para_ffm.model_para );
	int int_wlvalue = floor ( wlvalue * 100000 );
	graph->wlvalue = double ( int_wlvalue ) / double ( 100000 );
	
	string tmp = to_string ( graph->wlvalue );
	if ( !( bf.exists ( tmp.c_str () ) ) ) {
		bf.add ( tmp.c_str () );
	}
	
	int freqincluster;
	while ( command == 'k' ) {
		freqincluster = Utility1::readint ( input );
		graph->freqincluster = freqincluster;
		
		command = Utility1::readcommand ( input );
	}
	
	int cluster;
	while ( command == 'c' ) {
		for ( int i = 0; i < freqincluster; i++ ) {
		    cluster = Utility1::readint ( input );
		    graph->cluster = cluster;

		    graphs[cluster].push_back ( graph );
		
		    // graphs_qsi
            TGraph::Graph* graph_qsi = new TGraph::Graph ( graph->numv, graph->nume );
	        graphs_qsi[cluster].push_back ( graph_qsi );
	        graph_qsi->id = to_string( graph->tid );
	        for ( int i = 1; i < graph->numv + 1; i++ ) {
		        graph_qsi->label[i] = graph->nodelabels[i-1]; 
	        }
	        for ( int j = 0; j < graph->nume; j++ ) {
		        graph_qsi->ins ( graph->fromnodes[j] + 1, graph->tonodes[j] + 1 );
	        }	
		}
		
		command = Utility1::readcommand ( input );
	}
	
	int freqindata;
	while ( command == 'f' ) {
		freqindata = Utility1::readint ( input );
		graph->freqindata = freqindata;
		
		command = Utility1::readcommand ( input );
	}
	
	int idin;
	while ( !feof ( input ) && command == 'i' ) {
		for ( int i = 0; i < freqindata; i++ ) {
			idin = Utility1::readint ( input );
			graph->idindata.push_back ( idin );	
		}
		
		command = Utility1::readcommand ( input );		
	}
}

bool compare ( sortidx a, sortidx b ) {
	if ( a.value < b.value ) {
		return true;
	} else {
		return false;
	}
}
 
void Data::reorderf ( vector<vector<DFeatureInforPtr> > &graphs ) {
	for ( int i = 0; i < graphs.size(); i++ ) {
	    sortdgraphsidx[i].resize ( graphs[i].size() );
	    for ( int j = 0; j < graphs[i].size(); j++ ) {
		    sortdgraphsidx[i][j].index = j;
		    sortdgraphsidx[i][j].value = graphs[i][j]->wlvalue;
	    }
	    sort ( sortdgraphsidx[i].begin(), sortdgraphsidx[i].end(), compare );
	    for ( int k = 0; k < graphs[i].size (); k++ ) {
		    sorted_graphs[i].push_back ( graphs[i][sortdgraphsidx[i][k].index] );
		    sorted_graphs_qsi[i].push_back ( graphs_qsi[i][sortdgraphsidx[i][k].index] );
	    }
	
	    for ( int l = 0; l < sorted_graphs[i].size (); l++ ) {
		    int forward = 0;
		    for ( int m = l - 1; m >= -1; m-- ) {
			    if ( m >= 0 && ( sorted_graphs[i][l]->wlvalue == sorted_graphs[i][m]->wlvalue ) ) {
				    forward++;
			    } else {
				    sorted_graphs[i][l]->forward = forward;
				    break;
			    }
		    }
		    int backward = 0;
		    for ( int n = l + 1; n <= sorted_graphs[i].size (); n++ ) {
			    if ( n <= ( sorted_graphs[i].size () - 1 ) && ( sorted_graphs[i][l]->wlvalue == sorted_graphs[i][n]->wlvalue ) ) {
				    backward++;
			    } else {
				    sorted_graphs[i][l]->backward = backward;
				    break;
			    }
		    }	
	    }
	
	    for (int p = 0; p < data.sortdgraphsidx[i].size(); p++ ) {
		    sortdgraphsidx_nums[i].push_back ( data.sortdgraphsidx[i][p].value );
	    }
	}
}

void Data::read_statistic ( FILE *input ) {
	while ( !feof ( input ) ) {
		char command;
	    command = Utility1::readcommand ( input );
		while ( command == 'c' ) {
			int cluster = Utility1::readint ( input );
			int num = Utility1::readint ( input );
			int min_value = Utility1::readint ( input );
			int max_value = Utility1::readint ( input );
			int cluster_center = Utility1::readint ( input );
			
			cluster_centers.push_back ( cluster_center );
			
			command = Utility1::readcommand ( input );
		}
	}
	
	// Check
	for ( int i = 0; i < cluster_centers.size () - 1; i++ ) {
		if ( cluster_centers[i] >= cluster_centers[i+1] ) {
			cerr << "Error reading statistic.out file." << endl;
			exit (1); 
		}
	}
}


















