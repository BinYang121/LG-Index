// data.h
// Bin Yang, July 2021.
#ifndef DATA_H
#define DATA_H
#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include "misc.h"
#include "GraphDS.h"
#include "utility1.h"
#include "paraload.h"
#include "GINNet.h"
#include "bloomfilter.h"

using namespace std;

typedef unsigned int Tid;
extern BloomFilter bf;

struct sortidx{
	int index;
	double value;
};

struct DFeatureInfor {
  Tid tid;
  int numv;
  int nume; 
  int freqincluster;
  int cluster;
  int freqindata;
  vector<int> idindata;
  vector<vector<int> > adj;
  
  vector<int> nodes;
  vector<int> nodelabels;
  vector<int> fromnodes;
  vector<int> tonodes;
  vector<int> edgelabels;
  
  int forward;
  double wlvalue;
  int backward;

  DFeatureInfor ( Tid tid ): tid ( tid ) { }
  DFeatureInfor () { }
  
  friend ostream &operator<< ( ostream &stream, DFeatureInfor &dfeatureinfor );
};

typedef DFeatureInfor *DFeatureInforPtr;

class Data {
  public:
    Data () { }
	vector<vector<DFeatureInforPtr> > graphs;
	vector<vector<DFeatureInforPtr> > sorted_graphs;
	vector<vector<TGraph::Graph*> > graphs_qsi;
	vector<vector<TGraph::Graph*> > sorted_graphs_qsi;
	vector<vector<sortidx> > sortdgraphsidx;
	vector<vector<double> > sortdgraphsidx_nums;
	vector<int> cluster_centers;
	
    void readf ( FILE *input );
	void reorderf ( vector<vector<DFeatureInforPtr> > &graphs );
	void read_statistic ( FILE *input );
  private:
    void readFeature ( FILE *input, Tid tid, char command );
};

extern Data data;

#endif
