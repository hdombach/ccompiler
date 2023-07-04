#pragma once

#include "lines.h"

enum PPStage {
	PP_STAGE_BACKSLASH = 1
};

typedef struct {
	Lines files;
	int stage;
	/* Whether or not to show help */
	int help;
} PPArgs;

extern const char *PP_HELP_MSG;

PPArgs initPPArgs();

/*
 * Parses the provided args.
 * Returns 1 for success and 0 for fail.
 */
int ppParseArgs(int argc, char **argv, PPArgs *args);

/*
 * Returns the last error
 */
char *ppArgParserErr();

void ppArgsPrint(PPArgs const *args);
