#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arrayDecl.h"
#include "astUtil.h"
#include "declaration.h"
#include "enumDecl.h"
#include "expression.h"
#include "funcDecl.h"
#include "initializer.h"
#include "structDecl.h"
#include "scope.h"

void initASTTypeQualifier(ASTTypeQualifier *qualifiers) {
	*qualifiers = AST_TQ_NONE;
}

//TODO: implimenet warning when there are duplicate things

int parseASTTypeQualifier(ASTTypeQualifier *qualifiers, const Token *tok) {
	if (astHasErr()) {
		return 0;
	}

	switch (tok->type) {
		case TT_CONST:
			*qualifiers |= AST_TQ_CONST;
			return 1;
		case TT_VOLATILE:
			*qualifiers |= AST_TQ_VOLATILE;
			return 1;
		default:
			return 0;
	}
}

int printASTTypeQualifier(const ASTTypeQualifier *qualifiers) {
	char isFirst = 1;
	int n = 0;
	n += printf("[");

	if (*qualifiers & AST_TQ_CONST) {
		n += printf("\"const\"");
		isFirst = 0;
	}

	if (*qualifiers & AST_TQ_VOLATILE) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"volatile\"");
		isFirst = 0;
	}

	n += printf("]");

	return n;
}

void initASTStorageClassSpec(ASTStorageClassSpec *specs) {
	*specs = AST_SC_NONE;
}

int parseASTStorageClassSpec(ASTStorageClassSpec *specs, const Token *tok) {
	if (astHasErr()) {
		return 0;
	}

	switch (tok->type) {
		case TT_TYPEDEF:
			*specs |= AST_SC_TYPEDEF;
			return 1;
		case TT_AUTO:
			*specs |= AST_SC_AUTO;
			return 1;
		case TT_REGISTER:
			*specs |= AST_SC_REGISTER;
			return 1;
		case TT_STATIC:
			*specs |= AST_SC_STATIC;
			return 1;
		case TT_EXTERN:
			*specs |= AST_SC_EXTERN;
			return 1;
		default:
			return 0;
	}
}

int printASTStorageClassSpec(const ASTStorageClassSpec *specs) {
	char isFirst = 1;
	int n = 0;
	n += printf("[");

	if (*specs & AST_SC_TYPEDEF) {
		n += printf("\"typedef\"");
		isFirst = 0;
	}

	if (*specs & AST_SC_AUTO) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"auto\"");
		isFirst = 0;
	}

	if (*specs & AST_SC_REGISTER) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"register\"");
	}

	if (*specs & AST_SC_STATIC) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"static\"");
	}

	if (*specs & AST_SC_EXTERN) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"extern\"");
	}

	n += printf("]");

	return n;
}

void initASTArithType(ASTArithType *type) {
	*type = AST_AT_NONE;
}

char _isArith(TokenType tokType) {
	switch (tokType) {
		case TT_CHAR:
		case TT_INT:
		case TT_SIGNED:
		case TT_UNSIGNED:
		case TT_SHORT:
		case TT_LONG:
		case TT_FLOAT:
		case TT_DOUBLE:
			return 1;
		default:
			return 0;
	}
}

int parseASTArithType(ASTArithType *type, const Token *tok) {
	if (astHasErr()) {
		return 0;
	}

	switch (tok->type) {
		case TT_CHAR:
			*type |= AST_AT_CHAR;
			return 1;
		case TT_INT:
			*type |= AST_AT_INT;
			return 1;
		case TT_SIGNED:
			*type |= AST_AT_SIGNED;
			return 1;
		case TT_UNSIGNED:
			*type |= AST_AT_UNSIGNED;
			return 1;
		case TT_SHORT:
			*type |= AST_AT_SHORT;
			return 1;
		case TT_LONG:
			*type |= AST_AT_LONG;
			return 1;
		case TT_FLOAT:
			*type |= AST_AT_FLOAT;
			return 1;
		case TT_DOUBLE:
			*type |= AST_AT_DOUBLE;
			return 1;
		default:
			return 0;
	}
}

