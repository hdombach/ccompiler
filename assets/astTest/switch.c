void switchMain() {
	int value;

	switch (value + 4 % 10) {
		case 0:
			value = 1;
			break;
		case 2:
			value = 3;
			break;
		case 5 * 2 + 2:
			value = 10;
			break;
		default:
			value = 0;
			break;
	}
}
