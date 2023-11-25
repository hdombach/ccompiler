struct FuncStruct;

int funcDef(int param1, unsigned int param2, ...);
int funcDef2(struct FuncStruct *combo);
int (*funcPtr)(void);
int funcDefWithoutName(int, char const *, char []);
int *test;

void testFunc() {
	int test123 = 2;
}
