#pragma once

#include "../util/wordDict.h"
#include "../util/util.h"

typedef struct ASTScope {
	/* A quick and dirty dict for keeping track of typedefs while building ast*/
	WordDict typedefNames;
	struct ASTScope *parent;
} ASTScope;
void initASTScope(ASTScope *scope);
void freeASTScope(ASTScope *scope);

int astScopeIsTypedef(ASTScope const *scope, char const *name);
void astScopeAddTypedefName(ASTScope *scope, char *name);
/* Note: automatically free's names */
void astScopeAddTypedefNames(ASTScope *scope, DList names);
