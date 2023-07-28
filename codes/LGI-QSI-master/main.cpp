// main.cpp
// Bin Yang, July 2021.
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <set>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include "data.h"
#include "graphRD.h"
#include "utility1.h"
#include "utility2.h"
#include "Utilitycfql.h"
#include "match.h"
#include "misc.h"
#include "path.h"
#include "database.h"
#include "graphstate.h"
#include "closeleg.h"
#include "legoccurrence.h"
#include "FSCCommand.h"
#include "GraphDB.h"
#include "FSC.h"
#include "GraphDS.h"
#include "paraload.h"
#include "GINNet.h"
#include "bloomfilter.h"
#include "QuickSI.cpp" 
#include "RandomCLoquatForests.h"
#include "UserInteraction2.h"

using namespace std;

GraphRD graphrd;
Data data;
Para para_ffm;
Para para_assp_0, para_assp_1, para_assp_2;
Frequency minfreq = 1;
int phase = 3;
bool dooutput = false;
int maxsize = ( 1 << ( sizeof(NodeId)*8 ) ) - 1;
int counter;
int elem_num = 10000000;
BloomFilter bf = BloomFilter( elem_num );

FILE *output;

void Statistics::clear () {
	frequenttreenumbers.clear ();
	frequentpathnumbers.clear ();
	frequentgraphnumbers.clear ();
}

int match ( QFeatureInforPtr qfeature, TGraph::Graph* feature_qsi, int cluster ) {
	int result;
	int firstIndex = Match::find ( true, data.sortdgraphsidx_nums[cluster], qfeature->wlvalue );
	if ( firstIndex != -1 ) {
		vector<int> didxs;
	    for (int i = 0; i < data.sorted_graphs[cluster][firstIndex]->backward + 1; i++ ) {
		    didxs.push_back ( firstIndex + i );	
	    }		
		
	    for ( int i = 0; i < didxs.size(); i++ ) {
			vector<TGraph::Graph*> gs_i;
			gs_i.push_back ( data.sorted_graphs_qsi[cluster][didxs[i]] );
			initialize( 1, gs_i );
		    TGraph::Graph &P_i = *( feature_qsi ), &G_i = *gs_i[0];
			bool result = solve( P_i, G_i );
		    if ( result == 1 ) {
			    return didxs[i];
			    break;
		    }
	    }
	    if ( result == 0 ) {
		   return -1;
	    }
	}
	else {
		return -1;
	}
}

