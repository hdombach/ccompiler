#include "scompound.h"

#include <stdio.h>
#include <string.h>

#include "scope.h"
#include "type.h"
#include "../util/log.h"

static STypeVTable _compoundVTable = {
	{
		(FreeFunc) NULL,
		(PrintFunc) printSCompound,
	},
	(STypeDerefFunc) NULL,
	(STypeInternFunc) NULL,
};

void initSCompound(SCompound *type) {
	initSType((SType *) type, &_compoundVTable);
	type->scope = NULL;
	type->isUnion = 0;
}

int printSCompound(const SCompound *ref) {
	if (!ref) {
		return printf("\"null\"");
	}
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

static STypeVTable _compoundRefVTable = {
	{
		(FreeFunc) NULL,
		(PrintFunc) printSCompoundRef,
	},
	(STypeDerefFunc) scompoundDeref,
};

void initSCompoundRef(SCompoundRef *type) {
	initSType((SType *) type, &_compoundRefVTable);
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
	SCompoundRef *buf;

	initSCompoundRef(type);
	if (!loadSCompound(&tempComp, structDecl)) return 0;
	if (structDecl->name) {
		if (!astScopeHasTag(scope, structDecl->name)) {
			if (!astScopeAddCompound(scope, &tempComp, strdup(structDecl->name))) return 0;
		}
		if (!astScopeGetTag((SType *) type, scope, structDecl->name)) {
			INT_ERROR("struct %s wasn't sucessfully added", structDecl->name);
		}
		ASSERT(type->type.type == STT_STRUCT_REF || type->type.type == STT_UNION_REF);
		return 1;
	}

	astScopeAddAnonCompound(type, scope, &tempComp);
	ASSERT(type->type.type == STT_STRUCT_REF || type->type.type == STT_UNION_REF);
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
	if (ref->index > 0) {
		return dlistGetm(&ref->parentScope->tags, ref->index);
	} else {
		return NULL;
	}
}


