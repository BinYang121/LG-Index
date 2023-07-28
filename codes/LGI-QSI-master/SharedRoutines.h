#ifndef _SHARED_ROUTINES_H_
#define _SHARED_ROUTINES_H_

extern unsigned int g_random_seed;

enum TreeNodeTpye { enRootNode=0, enLinkNode=1, enLeafNode=2 };

enum RF_TREE_RANDOMNESS 
{
	TREE_RANDOMNESS_WEAK = 1,
	TREE_RANDOMNESS_MODERATE = 2,
	TREE_RANDOMNESS_STRONG = 3
};


float timeIt(int reset);

void GenerateSeedFromSysTime();

#define RAND_MAX_RF 0x7ffffffd
int rand_freebsd(void);
void srand_freebsd(unsigned seed);

void permute(const int n, int* order_permuted);


#ifdef _DEBUG
#define KCF_ASSERT(condition) if(!(condition)) \
{			\
	printf("[TRACK ASSERT] ERROR %s in FILE %s, LINE %u\n", #condition, __FILE__, __LINE__); \
	exit(-1);	\
	}
#else
#define KCF_ASSERT(condition)
#endif


typedef struct tagKCFMem
{
	void* mem;
	unsigned int tot_size;
	unsigned int used;
}KCFMemPool;

void* kcfAlloc(KCFMemPool* pool, unsigned int size);

void kcfFree(KCFMemPool* pool, void** ptr, unsigned int size);

#endif