int printASTArithType(const ASTArithType *type) {
	char isFirst = 1;
	int n = 0;
	n += printf("[");

	if (*type & AST_AT_SIGNED) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"signed\"");
		isFirst = 0;
	}

	if (*type & AST_AT_UNSIGNED) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"unsigned\"");
		isFirst = 0;
	}

	if (*type & AST_AT_SHORT) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"short\"");
		isFirst = 0;
	}

	if (*type & AST_AT_LONG) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"long\"");
		isFirst = 0;
	}

	if (*type & AST_AT_CHAR) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"char\"");

	}

	if (*type & AST_AT_INT) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"int\"");
		isFirst = 0;
	}

	if (*type & AST_AT_FLOAT) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"float\"");
		isFirst = 0;
	}

	if (*type & AST_AT_DOUBLE) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"double\"");
		isFirst = 0;
	}

	n += printf("]");

	return n;
}

int astArithTypeNormalize(const ASTArithType *type) {
	switch (*type) {
		case AST_AT_CHAR:
			return AST_AT_CHAR;

		case (AST_AT_SIGNED | AST_AT_CHAR):
			return AST_AT_SIGNED | AST_AT_CHAR;

		case (AST_AT_UNSIGNED | AST_AT_CHAR):
			return AST_AT_UNSIGNED | AST_AT_CHAR;

		case (AST_AT_SHORT):
		case (AST_AT_SHORT | AST_AT_INT):
		case (AST_AT_SIGNED | AST_AT_SHORT):
		case (AST_AT_SIGNED | AST_AT_SHORT | AST_AT_INT):
			return AST_AT_SHORT | AST_AT_INT;

		case (AST_AT_UNSIGNED | AST_AT_SHORT):
		case (AST_AT_UNSIGNED | AST_AT_SHORT | AST_AT_INT):
			return AST_AT_UNSIGNED | AST_AT_SHORT | AST_AT_INT;

		case (AST_AT_INT):
		case (AST_AT_SIGNED):
		case (AST_AT_SIGNED | AST_AT_INT):
			return AST_AT_INT;

		case (AST_AT_UNSIGNED):
		case (AST_AT_UNSIGNED | AST_AT_INT):
			return AST_AT_UNSIGNED | AST_AT_INT;

		case (AST_AT_LONG):
		case (AST_AT_LONG | AST_AT_INT):
		case (AST_AT_SIGNED | AST_AT_LONG):
		case (AST_AT_SIGNED | AST_AT_LONG | AST_AT_INT):
			return AST_AT_LONG | AST_AT_INT;

		case (AST_AT_UNSIGNED | AST_AT_LONG):
		case (AST_AT_UNSIGNED | AST_AT_LONG | AST_AT_INT):
			return AST_AT_UNSIGNED | AST_AT_LONG | AST_AT_INT;

		case(AST_AT_FLOAT):
			return AST_AT_FLOAT;

		case(AST_AT_DOUBLE):
			return AST_AT_DOUBLE;

		case (AST_AT_LONG | AST_AT_DOUBLE):
			return AST_AT_LONG | AST_AT_DOUBLE;

		default:
			return AST_AT_NONE;
	}
}

int _parseTypedefIdentifier(
		char **typedefName,
		const Token *tok,
		ASTScope const *scope)
{
	if (astHasErr()) {
		return 0;
	}

	if (tok->type != TT_IDENTIFIER) {
		return 0;
	}

	if (astScopeIsTypedef(scope, tok->contents)) {
		*typedefName = strdup(tok->contents);
		return 1;
	} else {
		return 0;
	}
}

void initASTTypeSpec(ASTTypeSpec *typeSpec) {
	initASTTypeQualifier(&typeSpec->qualifiers);
	initASTStorageClassSpec(&typeSpec->storage);
	typeSpec->typeSpecType = AST_TST_UNKNOWN;
	typeSpec->tok = NULL;
}

void freeASTTypeSpec(ASTTypeSpec *typeSpec) {
	switch (typeSpec->typeSpecType) {
		case AST_TST_TYPEDEF:
			free(typeSpec->c.typedefName);
			break;
		case AST_TST_STRUCT:
			freeASTStructDecl(&typeSpec->c.structDecl);
			break;
		case AST_TST_ENUM:
			freeASTEnumDecl(&typeSpec->c.enumDecl);
			break;
		default:
			break;
	}
	typeSpec->typeSpecType = AST_TST_UNKNOWN;
}

