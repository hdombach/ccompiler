#pragma once

#include "util/wordList.h"

typedef struct {
	WordList files;
	int help;
	int verbose;
} Args;

extern Args g_args;

extern const char *ARGS_HELP_MSG;

void initArgs(Args *args);
void freeArgs(Args *args);

int parseArgs(int argc, char **argv, Args *args);

int argsPrint(Args const *args);
