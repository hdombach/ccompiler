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

void astSimpleDecl() {
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

	tAstSuccess(
			"typedef struct test1 test;\n"
			"typedef struct test2_s test2_t;\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_STRUCT_DECL, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_STRUCT_DECL, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_UNKNOWN
			});
}

void astDeclaratorTest() {
	tAstSuccess(
			"void *ptr_test;\n"
			"void **ptr_test2;\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_POINTER_DECL, AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_POINTER_DECL, AST_POINTER_DECL, AST_IDENTIFIER_DECL,
				AST_UNKNOWN
			});

	tAstSuccess(
			"void arr_test[];\n"
			"void arr_test2[][];\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_ARRAY_DECL, AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_ARRAY_DECL, AST_ARRAY_DECL, AST_IDENTIFIER_DECL,
				AST_UNKNOWN
			});

	tAstSuccess(
			"void *mix_test[];\n"
			"void (*mix_test2)[];\n"
			"void *(mix_test3[]);\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_POINTER_DECL, AST_ARRAY_DECL, AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_ARRAY_DECL, AST_POINTER_DECL, AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_POINTER_DECL, AST_ARRAY_DECL, AST_IDENTIFIER_DECL,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"void (*mix_test[])[];\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_ARRAY_DECL, AST_POINTER_DECL, AST_ARRAY_DECL, AST_IDENTIFIER_DECL,
				AST_UNKNOWN
			});

	tAstSuccess(
			"void func1();\n"
			"int func2();\n"
			"unsigned func3(int value, ...);\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
					AST_PARAM, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_IDENTIFIER_DECL,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"void *malloc(int len);\n"
			"void free(void *mem);\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_POINTER_DECL, AST_FUNC_DECL,
					AST_PARAM, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
					AST_PARAM, AST_TYPE_SPEC, AST_DECLARATOR, AST_POINTER_DECL, AST_IDENTIFIER_DECL,
				AST_IDENTIFIER_DECL,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"void * (*callback)(void *);\n"
			"int (*(*foo)(double))[3];\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_POINTER_DECL, AST_FUNC_DECL,
					AST_PARAM, AST_TYPE_SPEC, AST_DECLARATOR, AST_POINTER_DECL, AST_POINTER_DECL,
				AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_ARRAY_DECL,
					AST_INT_CONSTANT, AST_POINTER_DECL, AST_FUNC_DECL,
					AST_PARAM, AST_TYPE_SPEC, AST_POINTER_DECL, AST_IDENTIFIER_DECL,
				AST_UNKNOWN
			});
}

void astInitializersTest() {
	tAstSuccess(
			"int test1 = 5;\n"
			"void *ptr_test = NULL;\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_INT_CONSTANT,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_POINTER_DECL, AST_IDENTIFIER_DECL, AST_IDENTIFIER,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"struct position = {1, 4};\n"
			"struct thing = {NULL, {2, 4}, &position};",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_STRUCT_DECL, AST_DECLARATOR,
					AST_INITIALIZER_LIST, AST_INT_CONSTANT, AST_INT_CONSTANT,
				AST_DECLARATION, AST_TYPE_SPEC, AST_STRUCT_DECL, AST_DECLARATOR,
					AST_INITIALIZER_LIST, AST_IDENTIFIER,
						AST_INITIALIZER_LIST, AST_INT_CONSTANT, AST_INT_CONSTANT, AST_PREFIX_OPERATION, AST_IDENTIFIER,
				AST_UNKNOWN,
			});
}

void astConstantTest() {
	tAstDebug(
			"int int_test = 5;\n");

	tAstDebug(
			"float f_test = 5.23;\n");

	tAstDebug(
			"float f_test = 5.23f;\n");

	tAstDebug(
			"float f_test = 5.23f;\n"
			"float f_test2 = -5.235;\n");

	tAstDebug(
			"double f_test = 5.23f;\n"
			"double f_test2 = -5.235;\n");

	tAstDebug(
			"char c_test = '5';\n"
			"char c_test2 = '\\n'\n;"
			"char c_test3 = '\''\n;");

	tAstDebug(
			"char *s_test = \"hello world\";\n"
			"char *s_test = \"hello \"\"world\";\n");

	tAstDebug(
			"struct box_t box = prev_box;\n");

}

void astTests() {
	astTestFile();
	astSimpleDecl();
	astDeclaratorTest();
	astInitializersTest();
	TODO("Figure out why constant tests aren't working");
	//astConstantTest();
}
