#pragma once

#include "../token.h"
#include <stdio.h>

typedef enum {
	AST_STATUS_NOMINAL = 1,
	AST_STATUS_FAILED = 0,
	AST_STATUS_ERROR = -1,
} ASTStatus;

enum {
	AST_STATE_MSG_S = 256 
};

static char astErrMsgBuf[AST_STATE_MSG_S];
static char ASTStateMsg[AST_STATE_MSG_S];
static char *astErrMsg = NULL;

typedef struct {
	Token *tok;
	ASTStatus status;
	char *errorMsg;
} ASTState;

void initASTState(ASTState *state, Token *toks);
void astMergeState(ASTState *main, ASTState const *b);

int astMacro(Token *tok, TokenType type);
Token *astReqMacro(ASTState *state, TokenType macroType);
Token *astExpMacro(ASTState *state, TokenType macroType);
Token *astPop(ASTState *state);
Token *astPopMacro(ASTState *state);
int astValid(ASTState *state);
void astError(ASTState *state, char *msg);
void fprintAstError(FILE *file, ASTState *state);
