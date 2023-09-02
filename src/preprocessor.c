#include "preprocessor.h"
#include "ast/astUtil.h"
#include "ast/macroDef.h"
#include "ast/macroIf.h"
#include "ast/macroInclude.h"
#include "ast/macroUndef.h"
#include "ast/tokenParser.h"
#include "token.h"
#include "tokenizer.h"
#include "util/dlist.h"
#include "util/macroDict.h"
#include "util/tokList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

FILE *_openDirectFile(ASTMacroIncl *include, Token *startTok) {
	FILE *result = fopen(include->filename, "r");
	if (!result) {
		fprintf(stderr, "Can't open file %s", include->filename);
		return NULL;
	}
	return result;
}

//TODO: I'll prob have to write my own header files
static char *_libs = "/usr/include";

FILE *_openLibraryFile(ASTMacroIncl *include) {
	FILE *result = NULL;

	int filenameLength = 256 + strlen(include->filename);
	char *filename = malloc(sizeof(char) * filenameLength);
	char *libs = _libs;

	char *lib = strtok(libs, ":");
	while (lib) {
		int tempLength = (strlen(lib) + strlen(include->filename) + 2) * sizeof(char);
		if (tempLength > filenameLength) {
			filenameLength = tempLength;
			filename = realloc(filename, tempLength);
		}
		sprintf(filename, "%s/%s", lib, include->filename);
		printf("Tyring to open file %s\n", filename);
		result = fopen(filename, "r");
		if (result) {
			break;
		}

		lib = strtok(NULL, ":");
		result = NULL;
	}
	
	free(filename);
	if (!result) {
		fprintf(stderr, "Could not find library file %s\n", include->filename);
	}
	return result;
}

void preprocessor(DList *tokens) {
	MacroDict macros;
	int res, n = 0;
	Token *tok;
	
	initMacroDict(&macros);

	tok = (Token *) dlistGetm(tokens, 0);
	while (tok[n].type != TT_EOF) {
		union {
			ASTMacroDef astDef;
			ASTMacroIncl astIncl;
			ASTMacroIf astIf;
			ASTMacroUndef astUndef;
		} u;

		if ((res = parseASTMacroDef(&u.astDef, tok + n))) {
			macroDictInsert(&macros, strdup(u.astDef.name), u.astDef);
			dlistRemMult(tokens, n, res, (DListFreeFunc) freeToken);
			tok = (Token *) dlistGetm(tokens, 0);
		} else if ((res = parseASTMacroIncl(&u.astIncl, tok + n))) {
			//printASTMacroIncl(&include);
			FILE *fp;
			if (u.astIncl.type == AST_MIT_DIRECT) {
				fp = _openDirectFile(&u.astIncl, tok);
			} else if (u.astIncl.type == AST_MIT_LIBRARY) {
				fp = _openLibraryFile(&u.astIncl);
			}
			if (!fp) {
				break;
			}
			TokList newTokens = tokenize(fp, u.astIncl.filename);
			tokListRemLast(&newTokens); //remove EOF token
			dlistRemMult(tokens, n, res, (DListFreeFunc) freeToken);
			dlistInsMult(tokens, &newTokens, n);
			tok = (Token *) dlistGetm(tokens, 0);

			freeASTMacroIncl(&u.astIncl);
		} else if (parseASTMacroIf(&u.astIf, tok + n, &macros)) {

			ASTMacroIf *cur = &u.astIf;
			int curOffset = 0;
			while (cur) {
				dlistRemMult(tokens, n + cur->start - curOffset, cur->end - cur->start, (DListFreeFunc) freeToken);
				curOffset += cur->end - cur->start;
				cur = cur->next;
			}
			tok = (Token *) dlistGetm(tokens, 0);
			freeASTMacroIf(&u.astIf);
		} else if ((res = parseASTMacroUndef(&u.astUndef, tok + n))) {
			macroDictDelete(&macros, u.astUndef.name);
			dlistRemMult(tokens, n, res, (DListFreeFunc) freeToken);
			tok = (Token *) dlistGetm(tokens, 0);
			freeASTMacroUndef(&u.astUndef);
		} else {
			n++;
		}

		if (astErrMsg) {
			fprintf(stderr, "%s", astErrMsg);
			freeMacroDict(&macros);
			exit(1);
		}
	}

	//printMacroDict(&macros);
	freeMacroDict(&macros);
}
