#include <stdio.h>
#include <string.h>

#include "argParser.h"
#include "util/dlist.h"
#include "util/util.h"

const char *ARGS_HELP_MSG =
"usage: cmd [-h | --help] file...\n"
"\n"
"ARGS:\n"
"\t--help: Shows this message.\n";

/*
 * Parses on argument from the provided args
 * REturns the number of parsed words or 0 on failure
 */
int _parseArg(int argc, char **argv, Args *args) {
	char *tempStr;

	if (argc < 1) {
		return 0;
	}

	if (strcmp(argv[0], "--help") == 0 || strcmp(argv[0], "-h") == 0) {
		args->help = 1;
		return 1;
	} else if (argv[0][0] == '-') {
		fprintf(stderr, "Invalid option %s", argv[0]);
		return 0;
	} else {
		tempStr = strdup(argv[0]);
		dlistApp(&args->files, &tempStr);
		return 1;
	}
}

int _printStr(char **str) {
	return printf("%s", *str);
}

void initArgs(Args *args) {
	args->help = 0;
	initDList(&args->files, sizeof(char*));
}

void freeArgs(Args *args) {
	freeDList(&args->files, (DListFreeFunc) freeStr);
}

int parseArgs(int argc, char **argv, Args *args) {
	int progress;
	int parseRes;;

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

int argsPrint(const Args *args) {
	int n = 0;
	n += printf("{");
	n += printf("\"help\": %d", args->help);
	n += printf(", \"files\": ");
	n += printDList(&args->files, (DListPrintFunc) _printStr);
	n += printf("}");
	return n;
}
