int testFunc2(int, float) { }

struct TestStruct2 {
	struct {
		int internalValue;
	} structValue;
};

void operationMain() {
	int value;
	int *ptr;
	struct TestStruct2 *structValue;

	4;
	5, 6;
	5, 6, 7;

	value = 4;
	value = 10, 11;

	value += 1, value -= 2;
	value *= 100, value /= 101, value %= 102;
	value <<= 5, value >>= 6;
	value &= 10, value ^= 9, value |= 8;

	5 ? 1 : 2;
	0 || 1;
	0 && 1;
	1 && 0 || 1;
	1 || 0 && 1;
	5 | 9;
	5 ^ 9;
	5 & 9;
	value = 5 ^ 2 & 10 | 3;

	1 == 0;
	1 != 0;

	2 < 3;
	2 <= 3;
	2 > 3;
	2 >= 3;

	5 >> 2;
	5 << 2;

	value = 3 - 2 + 5 - 2;
	value = 5 / 2 * 4 % 10;

	++value;
	--value;
	+value;
	-value;
	(float) value;
	!value;
	~value;
	*ptr;
	&value;
	sizeof(int);
	sizeof value;

	value++;
	value--;
	testFunc2(3 + 2 * 2 - 3, value++ + 3);
	ptr[5 * 2] = value;
	
	structValue->structValue.internalValue = value;

	value = ++(*ptr++) + 3 * (2 + 6);
}
