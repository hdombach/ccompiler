#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structDecl.h"
#include "type.h"
#include "declaration.h"
#include "../util/dstr.h"

void initASTType(ASTType *type) {
	type->qualifiers = AST_TQ_NONE;
	type->storage = AST_SC_NONE;
	type->name = NULL;
	type->type = AST_TT_UNKNOWN;
	type->tok = NULL;
}

void initASTTypePart(
		ASTType *type,
		const ASTTypeSpec *spec,
		const ASTDeclarator *declarator)
{
	type->tok = spec->tok;
	if (spec->storage & AST_SC_TYPEDEF) {
		ASTTypeSpec newSpec;
		type->qualifiers = AST_TQ_NONE;
		type->storage = AST_SC_NONE;
		type->type = AST_TT_TYPEDEF;
		type->c.tdef = malloc(sizeof(ASTType));

		cpASTTypeSpec(&newSpec, spec);
		newSpec.storage &= ~AST_SC_TYPEDEF;

		initASTTypePart(type->c.tdef, &newSpec, declarator);
		freeASTTypeSpec(&newSpec);

		if (type->c.tdef->name) {
			type->name = strdup(type->c.tdef->name);
		} else {
			type->name = NULL;
		}

		return;
	}
	//TODO: definitely update this function
	type->qualifiers = spec->qualifiers;
	type->storage = spec->storage;
	switch (declarator->type) {
		case AST_DT_IDENTIFIER:
			type->name = strdup(declarator->c.identifier);
			break;
		default:
			type->name = NULL;
			break;
	}
	switch (spec->typeSpecType) {
		case AST_TST_ARITH:
			type->type = AST_TT_ARITH;
			type->c.arith = astArithTypeNormalize(&spec->c.arith);
			break;
		case AST_TST_STRUCT:
			type->type = AST_TT_STRUCT;
			cpASTStructDecl(&type->c.structDecl, &spec->c.structDecl);
			break;
		case AST_TST_TYPEDEF:
			type->c.tdefRef = strdup(spec->c.typedefName);
			type->type = AST_TT_TYPEDEFREF;
			break;
		default:
			type->type = AST_TT_UNKNOWN;
			break;
	}
}

void freeASTType(ASTType *type) {
	if (type->name) {
		free(type->name);
	}
	switch (type->type) {
		case AST_TT_TYPEDEF:
			freeASTType(type->c.tdef);
			free(type->c.tdef);
			break;
		case AST_TT_STRUCT:
			freeASTStructDecl(&type->c.structDecl);
			break;
		case AST_TT_TYPEDEFREF:
			free(type->c.tdefRef);
			break;
		default:
			break;
	}
}

int printASTType(ASTType *type) {
	int n = 0;

	n += printf("{");

	if (type->type == AST_TT_TYPEDEF) {
		n += printf("\"typedef\": ");
		n += printASTType(type->c.tdef);
	} else {

		n += printf("\"qualifiers\": ");
		n += printASTTypeQualifier(&type->qualifiers);

		n += printf(", \"storage\": ");
		n += printASTStorageClassSpec(&type->storage);

		switch (type->type) {
			case AST_TT_ARITH:
				n += printf(", \"type\": ");
				n += printASTArithType(&type->c.arith);
				break;
			case AST_TT_TYPEDEFREF:
				n += printf(", \"typedef\": ");
				n += printJsonStr(type->c.tdefRef);
				break;
			case AST_TT_STRUCT:
				n += printf(", \"struct\": ");
				n += printASTStructDecl(&type->c.structDecl);
				break;
			default:
				break;
		}

		if (type->name) {
			n += printf(", \"name\": ");
			n += printJsonStr(type->name);
		}

	}

	n += printf("}");

	return n;
}

int cmpASTType(const ASTType *lhs, const ASTType *rhs) {
	if (lhs->qualifiers != rhs->qualifiers) {
		return 0;
	}
	if (lhs->storage != rhs->storage) {
		return 0;
	}

	if (!lhs->name != !rhs->name) {
		return 0;
	}
	if (lhs->name) {
		if (0 != strcmp(lhs->name, rhs->name)) {
			return 0;
		}
	}

	if (lhs->type != rhs->type) {
		return 0;
	}

	switch (lhs->type) {
		case AST_TT_ARITH:
			if (lhs->c.arith != rhs->c.arith) {
				return 0;
			}
			break;
		case AST_TT_TYPEDEF:
			if (!cmpASTType(lhs->c.tdef, rhs->c.tdef)) {
				return 0;
			}
			break;
		case AST_TT_TYPEDEFREF:
			if (0 != strcmp(lhs->c.tdefRef, rhs->c.tdefRef)) {
				return 0;
			}
			break;
		default:
			break;
	}

	//TODO: impliment struct

	return 1;
}

ASTType *astTypeComp(ASTType *lhs, ASTType *rhs) {
	if (lhs->type != rhs->type) {
		return NULL;
	}
	switch (lhs->type) {
		case AST_TT_TYPEDEF:
			if (cmpASTType(lhs, rhs)) {
				return lhs;
			} else {
				return NULL;
			}
		default:
			return NULL;
	}
}

