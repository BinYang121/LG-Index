#ifndef SRC_GRAPHREADER_H
#define SRC_GRAPHREADER_H

#include "Graphcfql.h"

class GraphReader {
public:
    static vector<Graphcfql*>* LoadGraphFormatA(const string& file, unordered_map<string, int>*& label_map,
                                            unordered_map<string, int>*& graph_map, unordered_map<int, int>*& label_histogram);

    static vector<Graphcfql*>* LoadGraphFormatB(const string& file, unordered_map<string, int>*& label_map,
                                            unordered_map<string, int>*& graph_map, unordered_map<int, int>*& label_histogram);

    static vector<Graphcfql*>* LoadGraphFormatC(const string& file, unordered_map<string, int>*& label_map,
                                            unordered_map<string, int>*& graph_map, unordered_map<int, int>*& label_histogram);
};


#endif //SRC_GRAPHREADER_H
