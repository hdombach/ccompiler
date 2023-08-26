#define MACRO_DEF_TEST_1
static const int globalVar1;
#define MACRO_DEF_TEST_2 unsigned long int
const int globalVar2 = 5;
#define MACRO_DEF_TEST_3(X, Y) max(X, Y)
#include <hello/world.h>
#include "helloWorld.h"
