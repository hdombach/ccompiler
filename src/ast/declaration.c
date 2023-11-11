#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astUtil.h"
#include "declaration.h"
#include "expression.h"
#include "identifier.h"
#include "initializer.h"
#include "../sem/scope.h"
#include "node.h"
#include "specialDecl.h"

/* =========================================================================
 * ASTTypeQualifier
 * ========================================================================= */

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

/* =========================================================================
 * ASTStorageClassSpec
 * ========================================================================= */

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

/* =========================================================================
 * ASTArithType
 * ========================================================================= */

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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"

ASTArithType astArithTypeNormalize(ASTArithType type) {
	switch (type) {
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

#pragma clang diagnostic pop


/* =========================================================================
 * ASTTypeSpec
 * ========================================================================= */

int _parseTypedefIdentifier(
		ASTIdentifier *identifier,
		const Token *tok,
		ASTScope *scope)
{
	AST_VALID(ASTIdentifier);
	int res, n = 0;
	if ((res = parseASTIdentifier(identifier, tok + n, scope))) {
		n += res;
		identifier->node.type = AST_IDENTIFIER_TS;
	}

	if (!astScopeIsTypedef(scope, identifier->name)) {
		freeASTNode((ASTNode *) identifier);
		return 0;
	}
	return n;
}

void initASTTypeSpec(ASTTypeSpec *typeSpec, Token const *tok) {
	initASTNode((ASTNode *) typeSpec, tok);
	initASTTypeQualifier(&typeSpec->qualifiers);
	initASTStorageClassSpec(&typeSpec->storage);
	typeSpec->typeSpecType = AST_TST_UNKNOWN;
	typeSpec->tok = NULL;
	typeSpec->content = NULL;
}

void freeASTTypeSpec(ASTTypeSpec *typeSpec) {
	if (typeSpec->content) {
		freeASTNode(typeSpec->content);
		free(typeSpec->content);
	}
	typeSpec->typeSpecType = AST_TST_UNKNOWN;
}

int parseASTTypeSpec(
		ASTTypeSpec *typeSpec,
		const Token *tok,
		ASTScope *scope)
{
	AST_VALID(ASTTypeSpec);
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTTypeSpec(typeSpec, tok);
	if (astHasErr()) {
		freeASTTypeSpec(typeSpec);
		return 0;
	}

	typeSpec->tok = tok + n;
	while (1) {
		if (tok[n].type == TT_VOID) {
			//Void type
			if (typeSpec->typeSpecType != AST_TST_UNKNOWN) {
				astErr("Multiple types in type specifier", tok + n);
				freeASTTypeSpec(typeSpec);
				return 0;
			}
			typeSpec->typeSpecType = AST_TST_VOID;
			n++;
		} else if (_isArith(tok[n].type)) {
			//Number type
			if (typeSpec->typeSpecType == AST_TST_UNKNOWN) {
				initASTArithType(&typeSpec->arith);
				n += parseASTArithType(&typeSpec->arith, tok + n);
				typeSpec->typeSpecType = AST_TST_ARITH;
			} else if (typeSpec->typeSpecType == AST_TST_ARITH) {
				n +=  parseASTArithType(&typeSpec->arith, tok + n);
			} else {
				astErr("Multiple types in type specifier", tok + n);
				freeASTTypeSpec(typeSpec);
				return 0;
			}
		} else if (tok[n].type == TT_IDENTIFIER) {
			//Typedef type
			if (typeSpec->typeSpecType != AST_TST_UNKNOWN) break;

			if ((res = _parseTypedefIdentifier((ASTIdentifier *) &tempBuf, tok + n, scope))) {
				typeSpec->content = dupASTNode((ASTNode *) &tempBuf);
				n += res;
				typeSpec->typeSpecType = AST_TST_TYPEDEF;
			} else {
				astErr("Unexpected identifier 1", tok + n);
				freeASTTypeSpec(typeSpec);
				return 0;
			}
		} else if (tok[n].type == TT_STRUCT || tok[n].type == TT_UNION) {
			//struct or union type
			if (typeSpec->typeSpecType != AST_TST_UNKNOWN) {
				astErr("Multiple types in type specifier", tok + n);
				freeASTTypeSpec(typeSpec);
				return 0;
			}

			if ((res = parseASTStructDecl((ASTStructDecl *) &tempBuf, tok + n, scope))) {
				n += res;
				typeSpec->content = dupASTNode((ASTNode *) &tempBuf);
				typeSpec->typeSpecType = AST_TST_STRUCT;
			} else {
				if (!astHasErr()) {
					astErr("Invalid struct declaration", tok + n);
				}
				return 0;
			}
		} else if (tok[n].type == TT_ENUM) {
			//enum type
			if (typeSpec->typeSpecType != AST_TST_UNKNOWN) {
				astErr("Unexpected identifier 2", tok + n);
				freeASTTypeSpec(typeSpec);
				return 0;
			}

			if ((res = parseASTEnumDecl((ASTEnumDecl *) &tempBuf, tok + n, scope))) {
				n += res;
				typeSpec->content = dupASTNode((ASTNode *) &tempBuf);
				typeSpec->typeSpecType = AST_TST_ENUM;
			} else {
				if (!astHasErr()) {
					astErr("Invalid enum declaration", tok + n);
				}
				return 0;
			}
		} else if ((res = parseASTStorageClassSpec(&typeSpec->storage, tok + n))) {
			n += res;
		} else if ((res = parseASTTypeQualifier(&typeSpec->qualifiers, tok + n))) {
			n += res;
		} else {
			break;
		}
	}

	typeSpec->node.type = AST_TYPE_SPEC;
	return n;
}

int printASTTypeSpec(ASTTypeSpec const * typeSpec) {
	int n = 0;

	switch (typeSpec->typeSpecType) {
		case AST_TST_VOID:
			n += printf("\"void\"");
			break;
		case AST_TST_ARITH:
			n += printASTArithType(&typeSpec->arith);
			break;
		default:
			if (typeSpec->content) {
				n += printASTNode(typeSpec->content);
			} else {
				n += printf("\"unknown\"");
			}
	}

	return n;
}

int astTypeSpecChildCount(const ASTTypeSpec *typeSpec) {
	return 1;
}

ASTNode *astTypeSpecGetChild(ASTTypeSpec *typeSpec, int index) {
	return typeSpec->content;
}

/* =========================================================================
 * ASTDeclarator
 * ========================================================================= */

void initASTDeclarator(ASTDeclarator *declarator, Token const *tok) {
	initASTNode((ASTNode *) declarator, tok);
	declarator->initializer = NULL;
	declarator->bitField = NULL;
	declarator->qualifiers = AST_TQ_NONE;
	declarator->encl = NULL;
}

void freeASTDeclarator(ASTDeclarator *declarator) {
	if (declarator->encl) {
		freeASTNode(declarator->encl);
		free(declarator->encl);
		declarator->encl = NULL;
	}
	if (declarator->initializer) {
		freeASTNode(declarator->initializer);
		free(declarator->initializer);
		declarator->initializer = NULL;
	}
	if (declarator->bitField) {
		freeASTNode(declarator->bitField);
		free(declarator->bitField);
		declarator->bitField = NULL;
	}
	declarator->node.type = AST_UNKNOWN;
}

int parseASTDeclarator(
		ASTDeclarator *declarator,
		const Token *tok,
		ASTScope *scope)
{
	AST_VALID(ASTDeclarator);
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTDeclarator(declarator, tok);

	if (astHasErr()) {
		return 0;
	}

	if ((res = parseASTIdentifier((ASTIdentifier *) &tempBuf, tok + n, scope))) {
		// Main name of variable
		// ex: int value;
		tempBuf.node.type = AST_IDENTIFIER_DECL;
		declarator->encl = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else if (tok[n].type == TT_O_PARAN) {
		// If type is wrapped in paranthesis
		// ex: int (value);
		n++;
		if ((res = parseASTDeclarator(declarator, tok + n, scope))) {
			n += res;
		} else {
			freeASTDeclarator(declarator);
			return 0;
		}
		if (tok[n].type == TT_C_PARAN) {
			n++;
		} else {
			freeASTDeclarator(declarator);
			return 0;
		}
	} else if (tok[n].type == TT_MULT) {
		//Specifing a pointer
		n++;

		while ((res = parseASTTypeQualifier(&declarator->qualifiers, tok + n))) {
			n += res;
		}

		if ((res = parseASTDeclarator((ASTDeclarator *) &tempBuf, tok + n, scope))) {
			tempBuf.node.type = AST_POINTER_DECL;
			declarator->encl = dupASTNode((ASTNode *) &tempBuf);
			n += res;
		} else {
			declarator->encl = NULL;
		}
	}

	if (tok[n].type == TT_O_PARAN) {
		//Searches forward for a function declaration
		ASTNode *enclBuf = NULL;

		if (declarator->encl) {
			enclBuf = declarator->encl;
		}

		if ((res = parseASTFuncDecl((ASTFuncDecl *) &tempBuf, tok + n, enclBuf, scope))) {
			n += res;
			declarator->encl = dupASTNode((ASTNode *) &tempBuf);
		} else {
			freeASTDeclarator(declarator);
			return 0;
		}
	} else {
		while (tok[n].type == TT_O_BRACE) {
			//Searches forward for an array declaration
			ASTNode *enclBuf = NULL;

			if (declarator->encl) {
				enclBuf = declarator->encl;
			}

			if ((res = parseASTArrayDecl((ASTArrayDecl *) &tempBuf, tok + n, enclBuf, scope))) {
				n += res;
			} else {
				freeASTDeclarator(declarator);
				return 0;
			}
			declarator->encl = dupASTNode((ASTNode *) &tempBuf);
		}
	}

	if (tok[n].type == TT_EQL) {
		n++;
		if ((res = parseASTInitializer((ASTNode *) &tempBuf, tok + n, scope))) {
			n += res;
			declarator->initializer = dupASTNode((ASTNode *) &tempBuf);
		} else {
			astErr("Expecting expression following =", tok + n);
			free(declarator->initializer);
			declarator->initializer = NULL;
			freeASTDeclarator(declarator);
			return 0;
		}
	} else if (tok[n].type == TT_COLON) {
		n++;
		declarator->bitField = malloc(AST_NODE_S);
		if ((res = parseASTExp(declarator->bitField, tok + n, scope))) {
			n += res;
		} else {
			astErr("Expecting bitfield expression following =", tok + n);
			free(declarator->bitField);
			declarator->bitField = NULL;
			freeASTDeclarator(declarator);
			return 0;
		}
	}
	declarator->node.type = AST_DECLARATOR;
	return n;
}

int printASTDeclarator(const ASTDeclarator *declarator) {
	int n = 0;

	n += printf("{");

	n += printf("\"Node type\": \"Declarator\"");

	n += printf(", \"enclosed\": ");
	if (declarator->encl) {
		n += printASTNode(declarator->encl);
	} else {
		n += printf("\"null\"");
	}

	if (declarator->qualifiers) {
		n += printf(", \"qualifiers\": ");
		n += printASTTypeQualifier(&declarator->qualifiers);
	}

	if (declarator->initializer) {
		n += printf(", \"initializer\": ");
		n += printASTNode(declarator->initializer);
	}

	if (declarator->bitField) {
		n += printf(", \"bitfield\": ");
		n += printASTNode(declarator->bitField);
	}

	n += printf("}");

	return n;
}

int astDeclaratorChildCount(const ASTDeclarator *node) {
	return 3;
}

ASTNode *astDeclaratorGetChild(ASTDeclarator *node, int index) {
	return (ASTNode *[]) {
		node->encl,
		node->bitField,
		node->initializer,
	}[index];
}

/* =========================================================================
 * ASTDeclaration
 * ========================================================================= */

void initASTDeclaration(ASTDeclaration *declaration, Token const *tok) {
	initASTNode((ASTNode *) declaration, tok);
	declaration->typeSpec = NULL;
	initDList(&declaration->declarators, AST_NODE_S);
}

void freeASTDeclaration(ASTDeclaration *declaration) {
	if (declaration->typeSpec) {
		freeASTTypeSpec(declaration->typeSpec);
		free(declaration->typeSpec);
	}
	freeDList(&declaration->declarators, (FreeFunc) freeASTDeclarator);
}

int parseASTDeclaration(
		ASTDeclaration *declaration,
		const Token *tok,
		ASTScope *scope)
{
	AST_VALID(ASTDeclaration);
	int n = 0, res;
	ASTNodeBuf tempBuf;
	DList typedefNames;

	initASTDeclaration(declaration, tok);

	if (astHasErr()) {
		freeASTDeclaration(declaration);
		return 0;
	}

	if ((res = parseASTTypeSpec((ASTTypeSpec *) &tempBuf, tok + n, scope))) {
		declaration->typeSpec = (ASTTypeSpec *) dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else {
		freeASTDeclaration(declaration);
		return 0;
	}

	if (tok[n].type == TT_SEMI_COLON) {
		n++;
		declaration->node.type = AST_DECLARATION;
		return n;
	}

	while (1) {
		if ((res = parseASTDeclarator((ASTDeclarator *) &tempBuf, tok + n, scope))) {
			n += res;
			dlistApp(&declaration->declarators, &tempBuf);
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

	typedefNames = astDeclarationTypedefNames(declaration);
	astScopeAddTypedefNames(scope, typedefNames);

	declaration->node.type = AST_DECLARATION;
	return n;
}

int printASTDeclaration(const ASTDeclaration *declaration) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"Declarationn\"");

	if (declaration->typeSpec->qualifiers) {
		n += printf(", \"Type Qualifiers\": ");
		n += printASTTypeQualifier(&declaration->typeSpec->qualifiers);
	}

	if (declaration->typeSpec->storage) {
		n += printf(", \"Storage Class Specifiers\": ");
		n += printASTStorageClassSpec(&declaration->typeSpec->storage);
	}

	n += printf(", \"type\": ");
	n += printASTTypeSpec(declaration->typeSpec);

	n += printf(", \"Declarators\": ");
	n += printDList(&declaration->declarators, (PrintFunc) printASTDeclarator);

	n += printf("}");

	return n;
}

int astDeclarationChildCount(const ASTDeclaration *node) {
	return node->declarators.size + 1;
}

ASTNode *astDeclarationGetChild(ASTDeclaration *node, int index) {
	if (index < 1) {
		return (ASTNode *) node->typeSpec;
	} else {
		return dlistGetm(&node->declarators, index - 1);
	}
}

DList astDeclarationTypedefNames(const ASTDeclaration *declaration) {
	DList result;
	if (declaration->typeSpec->storage & AST_SC_TYPEDEF) {
		initDListCap(&result, sizeof(char *), declaration->declarators.size);
		for (int i = 0; i < declaration->declarators.size; i++) {
			ASTDeclarator const *declarator = dlistGet(&declaration->declarators, i);
			char *newName = strdup(astDeclaratorName(declarator));
			if (newName) {
				dlistApp(&result, &newName);
			}
		}
	} else {
		initDListEmpty(&result, sizeof(char *));
	}
	return result;
}

const char *astDeclaratorName(const ASTDeclarator *declarator) {
	ASTNode *curDecl = (ASTNode *) declarator;
	while (curDecl) {
		switch (curDecl->type) {
			case AST_IDENTIFIER_DECL:
				return ((ASTIdentifier *) curDecl)->name;
				break;
			case AST_DECLARATOR:
			case AST_POINTER_DECL:
				curDecl = ((ASTDeclarator *) curDecl)->encl;
				break;
			case AST_ARRAY_DECL:
				curDecl = (ASTNode *) ((ASTArrayDecl *) curDecl)->encl;
				break;
			case AST_FUNC_DECL:
				curDecl = (ASTNode *) ((ASTFuncDecl *) curDecl)->encl;
				break;
			default:
				declarator = NULL;
				break;
		}
	}
	return NULL;
}
