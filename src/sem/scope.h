#pragma once

#include "../util/wordDict.h"
#include "../util/util.h"
#include "../util/dlist.h"

typedef struct ASTScope {
	/**
	 * @brief Used while building AST to keep track of typedefs`
	 */
	WordDict typedefNames;
	struct ASTScope *parent;
} ASTScope;
void initASTScope(ASTScope *scope);
void freeASTScope(ASTScope *scope);

int astScopeIsTypedef(ASTScope *scope, char const *name);
void astScopeAddTypedefName(ASTScope *scope, char *name);
/* Note: automatically free's names */
void astScopeAddTypedefNames(ASTScope *scope, DList names);
