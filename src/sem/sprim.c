#include "sprim.h"

#include <stdio.h>

#include "../util/log.h"
#include "../ast/declaration.h"


static STypeVTable _primVTable = {
	{
		NULL,
		(PrintFunc) printSPrim,
	}
};

void initSPrim(SPrim *type) {
		initSType((SType *) type, &_primVTable);
		type->primType = SPT_UNKNOWN;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"

int loadSPrim(SPrim *type, ASTTypeSpec *typeSpec) {
	STYPE_VALID(SPrim);
	if (!ASSERT(typeSpec->node.type == AST_TYPE_SPEC)) return 0;

	ASTArithType arith = astArithTypeNormalize(typeSpec->arith);

	initSPrim(type);
	switch (arith) {
		case AST_AT_CHAR:
			type->primType = SPT_CHAR;
			break;
		case (AST_AT_SIGNED | AST_AT_CHAR):
			type->primType = SPT_SCHAR;
			break;
		case (AST_AT_UNSIGNED | AST_AT_CHAR):
			type->primType = SPT_UCHAR;
			break;
		case (AST_AT_SHORT | AST_AT_INT):
			type->primType = SPT_SHORT;
			break;
		case (AST_AT_UNSIGNED | AST_AT_SHORT | AST_AT_INT):
			type->primType = SPT_USHORT;
			break;
		case (AST_AT_INT):
			type->primType = SPT_INT;
			break;
		case (AST_AT_UNSIGNED | AST_AT_INT):
			type->primType = SPT_UINT;
			break;
		case (AST_AT_LONG | AST_AT_INT):
			type->primType = SPT_LONG;
			break;
		case (AST_AT_UNSIGNED | AST_AT_LONG | AST_AT_INT):
			type->primType = SPT_ULONG;
			break;
		case (AST_AT_FLOAT):
			type->primType = SPT_FLOAT;
			break;
		case (AST_AT_DOUBLE):
			type->primType = SPT_DOUBLE;
			break;
		case (AST_AT_LONG | AST_AT_DOUBLE):
			type->primType = SPT_LDOUBLE;
			break;
		default:
			return 0;
	}
	if (typeSpec->storage & AST_TST_TYPEDEF) {
		type->type.isTypedef = 1;
	}
	//lovely line of code here
	type->type.type = STT_PRIM;
	return 1;
}

#pragma clang diagnostic pop

const char *sptStr(SPrimType t) {
	switch (t) {
		case SPT_CHAR: return "char";
		case SPT_UCHAR: return "unsigned char";
		case SPT_SCHAR: return "signed char";
		case SPT_SHORT: return "short";
		case SPT_USHORT: return "unsigned short";
		case SPT_INT: return "int";
		case SPT_UINT: return "unsigned int";
		case SPT_LONG: return "long";
		case SPT_ULONG: return "unsigned long";
		case SPT_FLOAT: return "float";
		case SPT_DOUBLE: return "double";
		case SPT_LDOUBLE: return "long double";
		default: return "(unknown)";
	}
}

int printSPrim(const SPrim *type) {
	if (!ASSERT(type->type.type == STT_PRIM || type->type.type == STT_ENUM_CONST)) return 0;
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"%s\"", sttStr(type->type.type));

	n += printf(", \"content\": \"%s\"", sptStr(type->primType));

	n += printf("}");

	return n;
}
