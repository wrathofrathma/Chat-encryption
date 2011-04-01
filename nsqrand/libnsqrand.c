#include <limits.h>
#include "libnsqrand.h"

#define SHIFT 63 /* Shift amount for sequence computation */
#define OBITS 31 /* Output bits */

/* Compute next sequence iteration */
#define seqiter(seq, shl, shr) \
{ \
    shl = (seq << 1) | (seq & (1ULL << SHIFT)) >> SHIFT; \
    shr = (seq >> 1) | ((seq & 1) << SHIFT); \
    seq = ((shr ^ shl) | seq) ^ (seq & shr); \
}

/* 64-bit LCG seed generator */
#define SM 6364136223846793005ULL
#define SAC 1442695040888963407ULL
#define SMOD (1ULL << SHIFT)
#define lcg64(s) ((SM * (s) + SAC) % SMOD)

typedef unsigned long long uint64;

static uint64 seqset[OBITS];

int nsqrand(void)
{
    register int i, r = 0;
    register uint64 shl, shr;

    for (i = 0; i < OBITS; i++) {
        r |= (seqset[i] & (1 << i));
        seqiter(seqset[i], shl, shr);
    }

    return r;
}

int nsqrandr(int min, int max)
{
    return nsqrand() % (max - min) + min;
}

double nsqunif(void)
{
    return (double)nsqrand() / (double)INT_MAX;
}

int nsqchance(int chance)
{
    return nsqrand() % 100 < chance;
}

int nsqluck(int odds)
{
    return nsqrand() % odds == 0;
}

void nsqseed(int seed)
{
    register int i;
    register uint64 seqseed = (uint64)seed;

    for (i = 0; i < OBITS; i++) {
        seqseed = lcg64(seqseed);
        seqset[i] = seqseed;
    }
}
