#include "test.h"
#include "tools.h"
#include "../ast/node.h"

/**
 * @file
 *
 * @brief Tests generating the ast
 */

/**
 * @brief Runs all ast gen tests
 */
static void astTests();

void astTestFile() {
	tStartSection("AST File");

	tAstSuccess(
			"", 
			(ASTNodeType[]) {AST_FILE, AST_UNKNOWN});

	tAstSuccess(
			"int;",
			(ASTNodeType[]) {AST_FILE, AST_DECLARATION, AST_TYPE_SPEC, AST_UNKNOWN});

	tAstSuccess(
			"int;\n"
			"char c;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_UNKNOWN
			});
}

void astDecl() {
	tStartSection("Ast Declaration");

	tAstSuccess(
			"unsigned int numeric;\n"
			"const unsigned int const_numberic;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_UNKNOWN
			});

	tAstSuccess(
			"unsigned int numeric;\n"
			"const unsigned int const_numberic;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_UNKNOWN
			});

	tAstSuccess(
			"typedef long int size_t;\n"
			"size_t typedef_value;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_IDENTIFIER_TS, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_UNKNOWN
			});

	tAstSuccess(
			"struct struct_t struct_value;\n"
			"struct {int m1; float m2;};\n"
			"struct struct2_t {int m3;};\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_STRUCT_DECL, AST_DECLARATOR, AST_IDENTIFIER_DECL, 
				AST_DECLARATION, AST_TYPE_SPEC, AST_STRUCT_DECL,
					AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_STRUCT_DECL,
					AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_UNKNOWN
			});

	tAstSuccess(
			"union union_t union_value;\n"
			"union {int m1; float m2;};\n"
			"union union2_t {int m3; };",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_STRUCT_DECL, AST_DECLARATOR, AST_IDENTIFIER_DECL, 
				AST_DECLARATION, AST_TYPE_SPEC, AST_STRUCT_DECL,
					AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_STRUCT_DECL,
					AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_UNKNOWN
			});

	tAstSuccess(
			"enum enum_t enum_value;\n"
			"enum enum2_t { VALUE_1, VALUE_2 = 2, VALUE_3 };\n"
			"enum {VALUE_1 = 0, VALUE_2};\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_ENUM_DECL, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_ENUM_DECL,
					AST_ENUM_CONST, AST_ENUM_CONST, AST_ENUM_CONST,
				AST_DECLARATION, AST_TYPE_SPEC, AST_ENUM_DECL,
					AST_ENUM_CONST, AST_ENUM_CONST, AST_ENUM_CONST,
				AST_UNKNOWN
			});
}

void astTests() {
	astTestFile();
	astDecl();
}
