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
	/* typedef reference used in type spec */
	AST_IDENTIFIER_TS,
	AST_STRUCT_DECL,
} ASTNodeType;

typedef struct ASTNode {
	ASTNodeType type;
} ASTNode;

void initASTNode(ASTNode *node);
void freeASTNode(ASTNode *node);
void mvASTNode(ASTNode *dest, ASTNode *src);
ASTNode *dupASTNode(ASTNode *node);
char *astNodeTypeStr(ASTNodeType type);
int printASTNode(ASTNode const *node);


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
