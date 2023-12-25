#pragma once

#include "../util/log.h"
#include "../ast/node.h"
#include "../token.h"
#include "../util/util.h"
#include "../ast/node.h"

void tTokensSuccess(const char *code, TokenType types[]);

void tTokensFailed(const char *code, CError *errors);

void tAstSuccess(const char *code, ASTNodeType types[]);

void tAstFailed(const char *code, CError *error);
