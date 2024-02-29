#pragma once

#include "../util/wordDict.h"
#include "../util/dlist.h"

typedef struct ASTNode ASTNode;
typedef struct SCompound SCompound;
typedef struct SType SType;
struct SEnum;
typedef struct STypedef STypedef;
typedef struct ASTLblIdentifier ASTLblIdentifier;
typedef struct SCompoundRef SCompoundRef;
typedef struct SEnumRef SEnumRef;

typedef struct ASTScope {
	/**
	 * @brief Used while building AST to keep track of typedefs`
	 * Only the keys in this dict matter
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

	/**
	 * @brief used for structs, unions, and enums
	 */
	WordDict tagDict;
	/**
	 * Contains SEnum or SCompound
	 * Is mainly used to determain if new struct can be added.
	 */
	DList tags;

	/**
	 * @brief Maps names to types in identifiers list
	 */
	WordDict identifierDict;
	/**
	 * @brief List of identifier types
	 * Contains SType
	 */
	DList identifiers;
	struct ASTScope *parent;
	char isFileScope;
} ASTScope;

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
 * @returns Whether the scope was successfully added
 */
int astScopeAddLabel(ASTScope *scope, ASTLblIdentifier *lbl);
/**
 * @brief Searches for label in the current scope
 *
 * @param scope
 * @param labelName
 *
 * @return Statement containing the label. NULL if not found
 */
ASTNode *astScopeGetLabel(ASTScope *scope, const char *labelName);


/**
 * @brief Checks whether the scope contains a tag
 * @param[in] scope
 * @param[in] name
 * @returns If the scope containes the compound
 */
int astScopeHasTag(ASTScope *scope, const char *name);

/**
 * @brief Gets enum, union, or struct
 * @param[in] scope
 * @param[in] name
 * @returns Returns SCompoundRef or SEnumRef
 */
ASTNode *astScopeGetTag(ASTScope *scope, const char *name);

/**
 * @brief Adds a compound to the scope
 * @param[in] scope
 * @param[in] compound
 * @param[in] name
 * @returns Where the compound is stored
 * @note Takes ownership of name, caller takes ownership of result
 */
SCompoundRef *astScopeAddCompound(ASTScope *scope, struct SCompound *compound, char *name);

SCompoundRef *astScopeAddAnonCompound(ASTScope *scope, struct SCompound*);

/**
 * @brief Adds an identifier
 *
 * @param[in] scope Scope to add the type into
 * @param[in] type Type to add
 * @param[in] name (takes ownership)
 *
 * @return 1 on success, 0 on error
 */
int astScopeAddIdent(ASTScope *scope, struct SType *type, char *name);

/**
 * @brief Adds an anonymous identifier
 */
int astScopeAddAnonIdent(ASTScope *scope, struct SType *type);

/**
 * @brief Gets the type for an identifier
 *
 * @param[in] scope
 * @param[in] name
 *
 * @returns Reference to type of identifier or NULL if not found
 */
SType *astScopeGetIdentifier(ASTScope *scope, char *name);

/**
 * @brief Gets the type for the corresponding identifier
 *
 * @param[in] scope
 * @param[in] name
 *
 * @returns The reference to underlying type or NULL if not found
 */
SType *astScopeGetTypedef(ASTScope *scope, char *name);

int astScopeHasEnum(ASTScope *scope, const char *name);

int astScopeAddEnum(ASTScope *scope, struct SEnum *type, char *name);

SEnumRef *astScopeGetEnum(ASTScope *scope, const char *name);

SEnumRef *astScopeAddAnonEnum(ASTScope *scope, struct SEnum*);
