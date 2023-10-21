#include <stdlib.h>

#include "node.h"
#include "param.h"
#include "astUtil.h"
#include "declaration.h"
#include "../token.h"
#include "structDecl.h"

void initASTParam(ASTParam *param) {
	param->typeSpec = NULL;
	param->declarator = NULL;
	param->node.type = AST_PARAM;
}

void freeASTParam(ASTParam *param) {
	if (param->typeSpec) {
		freeASTTypeSpec(param->typeSpec);
		free(param->typeSpec);
	}
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

	if ((res = parseASTTypeSpec((ASTTypeSpec *) &tempBuf, tok + n, scope))) {
		n += res;
		param->typeSpec = (ASTTypeSpec *) dupASTNode((ASTNode *) &tempBuf);
	} else {
		freeASTParam(param);
		return 0;
	}

	if ((res = parseASTDeclarator((ASTDeclarator *) &tempBuf, tok + n, scope))) {
		n += res;
		param->declarator = (ASTDeclarator *) dupASTNode((ASTNode *) &tempBuf);
	}

	param->node.type = AST_PARAM;
	return n;
}

int printASTParam(const ASTParam *param) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"param\"");

	if (param->typeSpec->qualifiers) {
		n += printf(", \"Type Qualifiers\": ");
		n += printASTTypeQualifier(&param->typeSpec->qualifiers);
	}

	if (param->typeSpec->storage) {
		n += printf(", \"Storage Class Specifiers\": ");
		n += printASTStorageClassSpec(&param->typeSpec->storage);
	}

	n += printf(", \"type\": ");
	switch (param->typeSpec->typeSpecType) {
		case AST_TST_VOID:
			n += printf("\"void\"");
			break;
		case AST_TST_ARITH:
			n += printASTArithType(&param->typeSpec->arith);
			break;
		default:
			if (param->typeSpec->content) {
				n += printASTNode(param->typeSpec->content);
			} else {
				n += printf("\"unknown\"");
			}
			break;
	}

	if (param->declarator) {
		n += printf(", \"Declarator\": ");
		n += printASTDeclarator(param->declarator);
	}

	n += printf("}");

	return n;

}

ASTTravRes astParamTrav(
		ASTParam *param,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc)
{
	ASTTravRes result;

	result = astNodeTrav((ASTNode *) param->typeSpec, beforeFunc, afterFunc);
	if (result == ASTT_FAILED) return ASTT_FAILED;

	result = astNodeTrav((ASTNode *) param->declarator, beforeFunc, afterFunc);
	if (result == ASTT_FAILED) return ASTT_FAILED;

	return ASTT_SUCCESS;
}
