#include "preprocessor.h"
#include "../ast/astUtil.h"
#include "../ast/macroDef.h"
#include "../ast/macroIf.h"
#include "../ast/macroInclude.h"
#include "../ast/macroUndef.h"
#include "../ast/tokenParser.h"
#include "token.h"
#include "tokenizer.h"
#include "../util/dlist.h"
#include "../util/macroDict.h"
#include "../util/stream.h"
#include "../util/tokList.h"
#include "../util/callbacks.h"
#include "../util/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

static FILE *_openDirectFile(ASTMacroIncl *include, Token *startTok) {
	FILE *result = fopen(include->filename, "r");
	if (!result) {
		fprintf(stderr, "Can't open file %s", include->filename);
		return NULL;
	}
	return result;
}

//TODO: I'll prob have to write my own header files
static char *_libs = "/usr/include";

static FILE *_openLibraryFile(ASTMacroIncl *include) {
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

typedef struct _TokenRange {
	Token const *start;
	Token const *end;
} _TokenRange;


static int _expandMacro(TokList *insert, ASTMacroDef *macro, Token const *tok) {
	int n = 1, curDepth = 0;
	DList ranges;
	_TokenRange curRange;

	if (macro->paramNames.size > 0) {
		initDList(&ranges, sizeof(_TokenRange));

		if (tok[n].type == TT_O_PARAN) {
			n++;
		} else {
			logCerr(CERR_BRACE, tok + n, "Expecting ( following macro name");
			freeDList(&ranges, NULL);
			return 0;
		}

		curRange.start = tok + n;
		while (1) {
			if (tok[n].type == TT_EOF) {
				logCerr(CERR_BRACE, tok + n, "Expected ) at end of macro");
				freeDList(&ranges, NULL);
				return 0;
			} else if (tok[n].type == TT_COMMA && curDepth == 0) {
				curRange.end = tok + n;
				dlistApp(&ranges, &curRange);
				curRange.start = tok + n + 1;
			} else if (tok[n].type == TT_C_PARAN && curDepth == 0) {
				curRange.end = tok + n;
				dlistApp(&ranges, &curRange);
				n++;
				break;
			} else {
				curDepth += tokenBracketDepth(tok[n].type);
			}
			n++;
		}

		if (ranges.size != macro->paramNames.size) {
			logCerr(
					CERR_UNKNOWN,
					tok + n,
					"Incorrect number of paran names (%d != %d)",
					ranges.size,
					macro->paramNames.size);
			freeDList(&ranges, NULL);
			return 0;
		}
	}

	for (int i = 0; i < macro->nodes.size; i++) {
		ASTMacroDefNode *node;
		node = dlistGetm(&macro->nodes, i);
		if (node->paramIndex >= 0) {
			curRange = * (_TokenRange *) dlistGetm(&ranges, node->paramIndex);
			for (Token const *t = curRange.start; t != curRange.end; t++) {
				Token new;
				cpToken(&new, t);
				new.posLine = tok->posLine;
				dlistApp(insert, &new);
			}
		} else {
			Token new;
			cpToken(&new, node->token);
			new.posLine = tok->posLine;
			dlistApp(insert, &new);
		}
	}

	if (macro->insertFunc) {
		macro->insertFunc(insert, tok);
	}

	if (macro->paramNames.size > 0) {
		freeDList(&ranges, NULL);
	}
	return n;
}

void preprocessor(DList *tokens) {
	MacroDict macros;
	int res, n = 0;
	Token *tok;
	
	initMacroDict(&macros);
	macroDictInsertDefault(&macros);

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
			dlistRemMult(tokens, n, res, (FreeFunc) freeToken);
			tok = (Token *) dlistGetm(tokens, 0);
		} else if ((res = parseASTMacroIncl(&u.astIncl, tok + n))) {
			//printASTMacroIncl(&include);
			FILE *fp;
			Stream stream;
			if (u.astIncl.type == AST_MIT_DIRECT) {
				fp = _openDirectFile(&u.astIncl, tok);
			} else if (u.astIncl.type == AST_MIT_LIBRARY) {
				fp = _openLibraryFile(&u.astIncl);
			}
			if (!fp) {
				break;
			}
			initStreamFile(&stream, fp);
			TokList newTokens = tokenize(&stream, u.astIncl.filename);
			tokListRemLast(&newTokens); //remove EOF token
			dlistRemMult(tokens, n, res, (FreeFunc) freeToken);
			dlistInsMult(tokens, &newTokens, n);
			tok = (Token *) dlistGetm(tokens, 0);

			freeASTMacroIncl(&u.astIncl);
		} else if (parseASTMacroIf(&u.astIf, tok + n, &macros)) {

			ASTMacroIf *cur = &u.astIf;
			int curOffset = 0;
			while (cur) {
				dlistRemMult(tokens, n + cur->start - curOffset, cur->end - cur->start, (FreeFunc) freeToken);
				curOffset += cur->end - cur->start;
				cur = cur->next;
			}
			tok = (Token *) dlistGetm(tokens, 0);
			freeASTMacroIf(&u.astIf);
		} else if ((res = parseASTMacroUndef(&u.astUndef, tok + n))) {
			macroDictDelete(&macros, u.astUndef.name);
			dlistRemMult(tokens, n, res, (FreeFunc) freeToken);
			tok = (Token *) dlistGetm(tokens, 0);
			freeASTMacroUndef(&u.astUndef);
		} else if (tok[n].type == TT_IDENTIFIER && macroDictPresent(&macros, tok[n].contents)) {
			TokList insert;
			initTokList(&insert);
			res = _expandMacro(&insert, macroDictGetm(&macros, tok[n].contents), tok + n);
			dlistRemMult(tokens, n, res, (FreeFunc) freeToken);
			dlistInsMult(tokens, &insert, n);
			tok = tokListGetm(tokens, 0);
		} else {
			n++;
		}

		if (astHasErr()) {
			freeMacroDict(&macros);
			exit(1);
		}
	}

	//printMacroDict(&macros);
	freeMacroDict(&macros);
}
