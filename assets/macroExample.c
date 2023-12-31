#define MACRO_DEF_TEST_1
static const int globalVar1;
#define MACRO_DEF_TEST_2 unsigned long int
const int globalVar2 = 5;
#define MACRO_DEF_TEST_3(X, Y) max(X, Y)
#include "includeExample.h"
void func8();

int line1 = __LINE__;
int line2 = __LINE__;
int line3 = __LINE__;


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

#define MACRO_DEF_SIZE 256
#define MACRO_MAX(X, Y) ((X) > (Y) ? (X) : (Y))

static int MacroDefSize = MACRO_DEF_SIZE;
static int MacroDefSize2 = MACRO_DEF_SIZE * 2;
int MacroDefSizeMax = MACRO_MAX(MACRO_DEF_SIZE + (20 * 30), MACRO_DEF_SIZE);

char *thing = "Hello world";
char *filename = __FILE__;

char *date = __DATE__;
char *time = __TIME__;
