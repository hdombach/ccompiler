#pragma once

#include "../tok/token.h"
#include "../util/dlist.h"
#include "specialDecl.h"
#include "identifier.h"
#include "initializer.h"
#include "node.h"

/**
 * @file
 * @brief AST Nodes used for identifier declarations
 *
 * See also [Declarations](https://en.cppreference.com/w/c/language/declarations)
 */

typedef struct ASTScope ASTScope;

/***********************************************************************
 * ASTTypeQualifier
 ***********************************************************************/

/**
 * @brief Bitfield listing qualifiers applied
 */
typedef enum  {
	AST_TQ_NONE = 0b0,
	AST_TQ_CONST = 0b1,
	AST_TQ_VOLATILE = 0b10,
} ASTTypeQualifier;

/**
 * @param[out] qualifiers
 */
void initASTTypeQualifier(ASTTypeQualifier *qualifiers);
/**
 * @param[out] qualifiers
 * @param[in] tok
 * @returns The number of tokens parsed
 */
int parseASTTypeQualifier(ASTTypeQualifier *qualifiers, Token const *tok);
/**
 * @param[in] qualifiers
 * @returns the number of characters printed
 */
int printASTTypeQualifier(ASTTypeQualifier const *qualifiers);

/***************************************************************************
 * ASTStorageClassSpec 
 ***************************************************************************/

/**
 * @brief Bitfield listing storage class specifiers applied
 */
typedef enum {
	AST_SC_NONE = 0b0,
	AST_SC_TYPEDEF = 0b1,
	AST_SC_AUTO = 0b10,
	AST_SC_REGISTER = 0b100,
	AST_SC_STATIC = 0b1000,
	AST_SC_EXTERN = 0b10000,
} ASTStorageClassSpec;

/**
 * @param[out] specs
 */
void initASTStorageClassSpec(ASTStorageClassSpec *specs);
/**
 * @param[out] specs
 * @param[in] tok
 * @returns The number of tokens parsed
 */
int parseASTStorageClassSpec(ASTStorageClassSpec *specs, Token const *tok);
/**
 * @param[in] specs
 * @returns The number of characters printed
 */
int printASTStorageClassSpec(ASTStorageClassSpec const *specs);

/***************************************************************************
 * ASTArithType
 ***************************************************************************/

/**
 * @brief Bitfield listing arithmatic keywords
 */
typedef enum {
	AST_AT_NONE = 0b0,
	AST_AT_CHAR = 0b1,
	AST_AT_INT = 0b10,
	AST_AT_SIGNED = 0b100,
	AST_AT_UNSIGNED = 0b1000,
	AST_AT_SHORT = 0b10000,
	AST_AT_LONG = 0b100000,
	AST_AT_FLOAT = 0b1000000,
	AST_AT_DOUBLE = 0b10000000,
} ASTArithType;

/**
 * @param[out] type
 */
void initASTArithType(ASTArithType *type);
/**
 * @param[out] type
 * @param[in] tok
 * @returns The number of tokens parsed
 */
int parseASTArithType(ASTArithType *type, Token const *tok);
/**
 * @brief Debug print the ASTArithType
 * @param[in] type
 * @returns the number of characters printed
 */
int printASTArithType(ASTArithType const *type);
/**
 * @brief Removes duplicate representations of arithmatic types
 * @param[in] type The arithmatic bitfield to transform
 * @returns The transformed bitfield
 */
ASTArithType astArithTypeNormalize(ASTArithType type);

/***************************************************************************
 * ASTTypeSpec
 ***************************************************************************/

/**
 * @brief Identifies the type specifier
 */
typedef enum {
	AST_TST_UNKNOWN,
	/** #brief The declaration references void */
	AST_TST_VOID,
	/** @brief The declaration referneces a number */
	AST_TST_ARITH,
	/** @brief The declaration references a typedef */
	AST_TST_TYPEDEF,
	/** @brief The declaration introduces or references a struct */
	AST_TST_STRUCT,
	/** @brief The declaration introduces or references an union */
	AST_TST_UNION,
	/** @brief The declaration introduces or references an enum */
	AST_TST_ENUM,
} ASTTypeSpecType;

/**
 * @brief A type specifier
 * @extends ASTNode
 *
 * The type specifier is the first chunk of a declaration.
 */
typedef struct ASTTypeSpec {
	ASTNode node;
	ASTTypeQualifier qualifiers;
	ASTStorageClassSpec storage;
	ASTTypeSpecType typeSpecType;
	ASTArithType arith;
	/** @brief Used for more complicated types like enums, typedef,s structs and unions*/
	ASTNode *content;
	/** @brief The starting token */
	Token const *tok;
} ASTTypeSpec;

