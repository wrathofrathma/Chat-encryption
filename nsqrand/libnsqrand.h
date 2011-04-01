/*
 * Non Seqiter header file
 */

void nsqseed(int seed);

/* Produces uniformly distributed random integers */
int nsqrand(void);

/* Produces uniformly distributed random integers over the range [min, max) */
int nsqrandr(int min, int max);

/* Produces uniformly distributed random floats over the interval [0, 1] */
double nsqunif(void);

/* Returns a 1 chance% of the time, and a 0 otherwise */
int nsqchance(int chance);

/* Returns a 1 with a probably of one in the specified odds, 0 otherwise */
int nsqluck(int odds);
