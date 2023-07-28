#ifndef GRAPHRD_H
#define GRAPHRD_H
#include <vector>
#include <fstream>
#include <string.h>
#include "misc.h"
#include "GraphDS.h"
#include "utility1.h"

using namespace std;

struct GraphInfor {
  Tid tid;
  int numv;
  int nume; 
  vector<int> degrees;
  vector<vector<int> > adj;
  
  vector<int> qnodes;
  vector<int> qnodelabels;
  vector<int> qfromnodes;
  vector<int> qtonodes;
  vector<int> qedgelabels;
  
  vector<double> aggnodelabels;

  GraphInfor ( Tid tid ): tid ( tid ) { }
  GraphInfor () { }
  
  friend ostream &operator<< ( ostream &stream, GraphInfor &graphinfor );
};

typedef GraphInfor *GraphInforPtr;

class GraphRD {
  public:
    GraphRD () { }
	vector<GraphInforPtr> querys;
	vector<TGraph::Graph*> querys_qsi;
	vector<GraphInforPtr> D;
	vector<TGraph::Graph*> D_qsi;
  
    void read ( FILE *input, string name );
};

extern GraphRD graphrd;

#endif
