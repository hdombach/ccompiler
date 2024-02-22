#include "stypedef.h"

#include <stdio.h>

#include "../util/log.h"
#include "../ast/identifier.h"

static STypeVTable _typerefVTable = {
	{
		(FreeFunc) NULL,
		(PrintFunc) printSTypedef,
	},
	(STypeDerefFunc) stypedefDeref,
};

void initSTypedef(STypedef *type) {
	initSType((SType *) type, &_typerefVTable);
	type->index = -1;
	type->parentScope = NULL;
}

int loadSTypedef(STypedef *type, ASTIdentifier *identifier, ASTScope *scope) {
	STYPE_VALID(STypedef);

	if (!ASSERT(identifier->node.type == AST_IDENTIFIER_TS)) return 0;

	initSTypedef(type);

	const int *index = wordDictGet(&scope->identifierDict, identifier->name);
	if (!index) {
		logCerr(CERR_IDENTIFIER, identifier->node.tok, "Unknown typedef identifier \"%s\"", identifier->name);
	}

	type->parentScope = scope;
	type->index = *index;

	type->type.type = STT_TYPEDEF_REF;
	return 1;
}

SType *stypedefDeref(STypedef *ref) {
	return dlistGetm(&ref->parentScope->identifiers, ref->index);
}

int printSTypedef(const STypedef *type) {
	if (!ASSERT(type->type.type == STT_TYPEDEF_REF)) return 0;
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"%s\"", sttStr(type->type.type));

	n += printf(", \"content\": ");
	n += printSType(stypedefDeref((STypedef *) type));

	n += printf("}");

	return n;
}
