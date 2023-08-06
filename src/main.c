#include <stdio.h>

#include "argParser.h"
#include "ast/macroDef.h"
#include "ast/tokenParser.h"
#include "token.h"
#include "tokenizer.h"
#include "util/dlist.h"

#define HELLO 

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

		dlistPrint(&tokens, (DListPrintFunc) printToken);

		while (n < tokens.size) {
			ASTMacroDef def;
			int res;

			initASTMacroDef(&def);
			res = parseASTMacroDef(&def, dlistGetm(&tokens, n));
			if (res > 0) {
				printASTMacroDef(&def);
				n += res;
			} else {
				n++;
			}
			if (res == TP_ERROR) {
				fprintf(stderr, "INTERNAL_ERROR");
			}
			freeASTMacroDef(&def);
		}

		freeDList(&tokens, (DListFreeFunc) freeToken);
	}

	printf("\n");
	freeArgs(&args);
	return 0;
}
