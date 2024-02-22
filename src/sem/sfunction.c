#include "sfunction.h"

#include <stdio.h>

#include "type.h"
#include "../util/log.h"

static STypeVTable _functionVTable = {
	{
		(FreeFunc) destroySFunction,
		(PrintFunc) printSFunction,
	},
	(STypeDerefFunc) NULL,
	(STypeInternFunc) sfunctionGetIntern
};

void initSFunction(SFunction *type) {
	initSType((SType *) type, &_functionVTable);
	type->returnType = NULL;
	type->paramScope = NULL;
}

void destroySFunction(SFunction *type) {
	if (type->returnType) {
		destroySType(type->returnType);
		free(type->returnType);
	}

	type->paramScope = NULL; //owned by the AST
}

int loadSFunction(
		SType *type,
		SType *internal,
		ASTFuncDecl *declarator,
		ASTScope *scope)
{
	if (!ASSERT(declarator->node.type == AST_FUNC_DECL)) return 0;
	STypeBuf func;
	SFunction *funcPtr = (SFunction *) &func;
	initSFunction(funcPtr);
	funcPtr->returnType = movaSType(internal);
	funcPtr->paramScope = declarator->scope;
	funcPtr->type.type = STT_FUNC;

	if (!loadDecl((SType *) type, (SType *) funcPtr, declarator->encl, scope)) return 0;
	return 1;
}

int printSFunction(const SFunction *func) {
	if (!ASSERT(func->type.type == STT_FUNC)) return 0;
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"%s\"", sttStr(func->type.type));

	if (func->returnType) {
		n += printf(", \"return type\": ");
		n += printSType(func->returnType);
	}

	n += printf(", \"params\": ");
	n += printASTScope(func->paramScope);

	n += printf("}");

	return n;
}

SType *sfunctionGetIntern(SFunction *type) {
	return type->returnType;
}
