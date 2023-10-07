#include <stdlib.h>

#include "node.h"
#include "param.h"
#include "astUtil.h"
#include "declaration.h"
#include "../token.h"
#include "structDecl.h"

void initASTParam(ASTParam *param) {
	initASTTypeSpec(&param->typeSpec);
	param->declarator = NULL;
	param->node.type = AST_PARAM;
}

void freeASTParam(ASTParam *param) {
	freeASTTypeSpec(&param->typeSpec);
	if (param->declarator) {
		freeASTDeclarator(param->declarator);
		free(param->declarator);
	}
}

int parseASTParam(ASTParam *param, Token const *tok, ASTScope const *scope) {
	AST_VALID(ASTParam);
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTParam(param);

	if (astHasErr()) {
		freeASTParam(param);
		return 0;
	}

	if ((res = parseASTTypeSpec(&param->typeSpec, tok + n, scope))) {
		n += res;
	} else {
		freeASTParam(param);
		return 0;
	}

	if ((res = parseASTDeclarator((ASTDeclarator *) &tempBuf, tok + n, scope))) {
		n += res;
		param->declarator = malloc(AST_NODE_S);
		mvASTNode((ASTNode *) param->declarator, (ASTNode *) &tempBuf);
	}

	return n;
}

int printASTParam(const ASTParam *param) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"param\"");

	if (param->typeSpec.qualifiers) {
		n += printf(", \"Type Qualifiers\": ");
		n += printASTTypeQualifier(&param->typeSpec.qualifiers);
	}

	if (param->typeSpec.storage) {
		n += printf(", \"Storage Class Specifiers\": ");
		n += printASTStorageClassSpec(&param->typeSpec.storage);
	}

	n += printf(", \"type\": ");
	switch (param->typeSpec.typeSpecType) {
		case AST_TST_VOID:
			n += printf("\"void\"");
			break;
		case AST_TST_ARITH:
			n += printASTArithType(&param->typeSpec.c.arith);
			break;
		case AST_TST_TYPEDEF:
			n += printf("\"%s\"", param->typeSpec.c.typedefName);
			break;
		case AST_TST_STRUCT:
			n += printASTStructDecl(&param->typeSpec.c.structDecl);
			break;
		case AST_TST_ENUM:
			n += printASTEnumDecl(&param->typeSpec.c.enumDecl);
			break;
		default:
			n += printf("\"unknown\"");
	}

	if (param->declarator) {
		n += printf(", \"Declarator\": ");
		n += printASTDeclarator(param->declarator);
	}

	n += printf("}");

	return n;

}
