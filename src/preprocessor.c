#include "preprocessor.h"
#include "ast/astState.h"
#include "ast/macroDef.h"
#include "ast/macroInclude.h"
#include "ast/tokenParser.h"
#include "token.h"
#include "util/dlist.h"
#include "util/macroDict.h"

#include <stdio.h>
#include <string.h>

void preprocessor(DList *tokens) {
	MacroDict macros;
	
	initMacroDict(&macros);

	Token *tok, *tokEnd;
	tok = (Token *) dlistGetm(tokens, 0);
	tokEnd = tok + tokens->size;
	while (tok <= tokEnd) {
		ASTState state;
		ASTMacroDef def;
		ASTMacroIncl include;

		initASTMacroDef(&def);
		initASTState(&state, tok);
		if (parseASTMacroDef(&def, &state)) {
			macroDictInsert(&macros, strdup(def.name), def);
			tok = state.tok;
		} else if (parseASTMacroIncl(&include, &state)) {
			printASTMacroIncl(&include);
			freeASTMacroIncl(&include);
			tok = state.tok;
		} else {
			tok++;
		}

		if (state.status == AST_STATUS_ERROR) {
			fprintAstError(stderr, &state);
			exit(1);
		}
		if (tok->type == TT_EOF) {
			break;
		}
	}

	printMacroDict(&macros);
	freeMacroDict(&macros);
}
