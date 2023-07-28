#ifndef QUICKSI
#define QUICKSI

#include "GraphDS.h"
#include "IsoSolver.h"
#include "IsoSolver.cpp"
#include "Graph.h"
#include "Graph.cpp"
#include <vector>

std::random_device dev;
std::mt19937 rng;

std::unordered_map<short, int> label_weights;
std::unordered_map<std::pair<short, short>, int> edge_weights;

void initialize(int data_num, vector<TGraph::Graph*> gs) {
	rng = std::mt19937(dev());

	label_weights.clear();
	edge_weights.clear();
	for (int i = 0; i < data_num; i++) {
		for (int v = 1; v <= gs[i]->n; v++) {
			short lv = gs[i]->label[v];
			label_weights[lv]++;
			for (int e = gs[i]->h[v]; e; e = gs[i]->e[e].ne) {
				int u = gs[i]->e[e].v;
				short lu = gs[i]->label[u];
				if (lv < lu) edge_weights[std::make_pair(lv, lu)]++;
			}
		}
	}
}

QuickSI::Graph convert(TGraph::Graph &G) {
	QuickSI::Graph Q;
	for (int v = 1; v <= G.n; v++) {
		short lv = G.label[v];
		Q.AddNode(lv, label_weights[lv]);
		for (int e = G.h[v]; e; e = G.e[e].ne) {
			int u = G.e[e].v;
			short lu = G.label[u];
			if (u < v) Q.AddEdge(u - 1, v - 1, edge_weights[std::make_pair(lv, lu)]);
		}
	}
	return Q;
}

bool phi[__V][__V];

bool solve( TGraph::Graph &P, TGraph::Graph &G ) {
	QuickSI::Graph Q = convert(P);
	Q.GetQISeq();
	QuickSI::Graph G_ = convert(G);
	IsoSolver solver(Q, G_);
	return solver.QuickSI(0);
}

#endif
