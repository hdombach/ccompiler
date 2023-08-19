#pragma once

#include "../token.h"

typedef enum {
	AST_STATUS_NOMINAL = 1,
	AST_STATUS_FAILED = 0,
	AST_STATUS_ERROR = -1,
} ASTStatus;

typedef struct {
	Token *tok;
	ASTStatus status;
	char *errorMsg;
} ASTState;

void initASTState(ASTState *state, Token *toks);
void astMergeState(ASTState *main, ASTState const *b);

Token *astReqMacro(ASTState *state, TokenType macroType);
Token *astExpMacro(ASTState *state, TokenType macroType);
Token *astPop(ASTState *state);
int astValid(ASTState *state);

