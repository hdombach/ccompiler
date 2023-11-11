#include <stdio.h>
#include <sys/types.h>

#include "argParser.h"
#include "ast/astUtil.h"
#include "ast/macroDef.h"
#include "ast/node.h"
#include "ast/tokenParser.h"
#include "ast/file.h"
#include "preprocessor.h"
#include "token.h"
#include "tokenizer.h"
#include "util/tokList.h"
#include "util/dlist.h"
#include "util/macroDict.h"
#include "sem/typeGen.h"

ASTTravRes travTest(ASTNode *node, ASTTravCtx *_) {
	printf("%s\n", astNodeTypeStr(node->type));
	return ASTT_SUCCESS;
}

int main(int argc, char **argv) {
	TokList tokens;

	initArgs(&g_args);

	if (!parseArgs(argc, argv, &g_args)) {
		freeArgs(&g_args);
		return 1;
	}

	if (g_args.help) {
		printf("%s\n", ARGS_HELP_MSG);
		freeArgs(&g_args);
		return 0;
	}

	for (int i = 0; i < g_args.files.size; i++) {
		char *file;
		FILE *fp;
		int n;

		file = *(char **) dlistGet(&g_args.files, i);
		fp = fopen(file, "r");
		if (!fp) {
			perror("ree");
			return 1;
		}
		tokens = tokenize(fp, file);

		//printDList(&tokens, (DListPrintFunc) printToken);

		preprocessor(&tokens);

		//printrTokList(&tokens);
		//printf("\n");

		ASTFile astFile;
		if (parseASTFile(&astFile, tokListGetm(&tokens, 0))) {
			typeGen(&astFile);
			printASTFile(&astFile);
			freeASTFile(&astFile);
		} else {
			printf("not successful\n");
			if (astHasErr()) {
				fprintASTErr(stderr);
			}
		}

		freeTokList(&tokens);
	}

	freeArgs(&g_args);
	return 0;
}
