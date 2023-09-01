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
	int filelength = strlen(startTok->filename) + strlen(include->name) + 2;
	char *filename = malloc(sizeof(char) * filelength);
	sprintf(filename, "%s/%s", dirname(startTok->filename), include->name);
	FILE *result = fopen(filename, "r");
	free(filename);
	if (!result) {
		fprintf(stderr, "Can't open file %s", include->name);
		return NULL;
	}
	return result;
}

//TODO: I'll prob have to write my own header files
static char *_libs = "/usr/include";

FILE *_openLibraryFile(ASTMacroIncl *include) {
	FILE *result = NULL;

	int filenameLength = 256 + strlen(include->name);
	char *filename = malloc(sizeof(char) * filenameLength);
	char *libs = _libs;

	char *lib = strtok(libs, ":");
	while (lib) {
		int tempLength = (strlen(lib) + strlen(include->name) + 2) * sizeof(char);
		if (tempLength > filenameLength) {
			filenameLength = tempLength;
			filename = realloc(filename, tempLength);
		}
		sprintf(filename, "%s/%s", lib, include->name);
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
		fprintf(stderr, "Could not find library file %s\n", include->name);
	}
	return result;
}

void preprocessor(DList *tokens) {
	MacroDict macros;
	ASTState state;
	int startIndex;
	Token *startTok;
	
	initMacroDict(&macros);

	initASTState(&state, (Token *) dlistGetm(tokens, 0));
	while (state.tok->type != TT_EOF) {
		ASTMacroDef def;
		ASTMacroIncl include;
		ASTMacroIf macroIf;
		startTok = state.tok;
		startIndex = startTok - (Token *) dlistGetm(tokens, 0);

		if (parseASTMacroDef(&def, &state)) {
			macroDictInsert(&macros, strdup(def.name), def);
			dlistRemMult(tokens, startIndex, state.tok - startTok, (DListFreeFunc) freeToken);
			state.tok = tokListGetm(tokens, startIndex);
		} else if (parseASTMacroIncl(&include, &state)) {
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
			TokList newTokens = tokenize(fp, include.name);
			tokListRemLast(&newTokens); //remove EOF token
			dlistRemMult(tokens, startIndex, state.tok - startTok, (DListFreeFunc) freeToken);
			dlistInsMult(tokens, &newTokens, startIndex);
			state.tok = tokListGetm(tokens, startIndex);

			freeASTMacroIncl(&include);
		} else if (parseASTMacroIf(&macroIf, &state, &macros)) {

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