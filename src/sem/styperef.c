#include "styperef.h"

#include <stdio.h>

#include "../util/log.h"
#include "../ast/identifier.h"

static STypeVTable _typerefVTable = {
	(FreeFunc) NULL,
	(PrintFunc) printSTypeRef,
};

void initSTypeRef(STypeRef *type) {
	initSType((SType *) type, &_typerefVTable);
	type->index = -1;
	type->parentScope = NULL;
}

int loadSTypeRef(STypeRef *type, ASTIdentifier *identifier, ASTScope *scope) {
	STYPE_VALID(STypeRef);

	if (!ASSERT(identifier->node.type == AST_IDENTIFIER_TS)) return 0;

	initSTypeRef(type);

	if (!astScopeGetIdentifier(type, scope, identifier->name)) return 0;

	type->type.type = STT_TYPEDEF_REF;
	return 1;
}

SType *stypdefDeref(STypeRef *ref) {
	return dlistGetm(&ref->parentScope->identifiers, ref->index);
}

int printSTypeRef(const STypeRef *type) {
	if (!ASSERT(type->type.type == STT_TYPEDEF_REF)) return 0;
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"%s\"", sttStr(type->type.type));

	n += printf(", \"content\": ");
	n += printSType(stypdefDeref((STypeRef *) type));

	n += printf("}");

	return n;
}
