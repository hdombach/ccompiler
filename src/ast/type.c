#include <stdlib.h>
#include <string.h>

#include "type.h"
#include "declaration.h"

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
