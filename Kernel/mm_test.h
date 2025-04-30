#ifndef MM_TEST
#define MM_TEST

#include "CuTest.h"

typedef void (*Test)(CuTest *const cuTest);

CuSuite *getMemoryManagerTestSuite(void);

#endif