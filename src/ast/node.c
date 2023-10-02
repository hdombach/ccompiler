#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"
#include "identifier.h"
#include "intConstant.h"
#include "node.h"
#include "operation.h"
#include "param.h"

void initASTNode(ASTNode *node) {
	node->type = AST_UNKNOWN;
}

void freeASTNode(ASTNode *node) {
	switch (node->type) {
		case AST_IDENTIFIER: free(((ASTIdentifier *) node)->name); break;

		case AST_FUNC_OPERATION: freeASTFuncOperation((ASTFuncOperation *) node); break;
		case AST_COND_OPERATION: freeASTCondOperation((ASTCondOperation *) node); break;
		case AST_SUBS_OPERATION:
		case AST_CAST_OPERATION:
		case AST_SIZEOF_EXP_OPERATION:
		case AST_SIZEOF_TYPE_OPERATION:
		case AST_BINARY_OPERATION:
		case AST_PREFIX_OPERATION:
		case AST_POSTFIX_OPERATION: freeASTOperation((ASTOperation *)node); break;
		case AST_PARAM: freeASTParam((ASTParam *) node); break;
		default: break;
	}
	node->type = AST_UNKNOWN;
}

void mvASTNode(ASTNode *dest, ASTNode *src) {
	memcpy(dest, src, AST_NODE_S);
	src->type = AST_UNKNOWN;
}

char *_astNodeTypes[] = {
	"unknown",
	"int constant",
	"identifier",

	"function call operation",
	"subscript operation",
	"conditional operation",
	"type case operation",
	"sizeof type operation",
	"sizeof expression operation",
	"binary operation",
	"prefix operation",
	"postfix operation",

	"param",
};

char *astNodeTypeStr(ASTNodeType type) {
	return _astNodeTypes[type];
}



int printASTNode(ASTNode *node) {
	switch (node->type) {
		case AST_INT_CONSTANT: return printASTIntContant((ASTIntConstant *) node);
		case AST_IDENTIFIER: return printASTIdentifier((ASTIdentifier *) node);

		case AST_FUNC_OPERATION: return printASTFuncOperation((ASTFuncOperation *) node);
		case AST_COND_OPERATION: return printASTCondOperation((ASTCondOperation *) node);
		case AST_SUBS_OPERATION:
		case AST_CAST_OPERATION:
		case AST_SIZEOF_EXP_OPERATION:
		case AST_SIZEOF_TYPE_OPERATION:
		case AST_BINARY_OPERATION:
		case AST_PREFIX_OPERATION:
		case AST_POSTFIX_OPERATION: return printASTOperation((ASTOperation *) node);
		case AST_PARAM: return printASTParam((ASTParam *) node);
		default: return printf("\"(null)\"");
	}
}
