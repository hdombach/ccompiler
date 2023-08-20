#include <stdio.h>

#include "argParser.h"
#include "ast/astState.h"
#include "ast/macroDef.h"
#include "ast/tokenParser.h"
#include "token.h"
#include "tokenizer.h"
#include "util/dlist.h"
#include "util/macroDict.h"

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
		Token *tok, *tokEnd;
		MacroDict macros;

		file = *(char **) dlistGet(&args.files, i);
		fp = fopen(file, "r");
		if (!fp) {
			perror("ree");
			return 1;
		}
		tokens = tokenize(fp, file);

		//dlistPrint(&tokens, (DListPrintFunc) printToken);

		tok = (Token *) dlistGet(&tokens, 0);
		tokEnd = tok + tokens.size;
		initMacroDict(&macros);
		while (tok <= tokEnd) {
			ASTState state;
			ASTMacroDef def;

			initASTMacroDef(&def);
			initASTState(&state, tok);
			if (parseASTMacroDef(&def, &state)) {
				macroDictInsert(&macros, strdup(def.name), def);
				tok = state.tok;
			} else {
				tok++;
			}

			if (state.status == TP_ERROR) {
				fprintf(stderr, "INTERNAL_ERROR");
			}
		}
		printMacroDict(&macros);

		freeMacroDict(&macros);
		freeDList(&tokens, (DListFreeFunc) freeToken);
	}

	printf("\n");
	freeArgs(&args);
	return 0;
}
