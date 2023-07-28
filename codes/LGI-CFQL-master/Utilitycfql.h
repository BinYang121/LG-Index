#ifndef FSC_UTILITYCFQL_H
#define FSC_UTILITYCFQL_H

#include "Graphcfql.h"
#include <string.h>
#include <sys/time.h>

class Utilitycfql {
private:
    Utilitycfql() {}

public:
    static void GetKCore(const Graphcfql* graph, int* core_table);
    static timeval GetTime();
    static unsigned long TimeDiffInMicroseconds(const timeval start, const timeval end);
    static double TimeDiffInSeconds(const timeval start, const timeval end);
    static unsigned long UpperPowerOfTwo(unsigned long v);
    static void old_cheap(int* col_ptrs, int* col_ids, int* match, int* row_match, int n, int m);
    static void match_bfs(int* col_ptrs, int* col_ids, int* match, int* row_match, int* visited,
                          int* queue, int* previous, int n, int m);

};

#endif //FSM_UTILITY_H
