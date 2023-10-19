#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arrayDecl.h"
#include "compStatement.h"
#include "declaration.h"
#include "doWhile.h"
#include "enumDecl.h"
#include "expression.h"
#include "file.h"
#include "for.h"
#include "funcDecl.h"
#include "funcDef.h"
#include "identifier.h"
#include "if.h"
#include "initializer.h"
#include "intConstant.h"
#include "label.h"
#include "node.h"
#include "operation.h"
#include "param.h"
#include "statement.h"
#include "structDecl.h"
#include "switch.h"
#include "while.h"

void initASTNode(ASTNode *node, struct Token const *tok) {
	node->type = AST_UNKNOWN;
	node->parent = NULL;
	node->curScope = NULL;
	node->tok = tok;
}

void freeASTNode(ASTNode *node) {
	switch (node->type) {
		case AST_INT_CONSTANT: break;
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
		case AST_ENUMERATOR_DECL: freeASTEnumeratorDecl((ASTEnumeratorDecl *) node); break;
		case AST_STM: freeASTStm((ASTStm *) node); break;
		case AST_COMP_STM: freeASTCompStm((ASTCompStm *) node); break;
		case AST_IF: freeASTIf((ASTIf *) node); break;
		case AST_SWITCH: freeASTSwitch((ASTSwitch *) node); break;
		case AST_WHILE: freeASTWhile((ASTWhile *) node); break;
		case AST_DO_WHILE: freeASTDoWhile((ASTDoWhile *) node); break;
		case AST_EMPTY_STM:
		case AST_BREAK: 
		case AST_CONTINUE: break;
		case AST_FUNC_DEF: freeASTFuncDef((ASTFuncDef *) node); break;
		case AST_FILE: freeASTFile((ASTFile *) node); break;
		case AST_INITIALIZER_LIST: freeASTInitializerList((ASTInitializerList *) node); break;
		case AST_LBL_IDENTIFIER: freeASTLblIdentifier((ASTLblIdentifier *) node); break;
		case AST_LBL_CASE: freeASTLblCase((ASTLblCase *) node); break;
		case AST_LBL_DEFAULT: break;
		case AST_FOR: freeASTFor((ASTFor *) node); break;
		default: break;
	}
	node->type = AST_UNKNOWN;
}

void astNodeSetParent(ASTNode *node, ASTNode *parent) {
	node->parent = node->parent;
	node->curScope = parent->curScope;
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
	"enumerator declaration",
	"statement",
	"compound statement",
	"if statement",
	"switch statement",
	"while statement",
	"do while statement",
	"empty statement",
	"break statement",
	"continue statement",
	"function definition",
	"file",
	"initializer list",
	"identifier label",
	"case label",
	"default label",
	"for statement",
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
		case AST_ENUMERATOR_DECL: return printASTEnumeratorDecl((ASTEnumeratorDecl *) node);
		case AST_STM: return printASTStm((ASTStm *) node);
		case AST_COMP_STM: return printASTCompStm((ASTCompStm *) node);
		case AST_IF: return printASTIf((ASTIf *) node);
		case AST_SWITCH: return printASTSwitch((ASTSwitch *) node);
		case AST_WHILE: return printASTWhile((ASTWhile *) node);
		case AST_DO_WHILE: return printASTDoWhile((ASTDoWhile *) node);
		case AST_EMPTY_STM: return printASTEmptyStm((ASTEmptyStm *) node);
		case AST_BREAK: return printASTBreak((ASTBreak *) node);
		case AST_CONTINUE: return printASTContinue((ASTContinue *) node);
		case AST_FUNC_DEF: return printASTFuncDef((ASTFuncDef *) node);
		case AST_FILE: return printASTFile((ASTFile *) node);
		case AST_INITIALIZER_LIST: return printASTInitializerList((ASTInitializerList *) node);
		case AST_LBL_IDENTIFIER: return printASTLblIdentifier((ASTLblIdentifier *) node);
		case AST_LBL_CASE: return printASTLblCase((ASTLblCase *) node);
		case AST_LBL_DEFAULT: return printASTLblDefault(node);
		case AST_FOR: return printASTFor((ASTFor *) node);
		default: return printf("\"(unknown node)\"");
	}
}