int parseASTTypeSpec(
		ASTTypeSpec *typeSpec,
		const Token *tok,
		ASTScope const *scope)
{
	int n = 0, res;

	initASTTypeSpec(typeSpec);
	if (astHasErr()) {
		freeASTTypeSpec(typeSpec);
		return 0;
	}

	typeSpec->tok = tok + n;
	while (1) {
		if (tok[n].type == TT_VOID) {
			if (typeSpec->typeSpecType != AST_TST_UNKNOWN) {
				astErr("Multiple types in type specifier", tok + n);
				freeASTTypeSpec(typeSpec);
				return 0;
			}
			typeSpec->typeSpecType = AST_TST_VOID;
			n++;
		} else if (_isArith(tok[n].type)) {
			if (typeSpec->typeSpecType == AST_TST_UNKNOWN) {
				initASTArithType(&typeSpec->c.arith);
				n += parseASTArithType(&typeSpec->c.arith, tok + n);
				typeSpec->typeSpecType = AST_TST_ARITH;
			} else if (typeSpec->typeSpecType == AST_TST_ARITH) {
				n +=  parseASTArithType(&typeSpec->c.arith, tok + n);
			} else {
				astErr("Multiple types in type specifier", tok + n);
				freeASTTypeSpec(typeSpec);
				return 0;
			}
		} else if (tok[n].type == TT_IDENTIFIER) {
			if (typeSpec->typeSpecType != AST_TST_UNKNOWN) break;

			if ((res = _parseTypedefIdentifier(&typeSpec->c.typedefName, tok + n, scope))) {
				n += res;
				typeSpec->typeSpecType = AST_TST_TYPEDEF;
			} else {
				astErr("Unexpected identifier", tok + n);
				freeASTTypeSpec(typeSpec);
				return 0;
			}
		} else if (tok[n].type == TT_STRUCT || tok[n].type == TT_UNION) {
			if (typeSpec->typeSpecType != AST_TST_UNKNOWN) {
				astErr("Multiple types in type specifier", tok + n);
				freeASTTypeSpec(typeSpec);
				return 0;
			}

			if ((res = parseASTStructDecl(&typeSpec->c.structDecl, tok + n, scope))) {
				n += res;
				typeSpec->typeSpecType = AST_TST_STRUCT;
			} else {
				if (!astHasErr()) {
					astErr("Invalid struct declaration", tok + n);
				}
			}
		} else if (tok[n].type == TT_ENUM) {
			if (typeSpec->typeSpecType != AST_TST_UNKNOWN) {
				astErr("Unexpected identifier", tok + n);
				freeASTTypeSpec(typeSpec);
				return 0;
			}

			if ((res = parseASTEnumDecl(&typeSpec->c.enumDecl, tok + n))) {
				n += res;
				typeSpec->typeSpecType = AST_TST_ENUM;
			} else {
				if (!astHasErr()) {
					astErr("Invalid enum declaration", tok + n);
				}
			}
		} else if ((res = parseASTStorageClassSpec(&typeSpec->storage, tok + n))) {
			n += res;
		} else if ((res = parseASTTypeQualifier(&typeSpec->qualifiers, tok + n))) {
			n += res;
		} else {
			break;
		}
	}

	return n;
}

int printASTTypeSpec(ASTTypeSpec const * typeSpec) {
	int n = 0;

	switch (typeSpec->typeSpecType) {
		case AST_TST_VOID:
			n += printf("\"void\"");
			break;
		case AST_TST_ARITH:
			n += printASTArithType(&typeSpec->c.arith);
			break;
		case AST_TST_TYPEDEF:
			n += printf("\"%s\"", typeSpec->c.typedefName);
			break;
		case AST_TST_STRUCT:
			n += printASTStructDecl(&typeSpec->c.structDecl);
			break;
		case AST_TST_ENUM:
			n += printASTEnumDecl(&typeSpec->c.enumDecl);
			break;
		default:
			n += printf("\"unknown\"");
	}

	return n;
}

void initASTDeclarator(ASTDeclarator *declarator) {
	declarator->type = AST_DT_UNKNOWN;
	declarator->initializer = NULL;
	declarator->bitField = NULL;
	declarator->qualifiers = AST_TQ_NONE;
}

void freeASTDeclarator(ASTDeclarator *declarator) {
	switch (declarator->type) {
		case AST_DT_IDENTIFIER:
			free(declarator->c.identifier);
			break;
		case AST_DT_POINTER:
			if (declarator->c.pointer) {
				freeASTDeclarator(declarator->c.pointer);
				free(declarator->c.pointer);
			}
			break;
		case AST_DT_ARRAY:
			freeASTArrayDecl(&declarator->c.array);
			break;
		case AST_DT_FUNC:
			freeASTFuncDecl(&declarator->c.func);
			break;
		default:
			break;
	}
	if (declarator->initializer) {
		freeASTInitializer(declarator->initializer);
		free(declarator->initializer);
		declarator->initializer = NULL;
	}
	if (declarator->bitField) {
		freeASTExp(declarator->bitField);
		free(declarator->bitField);
		declarator->bitField = NULL;
	}
}

