#pragma once

#include "../util/log.h"
#include "../ast/node.h"
#include "../token.h"
#include "../util/util.h"

void tTokensSuccess(const char *code, TokenType types[]);

#define T_INTEGRATION_F(CODE, ERRORS) \
	tIntegrationFailed(CODE, ERRORS, COUNTOF(ERRORS))
void tIntegrationFailed(const char *code, CError *errors, int errors_s);

#define T_INTEGRATION_S(CODE, TYPES) \
	tIntegrationSuccess(CODE, TYPES, COUNTOF(TYPES))
void tIntegrationSuccess(const char *code, ASTNodeType *types, int types_s);
