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

	AST_EXP, /* temp */
	AST_PARAM, /* temp */
} ASTNodeType;

typedef struct ASTNode {
	ASTNodeType type;
} ASTNode;

void initASTNode(ASTNode *node);
void freeASTNode(ASTNode *node);
void mvASTNode(ASTNode *dest, ASTNode *src);
char *astNodeTypeStr(ASTNodeType type);
int printASTNode(ASTNode *node);


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
