#pragma once

#if __STDC_VERSION__ >= 201112L
#include <assert.h>
#endif

/**
 * @file
 * 
 * @brief Tools associated with a generic asbstract syntax tree node
 */

/**
 * @brief Identifies the node type
 */
typedef enum ASTNodeType {
	AST_UNKNOWN,
	AST_INT_CONSTANT,
	AST_FLOAT_CONSTANT,
	AST_IDENTIFIER,
	AST_FUNC_OPERATION,
	AST_SUBS_OPERATION,
	AST_COND_OPERATION,
	AST_CAST_OPERATION,
	AST_SIZEOF_TYPE_OPERATION,
	AST_SIZEOF_EXP_OPERATION,
	AST_BINARY_OPERATION,
	AST_PREFIX_OPERATION,
	AST_POSTFIX_OPERATION,
	AST_PARAM, /* temp */
	AST_DECLARATION,
	AST_ARRAY_DECL,
	AST_FUNC_DECL,
	AST_IDENTIFIER_DECL,
	AST_POINTER_DECL,
	AST_DECLARATOR,
	AST_TYPE_SPEC, 
	AST_IDENTIFIER_TS, /* typedef reference used in type spec */
	AST_STRUCT_DECL,
	AST_ENUM_DECL,
	AST_ENUM_CONST,
	AST_STM,
	AST_COMP_STM,
	AST_IF,
	AST_SWITCH,
	AST_WHILE,
	AST_DO_WHILE,
	AST_EMPTY_STM,
	AST_BREAK,
	AST_CONTINUE,
	AST_FUNC_DEF,
	AST_FILE,
	AST_INITIALIZER_LIST,
	AST_LBL_IDENTIFIER,
	AST_LBL_CASE,
	AST_LBL_DEFAULT,
	AST_FOR,
	AST_GOTO,
} ASTNodeType;

struct ASTScope;
struct Token;

/**
 * @brief The generic struct for a node
 *
 * The struct should not be used directly. Instead, cast to specialized
 * structs based on the type value
 */
typedef struct ASTNode {
	/** @brief Identifies the node type */
	ASTNodeType type;
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
void initASTNode(ASTNode *node, struct Token const *tok);
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
