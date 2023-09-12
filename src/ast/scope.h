#pragma once

#include "../util/typeDict.h"

typedef struct ASTScope {
	TypeDict types;
	struct ASTScope *parent;
} ASTScope;

void initASTScope(ASTScope *scope);
void freeASTScope(ASTScope *scope);
int printASTScope(ASTScope const *scope);

int astScopeInsert(ASTScope *scope, ASTType type);
/*
 * Note: consumes types list
 */
int astScopeInsertMult(ASTScope *scope, DList *types);

int astScopePresent(ASTScope const *scope, char const *name);

ASTType const *astScopeGet(ASTScope const *scope, char const *name);
/*
 * Note: dict does not update if you change name
 */
ASTType *astScopeGetm(ASTScope *scope, char const *name);

ASTType *astScopeGetmLoc(ASTScope *scope, char *name);
