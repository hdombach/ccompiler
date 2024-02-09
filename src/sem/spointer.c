#include "spointer.h"

#include <stdio.h>

#include "../ast/declaration.h"
#include "../util/log.h"

static STypeVTable _spointerVTable = {
	{
		(FreeFunc) destroySPointer,
		(PrintFunc) printSPointer
	}
};

SPointer *newSPointer() {
	SPointer *result = malloc(STYPE_S);
	initSPointer(result);
	return result;
}

void initSPointer(SPointer *type) {
	initSType((SType *) type, &_spointerVTable);
	type->internal = NULL;
}

void destroySPointer(SPointer *type) {
	if (type->internal) {
		destroySType(type->internal);
		free(type->internal);
	}
}

int loadSPointer(
		SType *type,
		SType *internal,
		ASTDeclarator *declarator,
		ASTScope *scope)
{
	STypeBuf pointer;
	pointer.type.type = STT_POINTER;
	((SPointer *) &pointer)->internal = movaSType(internal);

	if (!loadDecl(type, (SType *) &pointer, declarator->encl, scope)) goto failure;
	return 1;

failure:
	destroySPointer((SPointer *) &pointer);
	return 0;
}

int printSPointer(const SPointer *type) {
	if (!ASSERT(type->type.type == STT_POINTER)) return 0;
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"%s\"", sttStr(type->type.type));

	if (type->internal) {
		n += printf(", \"content\": ");
		n += printSType(type->internal);
	}

	n += printf("}");
	
	return n;
}


