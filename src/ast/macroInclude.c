#include "macroInclude.h"
#include "astState.h"
#include "../util/dstr.h"

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initASTMacroIncl(ASTMacroIncl *node) {
	node->filename = NULL;
}

void freeASTMacroIncl(ASTMacroIncl *node) {
	if (node->filename) {
		free(node->filename);
	}
}

int parseASTMacroIncl(ASTMacroIncl *node, Token const *tok) {
	DStr str;
	int n = 0;

	initASTMacroIncl(node);

	if (astMacro(tok + n, TT_MACRO_INCLUDE)) {
		n++;
	} else if (astErrMsg) {
		freeASTMacroIncl(node);
		return 0;
	}

	if (astMacro(tok + n, TT_STR_CONSTANT)) {
		int filelength = strlen(tok[n].filename) + strlen(tok[n].contents) + 2;
		node->filename = malloc(sizeof(char) * filelength);
		sprintf(node->filename, "%s/%s", dirname(tok[n].filename), tok[n].contents);
		node->type = AST_MIT_DIRECT;
		n++;
	} else if (astMacro(tok + n, TT_LESS)) {
		DStr currFile;

		initDStr(&currFile);
		while (1) {
			if (astMacro(tok + n, TT_GREATER)) {
				n++;
				break;
			} else if (astMacro(tok + n, TT_IDENTIFIER)) {
				dstrAppStr(&currFile, tok[n].contents);
			} else if (astMacro(tok + n, TT_PERIOD)) {
				dstrApp(&currFile, '.');
			} else if (astMacro(tok + n, TT_DIV)) {
				dstrApp(&currFile, '/');
			} else {
				astErrMsg = "Invalid file name";
				freeDStr(&currFile);
				freeASTMacroIncl(node);
				return 0;
			}
			n++;
		}
		node->filename = currFile.data;
		node->type = AST_MIT_LIBRARY;
	} else {
		freeASTMacroIncl(node);
		astErrMsg = "Invalid token following \%include";
		return 0;
	}

	return n;
}

void printASTMacroInclType(ASTMacroInclType type) {
	switch (type) {
		case AST_MIT_DIRECT:
			printf("Local File");
			break;
		case AST_MIT_LIBRARY:
			printf("library File");
			break;
	}
}

void printASTMacroIncl(const ASTMacroIncl *node) {
	printf("{");

	printf("\"type\": \"Macro Include\"");

	printf(", \"filename\": ");
	printJsonStr(node->filename);

	printf(", \"filetype\": \"");
	printASTMacroInclType(node->type);
	printf("\"");

	printf("}");
}
