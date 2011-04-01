/* NSQ Random Number Generator
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include "libnsqrand.h"

static const char *program_name;
static int error = 0;

static void print_usage(void);

int main(int argc, char *argv[])
{
    int i;
    int optc;
    int low = 0, high = INT_MAX;        /* Range for integers */
    int seed = 0;                       /* Initial seed */
    int odds = 0;                       /* Odds for success in the chance game */
    unsigned int seqlen = 0;            /* Length of the sequence to generate */
    char type = 'i';                    /* Type of number to generate */

    program_name = argv[0];

    /* Initially seed with current time, in case no seed is specified */
    nsqseed(time(NULL));

    while ((optc = getopt(argc, argv, "fhc:n:r:s:")) != -1) {
        switch (optc) {
            case 'h':
                print_usage();
                exit(EXIT_SUCCESS);
            case 'c':
                if (optarg == NULL || !sscanf(optarg, "%d", &odds)) {
                    error = 1;
                    break;
                }
                printf("You take a chance... ");
                if (odds != 0) {
                    if (nsqluck(odds))
                        printf("and win the lottery (okay, it isn't that big a deal)!\n");
                    else
                        printf("and oh dear, that didn't work out.  Ho hum.\n");
                }
                else
                    printf("but it wasn't really fair anyway.\n");
                exit(EXIT_SUCCESS);
            case 'f':
                type = 'f';
                break;
            case 'r':
                if (optarg == NULL || sscanf(optarg, "%d-%d", &low, &high) != 2)
                    error = 1;
                break;
            case 'n':
                if (optarg == NULL || sscanf(optarg, "%u", &seqlen) != 1)
                    error = 1;
                break;
            case 's':
                if (optarg == NULL || sscanf(optarg, "%d", &seed) != 1)
                    error = 1;
                break;
        }
    }

    /* Exit if an error occurred */
    if (error) {
        print_usage();
        exit(EXIT_FAILURE);
    }

    /* Reseed if seed was given on the command line */
    if (seed)
        nsqseed(seed);

    switch (type) {
        case 'i':
            if (seqlen != 0)
                for (i = 0; i < seqlen; i++)
                    printf("%d\n", nsqrandr(low, high));
            else
                while (1)
                    printf("%d\n", nsqrandr(low, high));
            break;
        case 'f':
            if (seqlen != 0)
                for (i = 0; i < seqlen; i++)
                    printf("%f\n", nsqunif());
            else
                while(1)
                    printf("%f\n", nsqunif());
            break;
    }

    exit(EXIT_SUCCESS);
}

static void print_usage(void)
{
    printf("Usage: %s [options]\n", program_name);
    fputs("Outputs a sequence of uniformly distributed random numbers\n", stdout);
    fputs("-h               prints this message\n", stdout);
    fputs("-c odds          plays a game where you select your odds of winning\n", stdout);
    fputs("-f               generate uniform floats over [0, 1]\n", stdout);
    fputs("-r a-b           specify a range for generated numbers (integer type only)\n", stdout);
    fputs("-n number        length of sequence to generate (continuous if not specified)\n", stdout);
    fputs("-s seed          initial seed number (default: current time)\n", stdout);
}
