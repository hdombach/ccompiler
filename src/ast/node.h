#pragma once

#if __STDC_VERSION__ >= 201112L
#include <assert.h>
#endif

#include "../util/vtable.h"

/**
 * @file
 * 
 * @brief Tools associated with a generic asbstract syntax tree node
 */

/**
 * @brief A [x macro](https://en.wikipedia.org/wiki/X_macro) for CError
 */
#define X_AST_NODE_TYPE \
	X(AST_UNKNOWN, "unknown") \
	X(AST_INT_CONSTANT, "int constant") \
	X(AST_FLOAT_CONSTANT, "float constant") \
	X(AST_CHAR_CONSTANT, "char constant") \
	X(AST_STR_CONSTANT, "string constant") \
	X(AST_IDENTIFIER, "identifier") \
	X(AST_FUNC_OPERATION, "function call operation") \
	X(AST_SUBS_OPERATION, "subscript operation") \
	X(AST_COND_OPERATION, "conditional operation") \
	X(AST_CAST_OPERATION, "type cast operation") \
	X(AST_SIZEOF_TYPE_OPERATION, "sizeof type operation") \
	X(AST_SIZEOF_EXP_OPERATION, "sizeof expression operation") \
	X(AST_BINARY_OPERATION, "binary operation") \
	X(AST_PREFIX_OPERATION, "prefix operation") \
	X(AST_POSTFIX_OPERATION, "postfix operation") \
	X(AST_PARAM, "param") \
	X(AST_DECLARATION, "declaration") \
	X(AST_ARRAY_DECL, "array declarator") \
	X(AST_FUNC_DECL, "function declarator") \
	X(AST_IDENTIFIER_DECL, "identifier declarator") \
	X(AST_POINTER_DECL, "pointer declarator") \
	X(AST_DECLARATOR, "declarator") \
	X(AST_TYPE_SPEC, "type specifier") \
	X(AST_IDENTIFIER_TS, "typedef type specificer") \
	X(AST_STRUCT_DECL, "struct declaration") \
	X(AST_ENUM_DECL, "enum declaration") \
	X(AST_ENUM_CONST, "enumerator declaration const") \
	X(AST_STM, "statement") \
	X(AST_COMP_STM, "compound statement") \
	X(AST_IF, "if statement") \
	X(AST_SWITCH, "switch statement") \
	X(AST_WHILE, "while statement") \
	X(AST_DO_WHILE, "do while statement") \
	X(AST_EMPTY_STM, "empty statement") \
	X(AST_BREAK, "break statement") \
	X(AST_CONTINUE, "continue statement") \
	X(AST_FUNC_DEF, "function definition") \
	X(AST_FILE, "file") \
	X(AST_INITIALIZER_LIST, "initializer list") \
	X(AST_LBL_IDENTIFIER, "identifier label") \
	X(AST_LBL_CASE, "case label") \
	X(AST_LBL_DEFAULT, "default label") \
	X(AST_FOR, "for statement") \
	X(AST_GOTO, "goto statement")

#define X(NAME, STR) NAME,
/**
 * @brief Identifies the node type
 */
typedef enum ASTNodeType {
	X_AST_NODE_TYPE
} ASTNodeType;
#undef X

struct ASTScope;
struct Token;
typedef struct ASTNode ASTNode;

typedef int (*ASTChildCount)(ASTNode const *node);
typedef ASTNode *(*ASTGetChild)(ASTNode *node, int index);

/**
 * @extends VTable
 */
typedef struct ASTNodeVTable {
	VTable table;
	ASTChildCount childCount;
	ASTGetChild getChild;
} ASTNodeVTable;

/**
 * @brief The generic struct for a node
 *
 * The struct should not be used directly. Instead, cast to specialized
 * structs based on the type value
 */
