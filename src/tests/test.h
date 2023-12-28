#ifndef TEST_H
#define TEST_H

extern int testsTotal;
extern int testsPassed;

void tInit();

#define T_ASSERT(name, exp) \
	tAssert(__FILE__, __LINE__, name, exp, #exp)
void tAssert(
		const char *file,
		int line,
		char *name,
		int expression,
		const char *expressionStr);
void tPrintResult();
void tStartSection(char *name);

#endif
