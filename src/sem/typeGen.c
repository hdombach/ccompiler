#include <stdio.h>

#include "type.h"
#include "typeGen.h"
#include "scope.h"
#include "../util/log.h"
#include "../ast/node.h"

static ASTTravRes addLabels(ASTNode *node, ASTTravCtx *ctx) {
	TODO("Call astScopeAddLabels");
	return ASTT_SUCCESS;
}

static ASTTravRes checkLabels(ASTNode *node, ASTTravCtx *ctx) {
	if (node->type == AST_GOTO) {
		ASTGoto *stm = (ASTGoto *) node;
		if (!astScopeGetLabel(ctx->scope, stm->name)) {
			logCerr(CERR_UNKNOWN, node->tok, "No corresponding label \"%s\"", stm->name);
		}
	}
	return ASTT_SUCCESS;
}

static int checkIdentifier(ASTIdentifier *identifier, ASTScope *scope) {
	STypeRef ref;
	if (!astScopeGetIdentifier(&ref, scope, identifier->name)) {
		logCerr(CERR_UNKNOWN, identifier->node.tok, "Identifier %s is not defined.", identifier->name);
		return 0;
	}
	return 1;
}

static ASTTravRes resolveTypes(ASTNode *node, ASTTravCtx *ctx) {
	if (node->type == AST_DECLARATION) {
		ASTDeclaration *declaration = (ASTDeclaration *) node;
		loadSTypes(ctx->scope, declaration);
		//printASTScope(ctx->scope);
	} else if (node->type == AST_ENUM_CONST) {
		ASTEnumConst *decl = (ASTEnumConst *) node;
		SPrim primType;
		loadSEnumConst(&primType, decl, ctx->scope);
		astScopeAddIdent(ctx->scope, (SType *) &primType, strdup(decl->name));
	} else if (node->type == AST_PARAM) {
		ASTParam *param = (ASTParam *) node;
		loadParamSType(ctx->scope, param);
	} else if (node->type == AST_IDENTIFIER) {
		checkIdentifier((ASTIdentifier *) node, ctx->scope);
	}

	return ASTT_SUCCESS;
}

static ASTTravRes printScopes(ASTNode *node, ASTTravCtx *ctx) {
	ASTScope *scope = astNodeScope(node, NULL);
	if (scope) {
		printASTScope(scope);
	}
	return ASTT_SUCCESS;
}

void typeGen(ASTFile *file) {
	astNodeTrav((ASTNode *) file, NULL, (ASTTravFunc) addLabels, NULL);
	astNodeTrav((ASTNode *) file, NULL, (ASTTravFunc) checkLabels, NULL);
	astNodeTrav((ASTNode *) file, NULL, (ASTTravFunc) resolveTypes, NULL);

	astNodeTrav((ASTNode *) file, NULL, (ASTTravFunc) printScopes, NULL);
}
