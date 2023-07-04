#include "ppArgParser.h"

int main(int argc, char **argv) {
	PPArgs args;

	args = initPPArgs();

	if (!ppParseArgs(argc, argv, &args)) {
		printf("%s\n", ppArgParserErr());
		return 1;
	}

	if (args.help) {
		printf("%s\n", PP_HELP_MSG);
		return 0;
	}

	ppArgsPrint(&args);
	printf("\n");

}
