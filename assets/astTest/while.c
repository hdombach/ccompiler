void mainWhile() {
	int value = 0;
	while (value < 0) {
		value += 2;
	}

	while (value) value--;

	do {
		value++;
		value = value * 1 + 2;
	} while (value < 10);
}
