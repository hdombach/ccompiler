extern const long unsigned int arithTest;

unsigned char multTest1 = 2, multTest2, multTest3 = 23;

typedef char char_t;

char_t typedefTest;
char_t typedefTest2;

typedef short short_t;
typedef signed short int short_t;

typedef struct TestStruct {
	int value;
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
