#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
			type->c.arith = spec->c.arith;
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
		default:
			break;
	}

	return 1;
}

ASTType *astTypeComp(ASTType *lhs, ASTType *rhs) {
	if (lhs->type != rhs->type) {
		return NULL;
	}
	switch (lhs->type) {
		case AST_TT_ARITH:
			return NULL;
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

ASTType **getASTTypes(ASTDeclaration const *declaration) {
	ASTType **result = malloc(sizeof(ASTType *) * (declaration->declarators.size + 1));
	int i = 0;


	ASTDeclarator const *decl = dlistGet(&declaration->declarators, 0);
	ASTDeclarator const *end = decl + declaration->declarators.size;
	for (;decl < end; decl++) {
		ASTType *tempType = malloc(sizeof(ASTType));
		tempType->qualifiers = declaration->typeSpec.qualifiers;
		tempType->storage = declaration->typeSpec.storage;

		switch (declaration->typeSpec.typeSpecType) {
			case AST_TST_ARITH:
				tempType->c.arith = declaration->typeSpec.c.arith;
				break;
			default:
				break;
		}

		switch (decl->type) {
			case AST_DT_IDENTIFIER:
				tempType->name = strdup(decl->c.identifier);
				break;
			default:
				break;
		}
		result[i] = tempType;
		i++;
	}
	result[i] = NULL;

	return result;
}
