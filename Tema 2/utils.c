#include "utils.h"
static void remove_some_chars(char *word) {

    int i, n, count = 0;
    n = strlen(word);
    if (word == NULL)
        return;

    char *new_word = (char *)malloc(n + 1);

    for(i = 0; i < n; i++) {
        char c = word[i];
        if (c == ',') {
            new_word[count] = '.';
            ++count;
        } else if (c != '\n'
                   && c != '\t'
                   && c != ' ') {
            new_word[count] = c;
            ++count;
        }
    }
    new_word[count] = '\0';
    strcpy(word, new_word);
    free(new_word);
}

static void fill_value(struct test *my_test, char *key, char *value) {
    //printf("Trying to write [%s] = [%s] on test [%s]\n", key, value, my_test->name);
    /* let the fun begin. Finest switch... ever!*/
    if (!strcmp(key, NAME)) {
        strcpy(my_test->test_name, value);
    } else if (!strcmp(key, TRANSA)) {
        my_test->transa = value[0];
    } else if (!strcmp(key, TRANSB)) {
        my_test->transb = value[0];
    } else if (!strcmp(key, C_M)) {
        my_test->M = atoi(value);
    } else if (!strcmp(key, C_N)) {
        my_test->N = atoi(value);
    } else if (!strcmp(key, C_K)) {
        my_test->K = atoi(value);
    } else if (!strcmp(key, LDA)) {
        my_test->lda = atoi(value);
    } else if (!strcmp(key, LDB)) {
        my_test->ldb = atoi(value);
    } else if (!strcmp(key, LDC)) {
        my_test->ldc = atoi(value);
    } else if (!strcmp(key, ALPHA)) {
        my_test->alpha = atof(value);
    } else if (!strcmp(key, BETA)) {
        my_test->beta = atof(value);
    } else if (!strcmp(key, NUM_RANGE)) {
        my_test->num_range = atof(value);
    } else if (!strcmp(key, POINTS)) {
        my_test->points = atoi(value);
    } else if (!strcmp(key, ACTIVE)) {
        if (!strcmp(value, "True"))
            my_test->active = 1;
        else
            my_test->active = 0;
    }
}
static void parse_line(char *line,
                        struct test **tests)
{
    if (strlen(line) == 0)
        return;
    if (line[0] == '#'
        || line[0] == ' '
        || line[0] == '\t'
        || line[0] == '\n')
        return;

    char *token, *state, *state1;
    char *value, *key, *test_name;
    int i;
    token = strtok_r(line, "=", &state);

    /* get test name and remove spaces, tabs*/
    test_name = strtok_r(token, ".", &state1);
    remove_some_chars(test_name);

    /* get key for test_name and remove spaces, tabs */
    key = strtok_r(NULL, ".", &state1);
    remove_some_chars(key);

    /* get value for key and remove spaces, tabs, \n
       Extra: replace , with . for floats */
    value = strtok_r(NULL, "=", &state);
    remove_some_chars(value);

    //printf("[%s].[%s]=[%s]\n", test_name, key, value);

    for (i = 0; i < MAXTESTS; i++) {
        struct test *my_test = &((*tests)[i]);
        if (my_test->name[0] == '\0' || strcmp(test_name, my_test->name) == 0) {
            strcpy(my_test->name, test_name);
            fill_value(my_test, key, value);
            break;
        }

    }

}

void error(char *message)
{
    fprintf(stderr,"%s", message);
    exit(EXIT_FAILURE);
}

void parse_config(char *fname,
                  struct test **tests)
{
    FILE *f;
    char *line = (char *)malloc(MAXLEN);

#ifdef VERBOSE    
    printf("Parsing config file %s\n", fname);
#endif

    /* tests must not be NULL */
    if (tests == NULL) {
        error("test struct is NULL");    
    }
    /* open config file */
    f = fopen(fname, "r");
    if (f == NULL) {
        error("f is NULL");
    }
    
    while (fgets(line, MAXLEN, f) != NULL) {
        parse_line(line, tests);
    }
    fclose(f);
    if (line)
       free(line);
}

void print_tests(struct test *tests)
{
    printf("Tests that are available:\n");
    int i;
    for (i = 0; i < MAXTESTS; i++) {
        if (tests[i].name[0] == '\0')
            break;
        struct test *my_test = &tests[i];
        printf("%i. Test[%s] \'known\' as %s:\n", i, my_test->name, my_test->test_name);
        printf("\tTransA = %c\n", my_test->transa);
        printf("\tTransB = %c\n", my_test->transb);
        printf("\tM = %d\n", my_test->M);
        printf("\tN = %d\n", my_test->N);
        printf("\tK = %d\n", my_test->K);
        printf("\tLDA = %d\n", my_test->lda);
        printf("\tLDB = %d\n", my_test->ldb);
        printf("\tLDC = %d\n", my_test->ldc);
        printf("\tALPHA = %.3f\n", my_test->alpha);
        printf("\tBETA = %.3f\n", my_test->beta);
        printf("\tNUM_RANGE = %.3f\n", my_test->num_range);
        printf("\tPoints = %d\n", my_test->points);
        printf("\tActive = %d\n", my_test->active);
    }

}
