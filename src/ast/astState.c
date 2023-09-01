#include "astState.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
		snprintf(ASTStateMsg, AST_STATE_MSG_S, "Expected token %s", tokTypeStr(state->tok->type));
		astError(state, ASTStateMsg);
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

Token *astPopMacro(ASTState *state) {
	if (state->status <= AST_STATUS_FAILED) {
		return NULL;
	}

	Token *res = state->tok++;
	if (res->isMacro) {
		return res;
	} else {
		return NULL;
	}
}

int astValid(ASTState *state) {
	return state->status == AST_STATUS_NOMINAL;
}

void astError(ASTState *state, char *msg) {
	state->status = AST_STATUS_ERROR;
	state->errorMsg = msg;
}

void fprintAstError(FILE *file, ASTState *state) {
	fprintf(file, "%d:%d, %s\n", state->tok->posLine, state->tok->posColumn, state->errorMsg);
}
