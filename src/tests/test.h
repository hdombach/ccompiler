#ifndef TEST_H
#define TEST_H

extern int testsTotal;
extern int testsPassed;

void tInit();
void tAssert(char *name, int expression);
void tPrintResult();
void tStartSection(char *name);

#endif
