#pragma once

#include "../util/wordDict.h"
#include "../util/util.h"
#include "../util/dlist.h"

struct ASTStm;

typedef struct ASTScope {
	/**
	 * @brief Used while building AST to keep track of typedefs`
	 */
	WordDict typedefNames;

	/**
	 * @brief An index into labels
	 */
	WordDict labelDict;
	/**
	 * @brief Pointers to statements which contain the lables
	 */
	DList labels;
	struct ASTScope *parent;
} ASTScope;

typedef enum ASTScopeErr {
	SCOPE_SUCCESS,
	SCOPE_EXISTS,
	SCOPE_INVALID_ARG,
} ASTScopeErr;

void initASTScope(ASTScope *scope);
void freeASTScope(ASTScope *scope);

int astScopeIsTypedef(ASTScope *scope, char const *name);
void astScopeAddTypedefName(ASTScope *scope, char *name);
/* Note: automatically free's names */
void astScopeAddTypedefNames(ASTScope *scope, DList names);

/**
 * brief Adds a label to the scope
 *
 * @param scope
 * @param stm
 */
ASTScopeErr astScopeAddLabel(ASTScope *scope, struct ASTStm *stm);
/**
 * @brief Searches for label in the current scope
 *
 * @param scope
 * @param labelName
 *
 * @return Statement containing the label. NULL if not found
 */
struct ASTStm *astScopeGetLabel(ASTScope *scope, const char *labelName);
