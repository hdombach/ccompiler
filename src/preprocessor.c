#include "preprocessor.h"
#include "ast/astState.h"
#include "ast/macroDef.h"
#include "ast/macroIf.h"
#include "ast/macroInclude.h"
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
	ASTState state;
	int startIndex;
	int res;
	Token *startTok;
	
	initMacroDict(&macros);

	initASTState(&state, (Token *) dlistGetm(tokens, 0));
	while (state.tok->type != TT_EOF) {
		ASTMacroDef def;
		ASTMacroIncl include;
		ASTMacroIf macroIf;
		startTok = state.tok;
		startIndex = startTok - (Token *) dlistGetm(tokens, 0);

		if ((res = parseASTMacroDef(&def, state.tok))) {
			macroDictInsert(&macros, strdup(def.name), def);
			dlistRemMult(tokens, startIndex, res, (DListFreeFunc) freeToken);
			state.tok = tokListGetm(tokens, startIndex);
		} else if ((res = parseASTMacroIncl(&include, state.tok))) {
			//printASTMacroIncl(&include);
			FILE *fp;
			if (include.type == AST_MIT_DIRECT) {
				fp = _openDirectFile(&include, startTok);
			} else if (include.type == AST_MIT_LIBRARY) {
				fp = _openLibraryFile(&include);
			}
			if (!fp) {
				break;
			}
			TokList newTokens = tokenize(fp, include.filename);
			tokListRemLast(&newTokens); //remove EOF token
			dlistRemMult(tokens, startIndex, res, (DListFreeFunc) freeToken);
			dlistInsMult(tokens, &newTokens, startIndex);
			state.tok = tokListGetm(tokens, startIndex);

			freeASTMacroIncl(&include);
		} else if ((res = parseASTMacroIf(&macroIf, state.tok, &macros))) {

			ASTMacroIf *cur = &macroIf;
			int curOffset = 0;
			while (cur) {
				dlistRemMult(tokens, startIndex + cur->start - curOffset, cur->end - cur->start, (DListFreeFunc) freeToken);
				curOffset += cur->end - cur->start;
				cur = cur->next;
			}
			state.tok = tokListGetm(tokens, startIndex);
			astPop(&state);
			freeASTMacroIf(&macroIf);
		} else {
			Token *tok = astPop(&state);
			if (!tok) {
			} else if (tok->type == TT_MACRO_IFDEF) {
			}
		}

		if (state.status == AST_STATUS_ERROR) {
			fprintAstError(stderr, &state);
			exit(1);
		}
	}

	//printMacroDict(&macros);
	freeMacroDict(&macros);
}
