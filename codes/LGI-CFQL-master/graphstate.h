#ifndef GRAPHSTATE_H
#define GRAPHSTATE_H
#include <vector>
#include <iostream>
#include "misc.h"
#include "database.h"
#include "closeleg.h"
#include "patterntree.h"
#include "GraphDS.h"

using namespace std;

typedef unsigned int Mark;

class GraphState;

extern GraphState graphstate;

// Bin Yang, 20b903067@stu.hit.edu.cn, Oct 2021.
struct QFeatureInfor {
  Tid tid; 
  int numv;
  int nume;
  vector<vector<int> > adj;
  
  vector<int> nodes;
  vector<int> nodelabels;
  vector<int> fromnodes;
  vector<int> tonodes;
  vector<int> edgelabels;
  
  double wlvalue;

  QFeatureInfor ( Tid tid ): tid ( tid ) { }
  QFeatureInfor () { }
  
  friend ostream &operator<< ( ostream &stream, QFeatureInfor &qfeatureinfor );
};

typedef QFeatureInfor *QFeatureInforPtr;

class GraphState {
  public:
    struct GSDeletedEdge {
      NodeId tonode, fromnode;
      EdgeLabel edgelabel;
      int postonode, posfromnode;
      bool close;
      Mark cyclemark;
    };
    vector<GSDeletedEdge> deletededges;
	
	// Bin Yang, 20b903067@stu.hit.edu.cn, Oct 2021.
	vector<QFeatureInforPtr> features;
	//vector<TGraph::Graph*> features_qsi;
	
    // the current pattern
    vector<Tuple> *treetuples;
    vector<CloseTuple> *closetuples;
    vector<NodeId> nodesinpreorder;
    int backbonelength; // the length of the backbone, in number of nodes
    int startsecondpath; // the position of the second part of the backbone in
                         // the treetuples.
    bool nasty;  // nasty == A-B-A-B-A-B -like cases
    NodeLabel centerlabel;
    EdgeLabel bicenterlabel;
    int closecount;
    bool selfdone; // set by isnormal to store whether the original graph has been
                   // normal-checked; we try to delay this until the very last moment,
                   // as we know that on this graph the normalisation procedure will
                   // have to go through all phases
    
    struct GSEdge {
      int tonode;
      int postonode; // position in the adjacency list of the corresponding reverse edge
      EdgeLabel edgelabel;
      Mark cyclemark;
      bool close; // closing edge
      GSEdge ():cyclemark ( 0 ), close ( false ) { }
      GSEdge ( int tonode, int postonode, EdgeLabel edgelabel, bool close = false ): tonode ( tonode ), postonode ( postonode ), edgelabel ( edgelabel ), cyclemark ( 0 ), close ( close ) { }
    };
    struct GSNode {
      NodeLabel label;
      short unsigned int maxdegree;
      vector<GSEdge> edges;
    };
    //keep for debugging purposes
    void makeState ( DatabaseTree *databasetree );
    void undoState ();
    void insertNode ( NodeLabel nodelabel, short unsigned int maxdegree );
    void deleteNode2 ();
    vector<GSNode> nodes;
    int edgessize;
    short unsigned int getNodeDegree ( int i ) const { return nodes[i].edges.size (); }
    short unsigned int getNodeMaxDegree ( int i ) const { return nodes[i].maxdegree; }
    GraphState ();
    void determineCycles ( unsigned int usedbit );
    int enumerateSpanning ();
    int isnormal ();
    int normalizetree ();
    int normalizeSelf ();
    void init ();
    void insertStartNode ( NodeLabel nodelabel );
    void deleteStartNode ();
    void insertNode ( int from, EdgeLabel edgelabel, short unsigned int maxdegree );
    void deleteNode ();
    void insertEdge ( int from, int to, EdgeLabel edgelabel );
    void deleteEdge ( int from, int to );
    void deleteEdge ( GSEdge &edge ); // pushes deleted edge on stack
    void reinsertEdge (); // reinserts last edge on the stack
    NodeId lastNode () const { return nodes.size () - 1; }
    void print ();
	void clear ();
};

#endif