/**
 * @param[out] typeSpec
 * @param[in] tok The starting token for the parsed node
 */
void initASTTypeSpec(ASTTypeSpec *typeSpec, Token const *tok);
/** 
 * @param[in] typeSpec
 */
void freeASTTypeSpec(ASTTypeSpec *typeSpec);
/**
 * @param[out] typeSpec
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTTypeSpec(
		ASTTypeSpec *typeSpec,
		Token const *tok,
		ASTScope *scope);
/**
 * @brief Debug print the node
 * @param[in] typeSpec
 * @returns The number of characters printed
 */
int printASTTypeSpec(ASTTypeSpec const * typeSpec);
/**
 * @brief Impliments astNodeChildCount for ASTTypeSpec
 * @param[in] typeSpec
 * @returns The number of child nodes
 */
int astTypeSpecChildCount(ASTTypeSpec const *typeSpec);
/**
 * @brief Impliments astNodeGetChild for ASTTypeSpec
 * @param[in] typeSpec
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astTypeSpecGetChild(ASTTypeSpec *typeSpec, int index);

/* =========================================================================
 * ASTDeclarator
 * ========================================================================= */

/**
 * @brief Part of a declaration associated with identifier
 * @extends ASTNode
 * @note For declarators, the nesting of the declaration is reverse of the 
 * nesting of the type.
 * `*test[5] (pointer { array })` Is actually array of pointers
 */
typedef struct ASTDeclarator {
	ASTNode node;
	/**
	 * @brief Can be AST_POINTER_DECL, AST_ARRAY_DECL, AST_FUNC_DECL, AST_IDENTIFIER_DECL
	 */
	ASTNode *encl;
	/** @brief Used when representing contents of pointer*/
	ASTTypeQualifier qualifiers;
	ASTNode *initializer;
	ASTNode *bitField;
} ASTDeclarator;

/**
 * @param[out] declarator
 * @param[in] tok
 */
void initASTDeclarator(ASTDeclarator *declarator, Token const *tok);
/**
 * @param[in] declarator
 */
void freeASTDeclarator(ASTDeclarator *declarator);
/**
 * @param[out] declarator
 * @param[in] tok
 * @param[in] scope
 * @returns The number of parsed tokens
 */
int parseASTDeclarator(
		ASTDeclarator *declarator,
		Token const *tok,
		ASTScope *scope);
/**
 * @brief Debug print the declarator
 * @param[in] declarator
 * @returns The number of characters printed
 */
int printASTDeclarator(ASTDeclarator const *declarator);
/**
 * @brief Impliments astNodeChildCount for ASTDeclarator
 * @param[in] node
 * @returns The number of child nodes
 */
int astDeclaratorChildCount(ASTDeclarator const *node);
/**
 * @brief Impliments astNodeGetChild for ASTDeclarator
 * @param[in] node
 * @param[in] index
 * @return The corresponding node
 */
ASTNode *astDeclaratorGetChild(ASTDeclarator *node, int index);

/***************************************************************************
 * ASTDeclaration
 ***************************************************************************/

/**
 * @brief Defines collection of identifiers
 * @extends ASTNode
 */
typedef struct ASTDeclaration {
	ASTNode node;
	/** @brief Base type */
	ASTTypeSpec *typeSpec;
	/** @brief List of identifiers */
	DList declarators;
} ASTDeclaration;

/**
 * @param[out] declaration
 * @param[in] tok
 */
void initASTDeclaration(ASTDeclaration *declaration, Token const *tok);
/**
 * @param[in] declaration
 */
void freeASTDeclaration(ASTDeclaration *declaration);
/**
 * @param[out] declaration
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokkens parsed
 */
int parseASTDeclaration(
		ASTDeclaration *declaration,
		Token const *tok,
		ASTScope *scope);
/**
 * @brief Debug print a declaration
 * @param[in] declaration
 * @returns The number of characters printed
 */
int printASTDeclaration(ASTDeclaration const *declaration);
/**
 * @brief Impliments astNodeChildCount for ASTDeclaration
 * @param[in] node
 * @returns The number of child nodes
 */
int astDeclarationChildCount(ASTDeclaration const *node);
/**
 * @brief Impliments astNodeGetChild for ASTDeclaration
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astDeclarationGetChild(ASTDeclaration *node, int index);

/**
 * @brief If the declaration is a typedef, returns the list of typedef names
 * @param[in] declaration
 * @returns The list of found names found
 *
 * The return value is a list of owned strings
 */
DList astDeclarationTypedefNames(ASTDeclaration const *declaration);
/**
 * @brief Searches the declarators children for the identifier name
 * @param[in] declarator
 * @returns The identifier
 *
 * @note Caller needs to free result
 */
const char * astDeclaratorName(ASTNode const *declarator);
