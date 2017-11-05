/**
 * 	Arhitectura sistemelor de calcul
 *	Tema 2 - Inmultirea eficienta a matricelor in C/C++
 *
 *	Badila Gabriel-Alin, 333CA
 */


#include "utils.h"

/**
 * Functia de citire a unei matrici dintr-un fisier.
 */
void readMatrix(char *fileName, struct test *info, char type) {
	FILE *f;
	int i, j, rows = 0, collumns = 0, ret;

	if ((f = fopen(fileName, "r")) == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	/* alegerea dimensiunilor matricei */
	if (type == 'A') {
		rows = info->M;
		collumns = info->K;
	}
	else if (type == 'B') {
		rows = info->K;
		collumns = info->N;
	}
	else if (type == 'C') {
		rows = info->M;
		collumns = info->N;
	}

	/* alocarea liniarizata a matricei */
	if (type == 'A')
		info->A = (double*) calloc(rows * collumns, sizeof(double));
	else if (type == 'B')
		info->B = (double*) calloc(rows * collumns, sizeof(double));
	else if (type == 'C')
		info->C = (double*) calloc(rows * collumns, sizeof(double));

	/* citirea matricei din fisier */
	for (i = 0; i < rows; i++) {
		for (j = 0; j < collumns; j++) {
			if (type == 'A')
				ret = fscanf(f, "%lf", &info->A[i * collumns + j]);
			else if (type == 'B')
				ret = fscanf(f, "%lf", &info->B[i * collumns + j]);
			else if (type == 'C')
				ret = fscanf(f, "%lf", &info->C[i * collumns + j]);

			if (ret != 1)
				exit(1);
		}
	}

	fclose(f);
}


/**
 * Functia de calculare a minimului dintre 2 valori.
 */
int minV(int a, int b)
{
	if (a < b)
		return a;
	return b;
}

/**
 * Functia de baza care realizeaza inmultirea efectiva a matricelor.
 */
int main(int argc, char **argv)
{
    struct test **tests;
    tests = (struct test **)malloc(sizeof(struct test*));
    (*tests) = (struct test *)calloc(MAXTESTS, sizeof(struct test));

    if (argc != 2)
        error("Usage: tema2 <cfg_file>");

    parse_config(argv[1], tests);

    FILE *f;
    int i, j, k, kk, i2, j2, kk2;
    int size = 50;

    for (k = 0; k < 8; k++) {

    	char fileName[size], output[size], matrixA[size], matrixB[size], matrixC[size];
    	double *D;
    	int M, N, K;

    	/* setarea denumirilor fisierelor */
    	memset(fileName, 0, size);
        strcpy(fileName, "../input/");
        strcat(fileName, tests[0][k].name);

        memset(matrixA, 0, size);
        strcpy(matrixA, fileName);
        strcat(matrixA, "_A.in");

        memset(matrixB, 0, size);
        strcpy(matrixB, fileName);
        strcat(matrixB, "_B.in");

        memset(matrixC, 0, size);
        strcpy(matrixC, fileName);
        strcat(matrixC, "_C.in");

        memset(output, 0, size);
        strcpy(output, tests[0][k].name);
        strcat(output, ".out");

        /* citirea matricelor din fisiere */
        struct test *info = &tests[0][k];
        readMatrix(matrixA, info, 'A');
        readMatrix(matrixB, info, 'B');
        readMatrix(matrixC, info, 'C');

        D = (double*) calloc(info->M * info->N, sizeof(double));

		if ((f = fopen(output, "w")) == NULL) {
			printf("Error opening file2!\n");
			return -1;
		}

		M = info->M;
		N = info->N;
		K = info->K;

		int b = 8; // setare dimensiune block

		/* inmultirea matricelor folosind BMM */
		/* cazul: alpha = 0 */
		if (info->alpha == 0 && info->beta != 0) {
			for(i = 0; i < M; i++){
				for(j = 0; j < N; j++){
					D[i * N + j] += info->beta * info->C[i * N + j];
				}
			}
		}
		/* cazul: beta = 0 cu cele patru subcazuri*/
		else if (info->alpha != 0 && info->beta == 0) {
			if (info->transa == 'N' && info->transb == 'N') {
				for (i2 = 0; i2 < M; i2 = i2 + b) {
					for (kk2 = 0; kk2 < K; kk2 = kk2 + b) {
						for (j2 = 0; j2 < N; j2 = j2 + b) {
							for (i = i2; i < minV(i2 + b, M); i++) {
								for (kk = kk2; kk < minV(kk2 + b, K); kk++) {
									for (j = j2; j < minV(j2 + b, N); j++) {
										D[i * N + j] += info->alpha * info->A[i * K + kk] * info->B[kk * N + j];
									}
								}
							}
						}
					}
				}
			}
			else if (info->transa == 'N' && info->transb == 'T') {
				for (i2 = 0; i2 < M; i2 = i2 + b) {
					for (j2 = 0; j2 < N; j2 = j2 + b) {
						for (kk2 = 0; kk2 < K; kk2 = kk2 + b) {
							for (kk = kk2; kk < minV(kk2 + b, K); kk++) {
								for (j = j2; j < minV(j2 + b, N); j++) {
									for (i = i2; i < minV(i2 + b, M); i++) {
										D[i * N + j] += info->alpha * info->A[i * K + kk] * info->B[j * K + kk];
									}
								}
							}
						}
					}
				}
			}
			else if (info->transa == 'T' && info->transb == 'N') {
				for (kk2 = 0; kk2 < K; kk2 = kk2 + b) {
					for (j2 = 0; j2 < N; j2 = j2 + b) {
						for (i2 = 0; i2 < M; i2 = i2 + b) {
							for (kk = kk2; kk < minV(kk2 + b, K); kk++) {
								for (j = j2; j < minV(j2 + b, N); j++) {
									for (i = i2; i < minV(i2 + b, M); i++) {
										D[i * N + j] += info->alpha * info->A[kk * M + i] * info->B[kk * N + j];
									}
								}
							}
						}
					}
				}
			}
			else if (info->transa == 'T' && info->transb == 'T') {
				for (kk2 = 0; kk2 < K; kk2 = kk2 + b) {
					for (j2 = 0; j2 < N; j2 = j2 + b) {
						for (i2 = 0; i2 < M; i2 = i2 + b) {
							for (kk = kk2; kk < minV(kk2 + b, K); kk++) {
								for (j = j2; j < minV(j2 + b, N); j++) {
									for (i = i2; i < minV(i2 + b, M); i++) {
										D[i * N + j] += info->alpha * info->A[kk * M + i] * info->B[j * K + kk];
									}
								}
							}
						}
					}
				}
			}
		}
		/* cazul normal, cu ambele constante diferite de 0 ce contine
		   patru subcazuri: NN, NT, TN si TT */
		else if (info->alpha != 0 && info->beta != 0) {
			if (info->transa == 'N' && info->transb == 'N') {
				for (i2 = 0; i2 < M; i2 = i2 + b) {
					for (kk2 = 0; kk2 < K; kk2 = kk2 + b) {
						for (j2 = 0; j2 < N; j2 = j2 + b) {
							for (i = i2; i < minV(i2 + b, M); i++) {
								for (kk = kk2; kk < minV(kk2 + b, K); kk++) {
									for (j = j2; j < minV(j2 + b, N); j++) {
										D[i * N + j] += info->alpha * info->A[i * K + kk] * info->B[kk * N + j];
									}
								}
							}
						}
					}
				}
			}
			else if (info->transa == 'N' && info->transb == 'T') {
				for (i2 = 0; i2 < M; i2 = i2 + b) {
					for (j2 = 0; j2 < N; j2 = j2 + b) {
						for (kk2 = 0; kk2 < K; kk2 = kk2 + b) {
							for (kk = kk2; kk < minV(kk2 + b, K); kk++) {
								for (j = j2; j < minV(j2 + b, N); j++) {
									for (i = i2; i < minV(i2 + b, M); i++) {
										D[i * N + j] += info->alpha * info->A[i * K + kk] * info->B[j * K + kk];
									}
								}
							}
						}
					}
				}
			}
			else if (info->transa == 'T' && info->transb == 'N') {
				for (kk2 = 0; kk2 < K; kk2 = kk2 + b) {
					for (j2 = 0; j2 < N; j2 = j2 + b) {
						for (i2 = 0; i2 < M; i2 = i2 + b) {
							for (kk = kk2; kk < minV(kk2 + b, K); kk++) {
								for (j = j2; j < minV(j2 + b, N); j++) {
									for (i = i2; i < minV(i2 + b, M); i++) {
										D[i * N + j] += info->alpha * info->A[kk * M + i] * info->B[kk * N + j];
									}
								}
							}
						}
					}
				}
			}
			else if (info->transa == 'T' && info->transb == 'T') {
				for (kk2 = 0; kk2 < K; kk2 = kk2 + b) {
					for (j2 = 0; j2 < N; j2 = j2 + b) {
						for (i2 = 0; i2 < M; i2 = i2 + b) {
							for (kk = kk2; kk < minV(kk2 + b, K); kk++) {
								for (j = j2; j < minV(j2 + b, N); j++) {
									for (i = i2; i < minV(i2 + b, M); i++) {
										D[i * N + j] += info->alpha * info->A[kk * M + i] * info->B[j * K + kk];
									}
								}
							}
						}
					}
				}
			}

			for(i = 0; i < M; i++){
				for(j = 0; j < N; j++){
					D[i * N + j] += info->beta * info->C[i * N + j];
				}
			}
		}

		/* scrierea in fisierul de output */
    	for (i = 0; i < M; i++) {
			for (j = 0; j < N; j++) {
				fprintf(f, "%.3lf ", D[i * N + j]);
			}
			fprintf(f, "\n");
		}



		fclose(f);

		/* eliberare resurse */
		free(info->A);
		free(info->B);
		free(info->C);
		free(D);
    }

    return 0;
}
