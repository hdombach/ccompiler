extern const long unsigned int arithTest;

unsigned char multTest1 = 2, multTest2, multTest3 = 23;

typedef char char_t;

char_t typedefTest;
char_t typedefTest2;

typedef short short_t;
typedef signed short int short_t;

float floatTest;

typedef struct TestStruct {
	int value;
	int value2;
	int value3;
} TestStruct;

struct TestBitField {
	int value : 5;
	/* this makes me very unhappy */
	int : 0, :3;
	unsigned value2 : 2;
};

union TestUnion {
	int option1;
	char_t option2;
	struct {
		int id;
		int index;
	};
};

enum TestEnum {
	TEST_ENUM_1,
	TEST_ENUM_2 = 100,
	TEST_ENUM_3,
} testEnum1;

typedef enum {
	TEST_ENUM2_1,
} TestEnum2;

typedef struct {
	enum ComboType {
		TYPE_1,
		TYPE_2
	} type;
	union {
		int value;
		TestStruct value2;
	} c;
} ComboStruct;

struct IncompleteStruct *incompleteValue;

int (nestedParan), ((nestParan2));
char_t *pointerTest, **pointerTest2;
int arrayTest[5], arrayTest2[6][10];

int *mix1[1], (*mix2)[2], *(mix3[3]);

int funcDef(int param1, unsigned int param2, ...);
int funcDef2(ComboStruct *combo);
int (*funcPtr)(void);
int funcDefWithoutName(int, char const *, char []);

TestStruct testStruct1 = {1, 2, 3};

int const * const constTest;

void testFunc() {
	int test123 = 2;
}
