#pragma once

#include "../util/wordDict.h"
#include "../util/util.h"
#include "../util/dlist.h"

struct ASTStm;
struct SCompound;
struct SType;

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

	WordDict structDict;
	DList structs;

	WordDict identifierDict;
	DList identifiers;
	struct ASTScope *parent;
} ASTScope;

typedef enum ASTScopeErr {
	SCOPE_SUCCESS,
	SCOPE_EXISTS,
	SCOPE_INVALID_ARG,
} ASTScopeErr;

void initASTScope(ASTScope *scope);
void freeASTScope(ASTScope *scope);
int printASTScope(ASTScope const *scope);

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

int astScopeHasCompound(ASTScope *scope, const char *name);

int astScopeAddCompound(ASTScope *scope, struct SCompound *compound, char *name);

struct SCompoundRef astScopeGetCompound(ASTScope *scope, const char *name);

struct SCompoundRef astScopeAddAnonCompound(ASTScope *scope, struct SCompound*);

struct SCompoundRef astScopeAddIdentifier(ASTScope *scope, struct SType *type, char *name);
