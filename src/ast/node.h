#pragma once

#if __STDC_VERSION__ >= 201112L
#include <assert.h>
#endif

typedef enum ASTNodeType {
	AST_UNKNOWN,
	AST_INT_CONSTANT,
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
	AST_ENUMERATOR_DECL,
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

typedef struct ASTNode {
	ASTNodeType type;
	struct ASTNode *parent;
	struct ASTScope *curScope;
	struct Token const *tok;
} ASTNode;

void initASTNode(ASTNode *node, struct Token const *tok);
void freeASTNode(ASTNode *node);
void astNodeSetParent(ASTNode *node, ASTNode *parent);
void mvASTNode(ASTNode *dest, ASTNode *src);
ASTNode *dupASTNode(ASTNode *node);
char *astNodeTypeStr(ASTNodeType type);
int printASTNode(ASTNode const *node);
int astNodeChildCount(ASTNode const *node);
ASTNode *astNodeGetChild(ASTNode *node, int index);
struct ASTScope *astNodeScope(ASTNode *node, struct ASTScope *defaultScope);


/* The result of node traversal */
typedef enum ASTTravRes {
	ASTT_FAILED,
	ASTT_SUCCESS,
} ASTTravRes;

typedef struct ASTTravCtx {
	struct ASTScope *scope;
	ASTNode *node;
	struct ASTTravCtx *parent;
}ASTTravCtx;

typedef ASTTravRes (*ASTTravFunc)(ASTNode *, ASTTravCtx *);

ASTTravRes astNodeTrav(
		ASTNode *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *parent);


#define AST_NODE_S 128
typedef struct ASTNodeBuf {
	ASTNode node;
	char buf[AST_NODE_S - sizeof(ASTNode)];
} ASTNodeBuf;

#if __STDC_VERSION__ >= 201112L
#define AST_VALID(Type)\
	static_assert(sizeof(ASTNodeBuf) >= sizeof(Type), "ASTNodeBuf is too small")
#else
#define AST_VALID(Type)
#endif
