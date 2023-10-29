#include "typeGen.h"
#include "../ast/node.h"
#include "scope.h"

static ASTTravRes addLabels(ASTNode *node, ASTTravCtx *ctx) {
	if (node->type == AST_STM) {
		ASTStm *stm = (ASTStm *) node;
		if (stm->label) {
			astScopeAddLabel(ctx->scope, stm);
		}
	}
	return ASTT_SUCCESS;
}

void typeGen(ASTFile *file) {
	astNodeTrav((ASTNode *) file, NULL, (ASTTravFunc) addLabels, NULL);
}
