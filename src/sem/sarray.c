#include "sarray.h"

#include <stdio.h>

#include "../util/log.h"
#include "../ast/declaration.h"
#include "type.h"

static STypeVTable _arrayVTable = {
	{
		(FreeFunc) destroySArray,
		(PrintFunc) printSArray,
	},
	(STypeDerefFunc) NULL,
};


void initSArray(SArray *type) {
	initSType((SType *) type, &_arrayVTable);

	type->elType = NULL;
	type->size = -1;
	type->hasSize = 0;
}

void destroySArray(SArray *type) {
	if (type->elType) {
		destroySType(type->elType);
		free(type->elType);
	}
}

int loadSArray(SArray *type, SType *internal, ASTArrayDecl *arrayDecl, ASTScope *scope) {
	STYPE_VALID(SArray);
	if (!ASSERT(arrayDecl->node.type == AST_ARRAY_DECL)) return 0;

	STypeBuf tempBuf;
	initSArray(type);

	if (arrayDecl->node.type != AST_ARRAY_DECL) {
		return 0;
	}

	if (!loadDecl((SType *) &tempBuf, internal, arrayDecl->encl, scope)) {
		return 0;
	}

	type->elType = movaSType((SType *) &tempBuf);
	type->type.type = STT_ARRAY;
	type->size = 0;
	TODO("Set size");
	return 1;
}

int printSArray(const SArray *type) {
	if (!ASSERT(type->type.type == STT_ARRAY)) return 0;
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"%s\"", sttStr(type->type.type));

	if (type->hasSize) {
		n += printf(", \"size\": %zu", type->size);
	}

	if (type->elType) {
		n += printf(", \"element type\": ");
		n += printSType(type->elType);
	}

	n += printf("}");

	return n;
}