int parseASTDeclarator(
		ASTDeclarator *declarator,
		const Token *tok,
		ASTScope const *scope)
{
	int n = 0, res;

	initASTDeclarator(declarator);

	if (astHasErr()) {
		return 0;
	}

	if (tok[n].type == TT_IDENTIFIER) {
		declarator->type = AST_DT_IDENTIFIER;
		declarator->c.identifier = strdup(tok[n].contents);
		n++;
	} else if (tok[n].type == TT_O_PARAN) {
		n++;
		if ((res = parseASTDeclarator(declarator, tok + n, scope))) {
			n += res;
		} else {
			//Don't need to free since the function call will do it
			return 0;
		}
		if (tok[n].type == TT_C_PARAN) {
			n++;
		} else {
			freeASTDeclarator(declarator);
			return 0;
		}
	} else if (tok[n].type == TT_MULT) {
		ASTDeclarator tempDeclarator;
		n++;

		while ((res = parseASTTypeQualifier(&declarator->qualifiers, tok + n))) {
			n += res;
		}

		if ((res = parseASTDeclarator(&tempDeclarator, tok + n, scope))) {
			declarator->c.pointer = malloc(sizeof(ASTDeclarator));
			*declarator->c.pointer = tempDeclarator;
			n += res;
		} else {
			declarator->c.pointer = NULL;
		}
		declarator->type = AST_DT_POINTER;
	}

	if (tok[n].type == TT_O_PARAN) {
		ASTDeclarator temp, *tempPtr;

		if (declarator->type == AST_DT_UNKNOWN) {
			tempPtr = NULL;
		} else {
			temp = *declarator;
			tempPtr = &temp;
			initASTDeclarator(declarator);
		}

		if ((res = parseASTFuncDecl(&declarator->c.func, tok + n, tempPtr, scope))) {
			n += res;
			declarator->type = AST_DT_FUNC;
		} else {
			freeASTDeclarator(declarator);
			return 0;
		}
	} else {
		while (tok[n].type == TT_O_BRACE) {
			ASTDeclarator temp, *tempPtr;

			if (declarator->type == AST_DT_UNKNOWN) {
				tempPtr = NULL;
			} else {
				temp = *declarator;
				tempPtr = &temp;
				initASTDeclarator(declarator);
			}

			initASTDeclarator(declarator);
			if ((res = parseASTArrayDecl(&declarator->c.array, tok + n, tempPtr))) {
				n += res;
				declarator->type = AST_DT_ARRAY;
			} else {
				freeASTDeclarator(declarator);
				return 0;
			}
		}
	}

	if (tok[n].type == TT_EQL) {
		n++;
		declarator->initializer = malloc(sizeof(ASTInitializer));
		if ((res = parseASTInitializer(declarator->initializer, tok + n))) {
			n += res;
		} else {
			astErr("Expecting expression following =", tok + n);
			free(declarator->initializer);
			declarator->initializer = NULL;
			freeASTDeclarator(declarator);
			return 0;
		}
	} else if (tok[n].type == TT_COLON) {
		n++;
		declarator->bitField = malloc(sizeof(ASTExp));
		if ((res = parseASTExp(declarator->bitField, tok + n))) {
			n += res;
		} else {
			astErr("Expecting bitfield expression following =", tok + n);
			free(declarator->bitField);
			declarator->bitField = NULL;
			freeASTDeclarator(declarator);
			return 0;
		}
	}

	return n;
}