//./lgi 1 data/queries.out data/dataset.out data/indexes.out data/queries_result.out 583.3433 0.245 0.2331
int main( int argc, char *argv[] ){
	
	cerr << "Query processing program" << endl;
	cerr << "Version 1.0" << endl;
	cerr << "Bin Yang, Oct 2022" << endl;
	
	dooutput = true;
	minfreq = atoi ( argv[optind] );
	
	FILE *inputqs = fopen ( argv[optind+1], "r" );
	FILE *inputd = fopen ( argv[optind+2], "r" );
	graphrd.read ( inputqs, "qs" ); 
    graphrd.read ( inputd, "d" );	
	fclose ( inputqs );
	fclose ( inputd );
	
	LoquatCForest* loquatCForest = NULL;
	int rv = BuildRandomClassificationForestModel("data/RFModelfile.xml", 0/*filetype:xml*/, loquatCForest);
	
	string query_graph_path = argv[optind+1];
    string data_graph_path = argv[optind+2];
    GraphFormat data_graph_format = GraphFormat::FB;
    GraphFormat query_graph_format = GraphFormat::FB;
	GraphDB graphDB;
    graphDB.BuildDB(data_graph_path, data_graph_format);
    vector<Graphcfql*>* data_graphs = graphDB.GetDataGraphs();
    vector<Graphcfql*>* query_graphs = graphDB.LoadQueryGraphs(query_graph_path, query_graph_format);
	FSC fsc ( &graphDB );
	
	FILE *input_statistic = fopen ( "data/statistic.out", "r" );
	data.read_statistic ( input_statistic );
	fclose ( input_statistic );
	
	ifstream in_aspredictor_0 ("data/cluster_0_para_end_assp.txt", ios::in);
    istreambuf_iterator<char> beg_aspredictor_0 ( in_aspredictor_0 ), end_aspredictor_0;
    string strdata_aspredictor_0 ( beg_aspredictor_0, end_aspredictor_0 );
    in_aspredictor_0.close(); 
    para_assp_0.split ( strdata_aspredictor_0 );
	
	ifstream in_aspredictor_1 ("data/cluster_1_para_end_assp.txt", ios::in);
    istreambuf_iterator<char> beg_aspredictor_1 ( in_aspredictor_1 ), end_aspredictor_1;
    string strdata_aspredictor_1 ( beg_aspredictor_1, end_aspredictor_1 );
    in_aspredictor_1.close(); 
    para_assp_1.split ( strdata_aspredictor_1 );
	
	ifstream in_aspredictor_2 ("data/cluster_2_para_end_assp.txt", ios::in);
    istreambuf_iterator<char> beg_aspredictor_2 ( in_aspredictor_2 ), end_aspredictor_2;
    string strdata_aspredictor_2 ( beg_aspredictor_2, end_aspredictor_2 );
    in_aspredictor_2.close(); 
    para_assp_2.split ( strdata_aspredictor_2 );
	vector<Para> para_assps = { para_assp_0, para_assp_1, para_assp_2 };
	
	ifstream in_ffm ("data/model_para_end_ffm.txt", ios::in);
    istreambuf_iterator<char> beg_ffm ( in_ffm ), end_ffm;
    string strdata_ffm ( beg_ffm, end_ffm );
    in_ffm.close(); 
    para_ffm.split ( strdata_ffm );
	
	FILE *inputdf = fopen ( argv[optind+3], "r" );
	data.readf ( inputdf );
	data.reorderf ( data.graphs );
	fclose ( inputdf );
	
	FILE *output = fopen ( argv[optind+4], "w" );
	
	vector<float> maes = { atof(argv[optind+5]), atof(argv[optind+6]), atof(argv[optind+7]) };
	double pre_error = 0.02;
	double totle_candidate_time = 0;
    double totle_verification_time = 0;
	double totle_notemptycand_verification_time = 0;
	double totle_emptycand_verification_time = 0;
	double totle_process_time = 0;
	double notemptycand_filtering_precision = 0;
	double total_filtering_precision = 0;
	int empty_candidate_set = 0; 
	vector<TGraph::Graph*> qs;
    vector<TGraph::Graph*> gs;
	for ( int i = 0; i < graphrd.querys.size (); i++ ){
		cout << "-------------------------------------------------------------------" << endl;
		cout << "Query ID: " << i << endl;
		cerr << "Gaston..." << endl;
		timeval t1 = Utilitycfql::GetTime();
		counter = -1;
		database.readTree ( graphrd.querys[i] );
		database.edgecount ();
        database.reorder ();
		initLegStatics ();
        graphstate.init ();
		int group_size = 0;
		int candidate_flag = 0;
		
		// Random forests
		set<int> nodelabels_types;
		for ( int j = 0; j < graphrd.querys[i]->numv; j++ ) {
			nodelabels_types.insert ( graphrd.querys[i]->qnodelabels[j] );
		}
		int total_degree = 0;
        for ( int j = 0; j < graphrd.querys[i]->numv; j++ ) {
		    total_degree = total_degree + graphrd.querys[i]->degrees[j];
	    }			
		float avg_degree = ( (float) total_degree ) / ( (float) graphrd.querys[i]->numv );
		int att_dim = 4;
		float* rf_data_test = new float [att_dim];
	    rf_data_test[0] = graphrd.querys[i]->numv;
        rf_data_test[1] = graphrd.querys[i]->nume;
	    rf_data_test[2] = nodelabels_types.size ();
	    rf_data_test[3] = avg_degree;
		int pred_cluster;
        EvaluateOneSample(rf_data_test, loquatCForest, pred_cluster /*OUT*/, 1);
		
		// Query result size estimation
		vector<double> firstagg = Utility1::agg_convert ( graphrd.querys[i]->aggnodelabels, graphrd.querys[i]->adj );
	    double wlvalue = 0;
	    for ( int j = 0; j < firstagg.size (); j++ ) {
		    wlvalue += firstagg[j];
	    }
		vector<double> dist_clusters;
		for ( int j = 0; j < data.cluster_centers.size (); j++ ) {
			double dist_cluster = abs ( data.cluster_centers[j] - wlvalue );
			dist_clusters.push_back ( dist_cluster );
		}
		auto minap = min_element ( dist_clusters.begin(), dist_clusters.end() );
		int cluster_Pminf = minap - dist_clusters.begin();	
		// estimation model
		float mae = maes[cluster_Pminf];
		double ps = GINNet::net_aspredictor ( graphrd.querys[i]->qnodelabels, graphrd.querys[i]->adj, para_assps[cluster_Pminf].model_para );
		
		vector<int> candidate_set;
		for (int i1 = 0; i1 < graphrd.D.size (); i1++ ) {
		    candidate_set.push_back ( i1 );
	    }
        for ( int j = 0; j < database.nodelabels.size (); j++ ) {
            if ( database.nodelabels[j].frequency >= minfreq &&
                database.nodelabels[j].frequentedgelabels.size () ) {
                Path path ( j );
                path.expand ();
				
				vector<QFeatureInforPtr> ordered_features;
				vector<QFeatureInforPtr> features_group;
				for ( int n = group_size; n < ( graphstate.features.size () - group_size ); n++ ) {
					features_group.push_back ( graphstate.features[n] );
				}
				group_size = graphstate.features.size (); 
		        ordered_features = Utility2::qf_reorder ( features_group, graphrd.querys[i]->numv );
				for (int i2 = 0; i2 < ordered_features.size (); i2++ ){
                    ordered_features[i2]->adj.resize ( ordered_features[i2]->numv );
	                for ( int k = 0; k < ordered_features[i2]->numv; k++ ) {
	                    ordered_features[i2]->adj[k].resize ( ordered_features[i2]->numv ); 			
	                }
		            for ( int n = 0; n < ordered_features[i2]->nume; n++ ) {
			            ordered_features[i2]->adj[ordered_features[i2]->fromnodes[n]][ordered_features[i2]->tonodes[n]] = ordered_features[i2]->edgelabels[n];
		                ordered_features[i2]->adj[ordered_features[i2]->tonodes[n]][ordered_features[i2]->fromnodes[n]] = ordered_features[i2]->edgelabels[n];
		            }
					
					// Filtering and Fingerprinting
					vector<vector<double> > hidden_rep;
                	hidden_rep = GINNet::net_ffm ( ordered_features[i2]->nodelabels, ordered_features[i2]->adj, para_ffm.model_para );
                    vector<double> result_class = GINNet::ffm_class ( hidden_rep, para_ffm.model_para );
                    auto valueIter = max_element ( result_class.begin (), result_class.end () );
	                int index = distance ( result_class.begin (), valueIter );
					double wlvalue = GINNet::ffm_reg ( hidden_rep, para_ffm.model_para );
					int int_wlvalue = floor ( wlvalue * 100000 );
	                ordered_features[i2]->wlvalue = double ( int_wlvalue ) / double ( 100000 );
					if ( index == 1 ) {
                        // feature_qsi
                        TGraph::Graph* feature_qsi = new TGraph::Graph ( ordered_features[i2]->numv, ordered_features[i2]->nume );
                        // feature_qsi->id = to_string( ordered_features[i2]->tid );
                        for ( int i = 1; i < ordered_features[i2]->numv + 1; i++ ) {
	                        feature_qsi->label[i] = ordered_features[i2]->nodelabels[i-1]; 
                        }
                        for ( int j = 0; j < ordered_features[i2]->nume; j++ ) {
                            feature_qsi->ins ( ordered_features[i2]->fromnodes[j] + 1, ordered_features[i2]->tonodes[j] + 1 );
                        }

		                // match
	                    int matched_featureid = match ( ordered_features[i2], feature_qsi, pred_cluster );
                        if ( matched_featureid != -1 ) {
				            candidate_flag++;
			                vector<int> matched_idindata = data.sorted_graphs[pred_cluster][matched_featureid]->idindata;
			                candidate_set = Utility2::vectors_intersection ( matched_idindata, candidate_set ); 
		                }	
		                if (  1 - ( ps + mae ) / candidate_set.size () <= pre_error || 1 - ( ps - mae ) / candidate_set.size () <= pre_error ) {
		                    break;
	                    }
					} else {
						string tmp = to_string ( ordered_features[i2]->wlvalue );	
                        if ( bf.exists ( tmp.c_str () ) ) {						
	                        // feature_qsi
                            TGraph::Graph* feature_qsi = new TGraph::Graph ( ordered_features[i2]->numv, ordered_features[i2]->nume );
                            // feature_qsi->id = to_string( ordered_features[i2]->tid );
                            for ( int i = 1; i < ordered_features[i2]->numv + 1; i++ ) {
	                            feature_qsi->label[i] = ordered_features[i2]->nodelabels[i-1]; 
                            }
                            for ( int j = 0; j < ordered_features[i2]->nume; j++ ) {
                                feature_qsi->ins ( ordered_features[i2]->fromnodes[j] + 1, ordered_features[i2]->tonodes[j] + 1 );
                            }

		                    // match
	                        int matched_featureid = match ( ordered_features[i2], feature_qsi, pred_cluster );
                            if ( matched_featureid != -1 ) {
				                candidate_flag++;
			                    vector<int> matched_idindata = data.sorted_graphs[pred_cluster][matched_featureid]->idindata;
			                    candidate_set = Utility2::vectors_intersection ( matched_idindata, candidate_set ); 
		                    }	
		                    if (  1 - ( ps + mae ) / candidate_set.size () <= pre_error || 1 - ( ps - mae ) / candidate_set.size () <= pre_error ) {
		                        break;
	                        }
						}	
					}				
                }
                if ( 1 - ( ps + mae ) / candidate_set.size () <= pre_error || 1 - ( ps - mae ) / candidate_set.size () <= pre_error ) {
		            break;
	            }					
            }
        }
		timeval t2 = Utilitycfql::GetTime ();
		double temp_t2_t1 = Utilitycfql::TimeDiffInMicroseconds ( t1, t2 );
	    totle_candidate_time += temp_t2_t1;
		
		cerr << "Verification..." << endl;
		// query_result
		int verification_count = 0;
		vector<int> verification_set;
        if ( candidate_flag != 0 && candidate_set.size () != 0 ) {
			timeval t3 = Utilitycfql::GetTime ();
			qs.push_back ( graphrd.querys_qsi[i] );
			for ( int m1 = 0; m1 < candidate_set.size (); m1++ ) {
				gs.push_back ( graphrd.D_qsi[candidate_set[m1]] );
			}
            initialize( candidate_set.size (), gs );
            for ( int n1 = 0; n1 < candidate_set.size (); n1++ ) {
                TGraph::Graph &P = *qs[0], &G = *gs[n1];     
                bool res = solve( P, G );
                if ( res == 1 ) {
					verification_set.push_back ( int ( atoi ( G.id.c_str() ) ) );
				}
            }
			
			timeval t4 = Utilitycfql::GetTime ();
			double temp_t4_t3 = Utilitycfql::TimeDiffInMicroseconds ( t3, t4 );
			double notemptycand_verification_time = temp_t4_t3;
			totle_verification_time += notemptycand_verification_time;
			totle_notemptycand_verification_time += notemptycand_verification_time;
			verification_count = verification_set.size ();
			total_filtering_precision += ( (double) verification_count ) / ( (double) candidate_set.size () );	
            notemptycand_filtering_precision += ( (double) verification_count ) / ( (double) candidate_set.size () );			
            Utility2::print ( output, graphrd.querys[i], verification_set, verification_count );
		} else if ( candidate_flag != 0 && candidate_set.size () == 0 ) {
			double notemptycand_verification_time = 0;
			totle_verification_time += notemptycand_verification_time;
			totle_notemptycand_verification_time += notemptycand_verification_time;
			total_filtering_precision += 1;
			notemptycand_filtering_precision += 1;
			Utility2::print ( output, graphrd.querys[i], verification_set, verification_count );
		} else {
			timeval t5 = Utilitycfql::GetTime ();
			cout << "candidate_set is empty" << endl;
			empty_candidate_set += 1;
			verification_set.resize ( data_graphs->size() );
            // Process the query graphs.
			verification_set = fsc.Query((*query_graphs)[i], verification_set, verification_count, i );
			
			timeval t6 = Utilitycfql::GetTime ();
			double temp_t6_t5 = Utilitycfql::TimeDiffInMicroseconds ( t5, t6 );
			double emptycand_verification_time = temp_t6_t5;
			totle_verification_time += emptycand_verification_time;
			totle_emptycand_verification_time += emptycand_verification_time;
			total_filtering_precision += ( (double) verification_count ) / ( (double) candidate_set.size () );
			
			Utility2::print ( output, graphrd.querys[i], verification_set, verification_count );
			fsc.ClearQueryGraphResource();
		}
		
		// Clear Gaston's global variable. 
		database.clear ();
		statistics.clear ();
		graphstate.clear ();
		closelegoccurrences.elements.clear ();
		closelegsoccsused = true;
		candidatecloselegsoccs.clear ();
		candidatecloselegsoccsused.clear ();
		legoccurrences.elements.clear ();
		candidatelegsoccurrences.clear ();
		
		// Clear rf_data_test
   	    delete[] rf_data_test;
		
		// Clear QuickSI's vectors.
		qs.clear ();
        gs.clear ();
		
		// Clear verification_set.
		verification_set.clear ();
	}
	
	// clear the memory allocated for the entire forest
	ReleaseClassificationForest(&loquatCForest);
	
	totle_process_time = totle_candidate_time + totle_verification_time;
	
	cout << "-------------------------------------------------------------------" << endl;
    cout << "The average processing time: " << totle_process_time / graphrd.querys.size () << " us." << endl;
    cout << "The average filtering time: " << totle_candidate_time / graphrd.querys.size () << " us." << endl;
    cout << "The average verification time: " << totle_verification_time / graphrd.querys.size () << " us." << endl;
	cout << "The average verification time due to not empty candidate set: " << totle_notemptycand_verification_time / ( graphrd.querys.size () - empty_candidate_set ) << " us." << endl;
    cout << "The average verification time due to empty candidate set: " << totle_emptycand_verification_time / empty_candidate_set << " us." << endl;
    cout << "The number of empty_candidate_set: " << empty_candidate_set << endl;
	cout << "The average notemptycand filtering precision: " << setprecision(6) << notemptycand_filtering_precision / ( (double) graphrd.querys.size () - (double) empty_candidate_set ) << endl;
	cout << "The average total filtering precision: " << setprecision(6) << total_filtering_precision / ( (double) graphrd.querys.size () ) << endl;
	
    fclose ( output );
}
