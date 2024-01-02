#include <stdio.h>
#include <string.h>

#include "testArgs.h"

TestArgs gTestArgs;

const char *TEST_ARGS_HELP_MSG =
"usage: cmd [ARGS] file...\n"
"\n"
"ARGS:\n"
"\t-v: verbose\n"
"\t--help|-h: Shows this message.\n";

/**
 * @brief Parses an argument from the provided args
 * @returns The number of parsed args or 0 on failure
 */
static int _parseArg(int argc, char **argv, TestArgs *args) {
	if (argc < 1) return 0;

	if (strcmp(argv[0], "--help") == 0 || strcmp(argv[0], "-h") == 0) {
		args->help = 1;
		return 1;
	} else if (strcmp(argv[0], "-v") == 0) {
		args->verbose = 1;
		return 1;
	} else {
		fprintf(stderr, "Invalid option %s", argv[0]);
		return 0;
	}
}

void initTestArgs(TestArgs *args) {
	args->help = 0;
	args->verbose = 0;
}

void freeTestArgs(TestArgs *args) {
	return;
}

int parseTestArgs(int argc, char **argv, TestArgs *args) {
	int progress;
	int parseRes;;

	initTestArgs(args);

	//skip the first item
	argv++;
	progress = 1;

	while (progress < argc) {
		parseRes = _parseArg(argc - progress, argv, args);
		if (parseRes == 0) {
			return 0;
		}
		progress += parseRes;
		argv += parseRes;
	}
	return 1;
}

int testArgsPrint(const TestArgs *args) {
	int n = 0;

	n += printf("{");
	n += printf("\"help\": %d", args->help);
	n += printf("\"verbose\": %d", args->verbose);
	n += printf("}");

	return n;
}
