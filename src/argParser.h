#pragma once

#include "util/dlist.h"

typedef struct {
	DList files;
	int help;
} Args;

extern const char *ARGS_HELP_MSG;

void initArgs(Args *args);
void freeArgs(Args *args);

int parseArgs(int argc, char **argv, Args *args);

void argsPrint(Args const *args);
