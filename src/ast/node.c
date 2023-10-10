#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arrayDecl.h"
#include "declaration.h"
#include "enumDecl.h"
#include "expression.h"
#include "funcDecl.h"
#include "identifier.h"
#include "intConstant.h"
#include "node.h"
#include "operation.h"
#include "param.h"
#include "structDecl.h"

void initASTNode(ASTNode *node) {
	node->type = AST_UNKNOWN;
}

void freeASTNode(ASTNode *node) {
	switch (node->type) {
		case AST_IDENTIFIER_DECL:
		case AST_IDENTIFIER_TS:
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
		case AST_DECLARATION: freeASTDeclaration((ASTDeclaration *) node); break;
		case AST_ARRAY_DECL: freeASTArrayDecl((ASTArrayDecl *) node); break;
		case AST_FUNC_DECL: freeASTFuncDecl((ASTFuncDecl *) node); break;
		case AST_POINTER_DECL:
		case AST_DECLARATOR: freeASTDeclarator((ASTDeclarator *) node); break;
		case AST_TYPE_SPEC: freeASTTypeSpec((ASTTypeSpec *) node); break;
		case AST_STRUCT_DECL: freeASTStructDecl((ASTStructDecl *) node); break;
		case AST_ENUM_DECL: freeASTEnumDecl((ASTEnumDecl *) node); break;
		default: break;
	}
	node->type = AST_UNKNOWN;
}

void mvASTNode(ASTNode *dest, ASTNode *src) {
	memcpy(dest, src, AST_NODE_S);
	src->type = AST_UNKNOWN;
}

ASTNode *dupASTNode(ASTNode *node) {
	ASTNode *result = malloc(AST_NODE_S);
	mvASTNode(result, node);
	return result;
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
	"declaration",
	"array declarator",
	"function declarator",
	"identifier declarator",
	"pointer declarator",
	"declarator",
	"type specifier",
	"typedef name type specifier",
	"struct declaration",
	"enum declaration",
};

char *astNodeTypeStr(ASTNodeType type) {
	return _astNodeTypes[type];
}



int printASTNode(ASTNode const *node) {
	switch (node->type) {
		case AST_INT_CONSTANT: return printASTIntContant((ASTIntConstant *) node);
		case AST_IDENTIFIER_DECL:
		case AST_IDENTIFIER_TS:
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
		case AST_DECLARATION: return printASTDeclaration((ASTDeclaration *) node);
		case AST_ARRAY_DECL: return printASTArrayDecl((ASTArrayDecl *) node);
		case AST_FUNC_DECL: return printASTFuncDecl((ASTFuncDecl *) node);

		case AST_POINTER_DECL:
		case AST_DECLARATOR: return printASTDeclarator((ASTDeclarator *) node);
		case AST_TYPE_SPEC: return printASTTypeSpec((ASTTypeSpec *) node);
		case AST_STRUCT_DECL: return printASTStructDecl((ASTStructDecl *) node);
		case AST_ENUM_DECL: return printASTEnumDecl((ASTEnumDecl *) node);
		default: return printf("\"(null)\"");
	}
}
