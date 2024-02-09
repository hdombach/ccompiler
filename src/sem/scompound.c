#include "scompound.h"

#include <stdio.h>
#include <string.h>

#include "type.h"
#include "../util/log.h"

static STypeVTable _compoundVTable = {
	{
		(FreeFunc) NULL,
		(PrintFunc) printSCompound,
	}
};

void initSCompound(SCompound *type) {
	initSType((SType *) type, &_compoundVTable);
	type->scope = NULL;
	type->isUnion = 0;
}

int printSCompound(const SCompound *ref) {
	if (!ASSERT(ref->type.type == STT_STRUCT || ref->type.type == STT_UNION))
		return 0;
	return printASTScope(ref->scope);
}

int loadSCompound(SCompound *type, ASTStructDecl *decl) {
	if (!ASSERT(decl->node.type == AST_STRUCT_DECL)) return 0;
	initSCompound(type);
	type->isUnion = decl->isUnion;
	type->scope = decl->scope;
	if (type->isUnion) {
		type->type.type = STT_UNION;
	} else {
		type->type.type = STT_STRUCT;
	}
	return 1;
}

void initSCompoundRef(SCompoundRef *type) {
	type->index = -1;
	type->parentScope = NULL;
}

int loadSCompoundRef(
		SCompoundRef *type,
		ASTStructDecl *structDecl,
		ASTScope *scope)
{
	STYPE_VALID(SCompoundRef);
	if (!ASSERT(structDecl->node.type == AST_STRUCT_DECL)) return 0;

	SCompound tempComp;

	initSCompoundRef(type);
	if (structDecl->name) {
		if (!astScopeHasCompound(scope, structDecl->name)) {
			if (!loadSCompound(&tempComp, structDecl)) return 0;
			if (!astScopeAddCompound(scope, &tempComp, strdup(structDecl->name))) return 0;
		}
		*type = astScopeGetCompound(scope, structDecl->name);
		return 1;
	}

	if (!loadSCompound(&tempComp, structDecl)) return 0;
	*type = astScopeAddAnonCompound(scope, &tempComp);
	return 1;
}

int printSCompoundRef(const SCompoundRef *ref) {
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"Compound Ref\"");

	n += printf(", \"content\": ");
	n += printSCompound(scompoundDeref((SCompoundRef*) ref));

	n += printf("}");

	return n;
}

SCompound *scompoundDeref(struct SCompoundRef *ref) {
	return dlistGetm(&ref->parentScope->tags, ref->index);
}


