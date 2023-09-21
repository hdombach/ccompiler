struct TestStruct {
	int value1;
	int value2;
	int value3;
} inlineTestStruct1, inlineTestStruct2 = {5, 2, 100};

struct TestStruct testStruct1, *testStruct2;
struct TestStruct testStructInit = {2, 10, 34};

typedef struct StructTypedef {
	int value1, value2, *value3;
} StructTypedef, StructTypedef1, StructTypedef2;

StructTypedef2 stypedef1, *stypedef2;
StructTypedef stypedef1, *stypedef2;
struct StructTypedef stypedef1, *stypedef2;

struct TestBitField {
	int value : 5;
	/* this makes me very unhappy */
	int : 0, :3;
	unsigned value2 : 2;
} testBitField1;

union TestUnion {
	int option1;
	char option2;
	struct {
		int id;
		int index;
	} content;
} testUnion1;

typedef struct {
	enum ComboType {
		TYPE_1,
		TYPE_2
	} type;
	union {
		int value;
		struct TestStruct value2;
	} c;
} ComboStruct;

struct IncompleteStruct *incompleteValue;
