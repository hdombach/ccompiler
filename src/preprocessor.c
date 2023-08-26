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
	ASTState state;
	
	initMacroDict(&macros);

	initASTState(&state, (Token *) dlistGetm(tokens, 0));
	while (state.tok->type != TT_EOF) {
		ASTMacroDef def;
		ASTMacroIncl include;

		initASTMacroDef(&def);
		if (parseASTMacroDef(&def, &state)) {
			macroDictInsert(&macros, strdup(def.name), def);
		} else if (parseASTMacroIncl(&include, &state)) {
			printASTMacroIncl(&include);
			freeASTMacroIncl(&include);
		} else {
			Token *tok = astPop(&state);
		}

		if (state.status == AST_STATUS_ERROR) {
			fprintAstError(stderr, &state);
			exit(1);
		}
	}

	printMacroDict(&macros);
	freeMacroDict(&macros);
}