typedef struct ASTNode {
	/** @brief Identifies the node type */
	ASTNodeType type;
	ASTNodeVTable const *vtable;
	/** @brief The parent node in the tree */
	struct ASTNode *parent;
	/** @brief The scope the node is currently within */
	struct ASTScope *curScope;
	/**
	 * @brief The start of the tokens used to parse nodes
	 *
	 * NOTE: Does no own the tok object
	 */
	struct Token const *tok;
} ASTNode;

/**
 * @param[out] node
 * @param[in] tok
 */
void initASTNode(ASTNode *node, struct Token const *tok, ASTNodeVTable const *vtable);
/**
 * @brief Frees any node that inherits from ASTNode
 * @param[in] node
 */
void freeASTNode(ASTNode *node);
/**
 * @param[in] node
 * @param[in] parent
 */
void astNodeSetParent(ASTNode *node, ASTNode *parent);
/**
 * @brief Moves the contest of src to dest and resets the contents of src
 * @param[out] dest
 * @param[in] src
 */
void mvASTNode(ASTNode *dest, ASTNode *src);
/**
 * @brief Moves the contents of node to a new buffer
 * @param[in] node
 * @returns New buffer containing node
 *
 * Doesn't technically duplicate node. Is like
 * mvASTNode except it allocates the dest buffer for you
 */
ASTNode *dupASTNode(ASTNode *node);
/**
 * @brief Returns a string representation of node type
 * @param[in] type
 * @returns String constant representing type
 */
const char *astNodeTypeStr(ASTNodeType type);
/**
 * @brief Debug print any node
 * @param node
 * @returns Number of characters printed
 */
int printASTNode(ASTNode const *node);
/**
 * @brief Get the child count for any node
 * @param[in] node
 * @returns number of children in tree
 */
int astNodeChildCount(ASTNode const *node);
/**
 * @brief Get a specific child of any node
 * @param[in] node
 * @param[in] index
 * @returns The node at the corresponding index
 */
ASTNode *astNodeGetChild(ASTNode *node, int index);
/**
 * @brief If node contains scope, return it
 * @param[in] node
 * @param[in] defaultScope The scope to return if node doesn't contain one
 * @returns The found scope
 */
struct ASTScope *astNodeScope(ASTNode *node, struct ASTScope *defaultScope);


/** @brief The result of the node traversal */
typedef enum ASTTravRes {
	ASTT_FAILED,
	ASTT_SUCCESS,
} ASTTravRes;

/** @brief The context used internally for AST traversal */
typedef struct ASTTravCtx {
	struct ASTScope *scope;
	ASTNode *node;
	struct ASTTravCtx *parent;
	/**
	 * @brief Syntax that can be used by the callback
	 *
	 * @note Needs to be allocated and freed by the caller
	 */
	void *customCtx;
}ASTTravCtx;

/** @brief Function used in AST traversal*/
typedef ASTTravRes (*ASTTravFunc)(ASTNode *, ASTTravCtx *);

/**
 * @brief Recursively traverses the AST
 * @param node[in]
 * @param node[in] beforeFunc Function to call in pre-order order
 * @param node[in] afterFunc Function to call in post-order order
 * @param node[in] customCtx Contex used by callbacks
 */
ASTTravRes astNodeTrav(
		ASTNode *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		void *customCtx);

/** @brief Size of generic node buffer */
#define AST_NODE_S 128
/**
 * @brief Used to allocated a generic node buffer
 *
 * ASTNode and all the nodes associated with it essentially act as a union.
 * In order to safely cast between nodes, enough memory needs to be allocated
 * for the biggest node.
 */
typedef struct ASTNodeBuf {
	ASTNode node;
	char buf[AST_NODE_S - sizeof(ASTNode)];
} ASTNodeBuf;

#if __STDC_VERSION__ >= 201112L
/**
 * @brief Verifies if enough memory is allocated for every node
 */
#define AST_VALID(Type)\
	static_assert(sizeof(ASTNodeBuf) >= sizeof(Type), "ASTNodeBuf is too small")
#else
#define AST_VALID(Type)
#endif