int printASTDeclarator(const ASTDeclarator *declarator) {
	int n = 0;

	n += printf("{");

	if (declarator->type == AST_DT_IDENTIFIER) {
		n += printf("\"name\": ");
		n += printf("\"%s\"", declarator->c.identifier);
	} else if (declarator->type == AST_DT_POINTER) {
		n += printf("\"pointer\": ");
		if (declarator->c.pointer) {
			n += printASTDeclarator(declarator->c.pointer);
		} else {
			n += printf("\"no name\"");
		}
	} else if (declarator->type == AST_DT_ARRAY) {
		n += printf("\"array\": ");
		n += printASTArrayDecl(&declarator->c.array);
	} else if (declarator->type == AST_DT_FUNC) {
		n += printf("\"func\": ");
		n += printASTFuncDecl(&declarator->c.func);
	} else {
		n += printf("\"error\": \"unknown\"");
	}

	if (declarator->qualifiers) {
		n += printf(", \"qualifiers\": ");
		n += printASTTypeQualifier(&declarator->qualifiers);
	}

	if (declarator->initializer) {
		n += printf(", \"initializer\": ");
		n += printASTInitializer(declarator->initializer);
	}

	if (declarator->bitField) {
		n += printf(", \"bitfield\": ");
		n += printASTExp(declarator->bitField);
	}

	n += printf("}");

	return n;
}

void initASTDeclaration(ASTDeclaration *declaration) {
	initASTTypeSpec(&declaration->typeSpec);
	initDList(&declaration->declarators, sizeof(ASTDeclaration));
}

void freeASTDeclaration(ASTDeclaration *declaration) {
	freeASTTypeSpec(&declaration->typeSpec);
	freeDList(&declaration->declarators, (FreeFunc) freeASTDeclarator);
}

int parseASTDeclaration(
		ASTDeclaration *declaration,
		const Token *tok,
		ASTScope const *scope)
{
	int n = 0, res;
	ASTDeclarator tempDeclarator;

	initASTDeclaration(declaration);

	if (astHasErr()) {
		freeASTDeclaration(declaration);
		return 0;
	}

	if ((res = parseASTTypeSpec(&declaration->typeSpec, tok + n, scope))) {
		n += res;
	} else {
		freeASTDeclaration(declaration);
		return 0;
	}

	if (tok[n].type == TT_SEMI_COLON) {
		n++;
		return n;
	}

	while (1) {
		if ((res = parseASTDeclarator(&tempDeclarator, tok + n, scope))) {
			n += res;
			dlistApp(&declaration->declarators, &tempDeclarator);
		} else {
			freeASTDeclaration(declaration);
			return 0;
		}

		if (tok[n].type == TT_COMMA) {
			n++;
		} else {
			break;
		}
	}

	if (tok[n].type != TT_SEMI_COLON) {
		astErr("Expected ; at end of statmeent", tok + n);
		freeASTDeclaration(declaration);
		return 0;
	}
	n++;

	return n;
}

int printASTDeclaration(const ASTDeclaration *declaration) {
	int n = 0, isFirst = 1;

	n += printf("{");

	if (declaration->typeSpec.qualifiers) {
		isFirst = 0;
		n += printf("\"Type Qualifiers\": ");
		n += printASTTypeQualifier(&declaration->typeSpec.qualifiers);
	}

	if (declaration->typeSpec.storage) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"Storage Class Specifiers\": ");
		n += printASTStorageClassSpec(&declaration->typeSpec.storage);
	}

	if (isFirst) {
		isFirst = 0;
	} else {
		n += printf(", ");
	}
	n += printf("\"type\": ");
	n += printASTTypeSpec(&declaration->typeSpec);

	n += printf(", \"Declarators\": ");
	n += printDList(&declaration->declarators, (PrintFunc) printASTDeclarator);

	n += printf("}");

	return n;
}

DList astDeclarationTypedefNames(const ASTDeclaration *declaration) {
	DList result;
	if (declaration->typeSpec.storage & AST_SC_TYPEDEF) {
		initDListCap(&result, sizeof(char *), declaration->declarators.size);
		for (int i = 0; i < declaration->declarators.size; i++) {
			ASTDeclarator const *declarator = dlistGet(&declaration->declarators, i);
			char *newName = astDeclaratorTypedefName(declarator);
			if (newName) {
				dlistApp(&result, &newName);
			}
		}
	} else {
		initDListEmpty(&result, sizeof(char *));
	}
	return result;
}

char *astDeclaratorTypedefName(const ASTDeclarator *declarator) {
	while (declarator) {
		switch (declarator->type) {
			case AST_DT_IDENTIFIER:
				return strdup(declarator->c.identifier);
			case AST_DT_POINTER:
				declarator = declarator->c.pointer;
				break;
			case AST_DT_ARRAY:
				declarator = declarator->c.array.encl;
				break;
			case AST_DT_FUNC:
				declarator = declarator->c.func.encl;
				break;
			default:
				break;
		}
	}
	return NULL;
}
