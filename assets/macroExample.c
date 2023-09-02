#define MACRO_DEF_TEST_1
static const int globalVar1;
#define MACRO_DEF_TEST_2 unsigned long int
const int globalVar2 = 5;
#define MACRO_DEF_TEST_3(X, Y) max(X, Y)
#include "includeExample.h"
void func8();

#ifndef MACRO_DEF_TEST_1
#ifdef MACRO_DEF_TEST_2
void func11();
#else
void func12();
#endif
#endif

#undef MACRO_DEF_TEST_1

#ifndef MACRO_DEF_TEST_1
void func9();
#else
void func13();
#endif
