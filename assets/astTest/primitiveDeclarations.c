typedef char char_t, char2_t;
typedef int int_t;

char_t testChar, testChar2;
char testChar;
extern const long unsigned intCompl;

int_t testInt1 = 4, testInt2, testInt3 = 500;

typedef int *intPtr_t, intBuf_t[5];

intPtr_t intPtr = 0, intPtr2;

int_t *(*pointerTest), ***pointerTest2;

int const * const intConstTest;

int arrayTest1[10], arrayTest2[20][10];

int_t *intMix1[1], (*intMix2)[2], *(intMix3[3]);

unsigned long int (((*intMix4[4])[5][10]));

float floatTest;
long double chunkyFloatTest;
