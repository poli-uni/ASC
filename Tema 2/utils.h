#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXTESTS    10
#define MAXLEN      128
#define NAME        "name"
#define TRANSA      "TRANSA"
#define TRANSB      "TRANSB"
#define C_M         "M"
#define C_N         "N"
#define C_K         "K"
#define LDA         "LDA"
#define LDB         "LDB"
#define LDC         "LDC"
#define ALPHA       "ALPHA"
#define BETA        "BETA"
#define NUM_RANGE   "num_range"
#define POINTS      "points"
#define ACTIVE      "active"

struct test {
    char name[32];
    char test_name[32];
    char transa, transb;
    double alpha, beta;
    int M, N, K;
    int lda, ldb, ldc;
    double *A, *B, *C;
    double num_range; /* used only by input generator */
    int points; /* used only by tester */
    int active; /* check if test */
};

void parse_config(char *, struct test **);
void print_tests(struct test *);
void error(char *);
#endif
