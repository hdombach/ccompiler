#include "astState.h"
#include <stdlib.h>

void initASTState(ASTState *state, Token *toks) {
	state->tok = toks;
	state->status = AST_STATUS_NOMINAL;
	state->errorMsg = NULL;
}

void astMergeState(ASTState *main, const ASTState *b) {
	if (b->status == AST_STATUS_NOMINAL) {
		main->tok = b->tok;
	} else if (b->status == AST_STATUS_ERROR) {
		main->status = AST_STATUS_ERROR;
		main->errorMsg = b->errorMsg;
	}
}

Token *astReqMacro(ASTState *state, TokenType macroType) {
	if (state->status <= AST_STATUS_FAILED) {
		return NULL;
	}
	if (state->tok->isMacro && state->tok->type == macroType) {
		return state->tok++;
	} else {
		state->status = AST_STATUS_ERROR;
		return NULL;
	}
}

Token *astExpMacro(ASTState *state, TokenType macroType) {
	if (state->status <= AST_STATUS_FAILED) {
		return NULL;
	}
	if (state->tok->isMacro && state->tok->type == macroType) {
		return state->tok++;
	} else {
		state->status = AST_STATUS_FAILED;
		return NULL;
	}
}

Token *astPop(ASTState *state) {
	if (state->status <= AST_STATUS_FAILED) {
		return NULL;
	}
	return state->tok++;
}

int astValid(ASTState *state) {
	return state->status == AST_STATUS_NOMINAL;
}
