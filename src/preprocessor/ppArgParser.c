#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ppArgParser.h"
#include "lines.h"

const char *PP_HELP_MSG =
"usage: cmd [-h | --help] [--stage <name>] file...\n"
"\n"
"ARGS:\n"
"\t--help: Shows this message.\n"
"\t--stage <\"backslash\">: Only applies change up to a specific stage\n"
"";

char *_ppArgParserErr = NULL;

/*
 * Updates parser error and takes ownership of err
 */
void _osetParserErr(char *err) {
	if (_ppArgParserErr) {
		free(_ppArgParserErr);
	}
	_ppArgParserErr = err;
}

void _setParserErr(const char *err) {
	char *newErr;

	newErr = malloc(sizeof(char) * (strlen(err) + 1));
	strcpy(newErr, err);
	_osetParserErr(newErr);
}

/*
 * Parses one argument from the provided args
 * Returns the number of parsed words or 0 on failure
 */
int _parseArg(int argc, char **argv, PPArgs *args) {
	char *err;

	if (argc < 1) {
		return 0;
	}

	if (strcmp(argv[0], "--help") == 0 || strcmp(argv[0], "-h") == 0) {
		args->help = 1;
		return 1;
	} else if (strcmp(argv[0], "--stage") == 0) {
		if (argc < 2) {
			_setParserErr("--stage argument requires a field <name>");
			return 0;
		} else if (strcmp(argv[1], "backslash") == 0) {
			args->stage = PP_STAGE_BACKSLASH;
			return 2;
		} else {
			asprintf(&err, "Invalid --stage option %s", argv[1]);
			_osetParserErr(err);
			return 0;
		}
	} else if (argv[0][0] == '-') {
		asprintf(&err, "Invalid option %s", argv[0]);
		_osetParserErr(err);
		return 0;
	} else {
		linesApp(&args->files, argv[0]);
		return 1;
	}
}

PPArgs initPPArgs() {
	PPArgs result;

	result.help = 0;
	result.stage = -1; //TODO: need an initial value
	result.files = initLines();

	return result;
}

int ppParseArgs(int argc, char **argv, PPArgs *args) {
	int progress;
	int parseRes;

	//skip this first item
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

char *ppArgParserErr() {
	if (_ppArgParserErr) {
		return _ppArgParserErr;
	} else {
		return "";
	}
}

void ppArgsPrint(const PPArgs *args) {
	printf("{");
	printf("\"stage\": %d", args->stage); //TODO: string representation
	printf(", \"help\": %d", args->help); //TODO: string representation
	printf(", \"files\": ");
	linesPrint(&args->files);
	printf("}");
}
