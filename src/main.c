#include <stdio.h>
#include <sys/types.h>

#include "argParser.h"
#include "ast/astState.h"
#include "ast/macroDef.h"
#include "ast/tokenParser.h"
#include "preprocessor.h"
#include "token.h"
#include "tokenizer.h"
#include "util/dlist.h"
#include "util/macroDict.h"

int main(int argc, char **argv) {
	Args args;
	DList tokens;

	initArgs(&args);

	if (!parseArgs(argc, argv, &args)) {
		freeArgs(&args);
		return 1;
	}

	if (args.help) {
		printf("%s\n", ARGS_HELP_MSG);
		freeArgs(&args);
		return 0;
	}

	for (int i = 0; i < args.files.size; i++) {
		char *file;
		FILE *fp;
		int n;

		file = *(char **) dlistGet(&args.files, i);
		fp = fopen(file, "r");
		if (!fp) {
			perror("ree");
			return 1;
		}
		tokens = tokenize(fp, file);

		//dlistPrint(&tokens, (DListPrintFunc) printToken);

		preprocessor(&tokens);

		freeDList(&tokens, (DListFreeFunc) freeToken);
	}

	printf("\n");
	freeArgs(&args);
	return 0;
}
