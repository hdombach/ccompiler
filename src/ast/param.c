#include "param.h"
#include "astUtil.h"
#include "declaration.h"
#include "../token.h"
#include "structDecl.h"
#include <stdlib.h>

void initASTParam(ASTParam *param) {
	initASTTypeSpec(&param->typeSpec);
	param->declarator = NULL;
}

void freeASTParam(ASTParam *param) {
	freeASTTypeSpec(&param->typeSpec);
	if (param->declarator) {
		freeASTDeclarator(param->declarator);
		free(param->declarator);
	}
}

int parseASTParam(ASTParam *param, Token const *tok, ASTScope const *scope) {
	int n = 0, res;
	ASTDeclarator tempDeclarator;

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

	if ((res = parseASTDeclarator(&tempDeclarator, tok + n, scope))) {
		n += res;
		param->declarator = malloc(sizeof(ASTDeclarator));
		*param->declarator = tempDeclarator;
	}

	return n;
}

int printASTParam(const ASTParam *param) {
	int n = 0, isFirst = 1;

	n += printf("{");

	if (param->typeSpec.qualifiers) {
		isFirst = 0;
		n += printf("\"Type Qualifiers\": ");
		n += printASTTypeQualifier(&param->typeSpec.qualifiers);
	}

	if (param->typeSpec.storage) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"Storage Class Specifiers\": ");
		n += printASTStorageClassSpec(&param->typeSpec.storage);
	}

	if (isFirst) {
		isFirst = 0;
	} else {
		n += printf(", ");
	}
	n += printf("\"type\": ");
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