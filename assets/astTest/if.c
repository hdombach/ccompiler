int printInt(int value);

void ifMain() {
	int value;
	if (value == 0) {
		printInt(500);
	} else if (value == 1) {
		printInt(600);
	} else if (value == 2) {
		printInt(200);
		printInt(1);
	} else if (value == 3) {
		printInt(1000);
	}

	if (value % 10 == 6) {
		printInt(6);
	}

	if (value % 20) printInt(20);
	else printInt(21);
}
