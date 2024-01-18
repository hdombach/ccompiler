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

static void astTestFile() {
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

	tAstSuccess(";", (ASTNodeType[]) {AST_FILE, AST_UNKNOWN});

	tAstFailed("test = 5;", (CError[]) {CERR_IDENTIFIER, CERR_UNKNOWN_TOK, CERR_UNKNOWN});
}

static void astSimpleDecl() {
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

	TODO("const;");

	tAstFailed(
			"const int thing = 2",
			(CError[]) {CERR_MISSING_SEMI, CERR_UNKNOWN_TOK, CERR_UNKNOWN});
}

static void astDeclaratorTest() {
	tStartSection("Declarator test");

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

	tAstFailed("[];", (CError[]) {CERR_UNKNOWN_TOK, CERR_UNKNOWN});
}

static void astInitializersTest() {
	tStartSection("Initializer test");

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

static void astConstantTest() {
	tStartSection("Constant tests");

	tAstSuccess(
			"int int_test = 5;\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_INT_CONSTANT,
				AST_UNKNOWN
			});

	tAstSuccess(
			"float f_test = 5.23f;\n"
			"float f_test2 = -5.235;\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_FLOAT_CONSTANT,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_PREFIX_OPERATION, AST_FLOAT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"double d_test = 5.23F;\n"
			"double d_test2 = -5.235;\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_FLOAT_CONSTANT,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_PREFIX_OPERATION, AST_FLOAT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"double d_test = .52e3;\n"
			"double d_test2 = 52.e+5;\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_FLOAT_CONSTANT,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_FLOAT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"char c_test = '5';\n"
			"char c_test2 = '\\n';\n"
			"char c_test3 = '\\'';\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_CHAR_CONSTANT,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_CHAR_CONSTANT,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_CHAR_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"char *s_test = \"hello world\";\n"
			"char *s_test2 = \"hello \\\"world\";\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_POINTER_DECL, AST_IDENTIFIER_DECL, AST_STR_CONSTANT,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_POINTER_DECL, AST_IDENTIFIER_DECL, AST_STR_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"struct box_t box = prev_box;\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_STRUCT_DECL, AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_IDENTIFIER,
				AST_UNKNOWN,
			});
}

static void astFuncOperationTest() {
	tStartSection("Func operation tests");

	tAstSuccess(
			"int main() {\n"
			"\tsayHi();"
			"}",
			(ASTNodeType[]) {
				AST_FILE,
				AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, AST_IDENTIFIER_DECL, AST_COMP_STM,
					AST_STM, AST_FUNC_OPERATION, AST_IDENTIFIER,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int main() {\n"
			"\tprintf(\"hello world\");"
			"}",
			(ASTNodeType[]) {
				AST_FILE,
				AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, AST_IDENTIFIER_DECL, AST_COMP_STM,
					AST_STM, AST_FUNC_OPERATION, AST_IDENTIFIER, AST_STR_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int main() {\n"
			"\tprintInt((5 - 2));"
			"}",
			(ASTNodeType[]) {
				AST_FILE,
				AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, AST_IDENTIFIER_DECL, AST_COMP_STM,
					AST_STM, AST_FUNC_OPERATION, AST_IDENTIFIER, AST_BINARY_OPERATION, AST_INT_CONSTANT, AST_INT_CONSTANT,
				AST_UNKNOWN
			});

	tAstSuccess(
			"int main() {\n"
			"\tcallbacks.sayHi();"
			"}",
			(ASTNodeType[]) {
				AST_FILE,
				AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, AST_IDENTIFIER_DECL, AST_COMP_STM,
					AST_STM, AST_FUNC_OPERATION, AST_BINARY_OPERATION, AST_IDENTIFIER, AST_IDENTIFIER,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int main() {\n"
			"\tcallbacks.sayHi(\"hello\");"
			"}",
			(ASTNodeType[]) {
				AST_FILE,
				AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, AST_IDENTIFIER_DECL, AST_COMP_STM,
					AST_STM, AST_FUNC_OPERATION, AST_BINARY_OPERATION, AST_IDENTIFIER, AST_IDENTIFIER, AST_STR_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int main() {\n"
			"\tcallbacksList[4](help);"
			"}",
			(ASTNodeType[]) {
				AST_FILE,
				AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, AST_IDENTIFIER_DECL, AST_COMP_STM,
					AST_STM, AST_FUNC_OPERATION, AST_SUBS_OPERATION, AST_IDENTIFIER, AST_INT_CONSTANT, AST_IDENTIFIER,
				AST_UNKNOWN
			});
}

static void astSubsOperationTest() {
	tStartSection("Subscript operation test");

	tAstSuccess(
			"int main() {\n"
			"\thello[5];"
			"}",
			(ASTNodeType[]) {
				AST_FILE,
				AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, AST_IDENTIFIER_DECL, AST_COMP_STM,
					AST_STM, AST_SUBS_OPERATION, AST_IDENTIFIER, AST_INT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int main() {\n"
			"\thello[5][6];"
			"}",
			(ASTNodeType[]) {
				AST_FILE,
				AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, AST_IDENTIFIER_DECL, AST_COMP_STM,
					AST_STM, AST_SUBS_OPERATION, AST_SUBS_OPERATION, AST_IDENTIFIER, AST_INT_CONSTANT, AST_INT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int main() {\n"
			"\thello[13].hello2[14];"
			"}",
			(ASTNodeType[]) {
				AST_FILE,
				AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, AST_IDENTIFIER_DECL, AST_COMP_STM,
					AST_STM, AST_SUBS_OPERATION, AST_BINARY_OPERATION, AST_SUBS_OPERATION, AST_IDENTIFIER, AST_INT_CONSTANT, AST_IDENTIFIER, AST_INT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int main() {\n"
			"\tgetList()[2];"
			"}",
			(ASTNodeType[]) {
				AST_FILE,
				AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, AST_IDENTIFIER_DECL, AST_COMP_STM,
					AST_STM, AST_SUBS_OPERATION, AST_FUNC_OPERATION, AST_IDENTIFIER, AST_INT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int main() {\n"
			"\tgetList[2]();"
			"}",
			(ASTNodeType[]) {
				AST_FILE,
				AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, AST_IDENTIFIER_DECL, AST_COMP_STM,
					AST_STM, AST_FUNC_OPERATION, AST_SUBS_OPERATION, AST_IDENTIFIER, AST_INT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"char thing = \"hello world\"[2];",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_SUBS_OPERATION, AST_STR_CONSTANT, AST_INT_CONSTANT,
				AST_UNKNOWN,
			});
}

static void astCondOperationTest() {
	tStartSection("Testing condition operation");

	tAstSuccess(
			"int test = 12 ? 2 : 2;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_COND_OPERATION, AST_INT_CONSTANT, AST_INT_CONSTANT, AST_INT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int test = 12 + 2 ? 42 || 32 : 2;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_COND_OPERATION, AST_BINARY_OPERATION, AST_INT_CONSTANT, AST_INT_CONSTANT,
					AST_BINARY_OPERATION, AST_INT_CONSTANT, AST_INT_CONSTANT,
					AST_INT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int test = test2 = var1 ? var2 : var3 && var4;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_BINARY_OPERATION, AST_IDENTIFIER, AST_COND_OPERATION,
						AST_IDENTIFIER,
						AST_IDENTIFIER,
						AST_BINARY_OPERATION, AST_IDENTIFIER, AST_IDENTIFIER,
				AST_UNKNOWN,
			});

}

static void astCastOperationTest() {
	tStartSection("Cast operation test");

	tAstSuccess(
			"int test = (int) 3.2;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_CAST_OPERATION, AST_PARAM, AST_TYPE_SPEC, AST_FLOAT_CONSTANT,
				AST_UNKNOWN
			});

	tAstSuccess(
			"int test = (int) (float) 3;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_CAST_OPERATION, AST_PARAM, AST_TYPE_SPEC,
					AST_CAST_OPERATION, AST_PARAM, AST_TYPE_SPEC,
					AST_INT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"typedef int value_t;\n"
			"int test = (value_t) value;\n",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_CAST_OPERATION, AST_PARAM, AST_TYPE_SPEC, AST_IDENTIFIER_TS,
					AST_IDENTIFIER,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int test = 4 + (int) 2.3f + 5;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_BINARY_OPERATION,
					AST_INT_CONSTANT,
					AST_BINARY_OPERATION,
						AST_CAST_OPERATION, AST_PARAM, AST_TYPE_SPEC, AST_FLOAT_CONSTANT,
						AST_INT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int test = (int) ++\"hello\"[2];",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_CAST_OPERATION, AST_PARAM, AST_TYPE_SPEC,
						AST_PREFIX_OPERATION, AST_SUBS_OPERATION, AST_STR_CONSTANT, AST_INT_CONSTANT,
				AST_UNKNOWN,
			});
}

static void astSizeofOperation() {
	tStartSection("Sizeof operation tests");

	tAstSuccess(
			"int test = sizeof(float);",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_SIZEOF_TYPE_OPERATION, AST_PARAM, AST_TYPE_SPEC,
				AST_UNKNOWN,
			});

	TODO("Add test for \"sizeof float\"");

	tAstSuccess(
			"int test = sizeof 4;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_SIZEOF_EXP_OPERATION, AST_INT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int test = sizeof(4);",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_SIZEOF_EXP_OPERATION, AST_INT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess("int test = sizeof value++;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_SIZEOF_EXP_OPERATION, AST_POSTFIX_OPERATION, AST_IDENTIFIER,
				AST_UNKNOWN,
			});
	
	tAstSuccess(
			"int test = sizeof value * 3;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_BINARY_OPERATION,
						AST_SIZEOF_EXP_OPERATION, AST_IDENTIFIER,
						AST_INT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int test = sizeof value++ * 3;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_BINARY_OPERATION,
						AST_SIZEOF_EXP_OPERATION, AST_POSTFIX_OPERATION, AST_IDENTIFIER,
						AST_INT_CONSTANT,
				AST_UNKNOWN,
			});
}

static void astBinaryOperationTest() {
	tStartSection("AST Binary operation testing");

	tAstSuccess(
		"int thing = 5 + 2;",
		(ASTNodeType[]) {
			AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_BINARY_OPERATION, AST_INT_CONSTANT, AST_INT_CONSTANT,
			AST_UNKNOWN,
		});

	tAstSuccess(
			"int thing = 5*2+5<<2;",
			(ASTNodeType[]) {
				AST_FILE,
					AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
						AST_BINARY_OPERATION, AST_BINARY_OPERATION, AST_BINARY_OPERATION,
						AST_INT_CONSTANT, AST_INT_CONSTANT, AST_INT_CONSTANT,
						AST_INT_CONSTANT, AST_INT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int thing = false&&hi;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_BINARY_OPERATION, AST_IDENTIFIER, AST_IDENTIFIER,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int thing = 5+-2;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
					AST_BINARY_OPERATION, AST_INT_CONSTANT, AST_PREFIX_OPERATION, AST_INT_CONSTANT,
				AST_UNKNOWN,
			});
}

static void astUnaryOperationTest() {
	tStartSection("Unary operator tests");

	tAstSuccess(
		"int value = thing++;",
		(ASTNodeType[]) {
			AST_FILE,
			AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
			AST_POSTFIX_OPERATION, AST_IDENTIFIER,
			AST_UNKNOWN,
		});

	tAstSuccess(
		"int value = --thing;",
		(ASTNodeType[]) {
			AST_FILE,
			AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
			AST_PREFIX_OPERATION, AST_IDENTIFIER, AST_UNKNOWN,
		});

	tAstSuccess(
		"int value = !thing();",
		(ASTNodeType[]) {
			AST_FILE,
			AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
			AST_PREFIX_OPERATION, AST_FUNC_OPERATION, AST_IDENTIFIER, AST_UNKNOWN,
		});

	tAstSuccess(
			"int value = !thing++;",
			(ASTNodeType[]) {
				AST_FILE,
				AST_DECLARATION, AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL,
				AST_PREFIX_OPERATION, AST_POSTFIX_OPERATION, AST_IDENTIFIER,
				AST_UNKNOWN,
			});
}

static void astTestIf() {
	tStartSection("Test ast if statements");

	tAstSuccess(
		"int main() {\n"
		"	if (value) {\n"
		" 	say_hi();\n"
		"	}\n"
		"}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, AST_IDENTIFIER_DECL,
			AST_COMP_STM, AST_STM, AST_IF, AST_IDENTIFIER, AST_STM, AST_COMP_STM, AST_STM,
			AST_FUNC_OPERATION, AST_IDENTIFIER, AST_UNKNOWN,
		});

	tAstSuccess(
		"int main() {\n"
		"	if (value) {\n"
		" 	say_hi();\n"
		"	} if (value) {\n"
		" 	thing = 5;\n"
		"	}\n"
		"}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_IF, AST_IDENTIFIER, AST_STM,
			AST_COMP_STM, AST_STM, AST_FUNC_OPERATION, AST_IDENTIFIER, AST_STM, AST_IF,
			AST_IDENTIFIER, AST_STM, AST_COMP_STM, AST_STM, AST_BINARY_OPERATION,
			AST_IDENTIFIER, AST_INT_CONSTANT, AST_UNKNOWN,
		});

	tAstSuccess(
		"int main() {\n"
		"	if (value) {\n"
		" 	say_hi();\n"
		"	} else {\n"
		" 	thing = 5;\n"
		"	}\n"
		"}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_IF, AST_IDENTIFIER,
			AST_STM, AST_COMP_STM, AST_STM, AST_FUNC_OPERATION, AST_IDENTIFIER,
			AST_STM, AST_COMP_STM, AST_STM, AST_BINARY_OPERATION, AST_IDENTIFIER,
			AST_INT_CONSTANT, AST_UNKNOWN,
		});

	tAstSuccess(
		"int main() {\n"
		"	if (value) {\n"
		" 	say_hi();\n"
		"	} else if (3) {\n"
		" 	thing = 5;\n"
		"	}\n"
		"}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_IF, AST_IDENTIFIER,
			AST_STM, AST_COMP_STM, AST_STM, AST_FUNC_OPERATION, AST_IDENTIFIER,
			AST_STM, AST_IF, AST_INT_CONSTANT, AST_STM, AST_COMP_STM, AST_STM,
			AST_BINARY_OPERATION, AST_IDENTIFIER, AST_INT_CONSTANT,
			AST_UNKNOWN,
		});
}

static void astSwitchTest() {
	tStartSection("Test ast switch statements");

	tAstSuccess(
		"int main() {\n"
		"	switch (thing) {\n"
		"		case 5: return 4;\n"
		"		default: return 6;\n"
		"	}\n"
		"}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_SWITCH, AST_IDENTIFIER,
			AST_STM, AST_COMP_STM, AST_STM, AST_LBL_CASE, AST_INT_CONSTANT, AST_RETURN,
			AST_INT_CONSTANT, AST_STM, AST_LBL_DEFAULT, AST_RETURN, AST_INT_CONSTANT,
			AST_UNKNOWN
		});

	tAstSuccess(
		"int main() {\n"
		"	switch (thing) {\n"
		"		case 6:\n"
		"		case 5: return 4;\n"
		"	}\n"
		"}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, 
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_SWITCH, AST_IDENTIFIER, 
			AST_STM, AST_COMP_STM, AST_STM, AST_LBL_CASE, AST_INT_CONSTANT,
			AST_LBL_CASE, AST_INT_CONSTANT, AST_RETURN, AST_INT_CONSTANT,
			AST_UNKNOWN,
		});

	tAstSuccess(
		"int main() {\n"
		"	switch (thing) {\n"
		"		case 3: return 6;\n"
		"	}\n"
		"}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_SWITCH, AST_IDENTIFIER,
			AST_STM, AST_COMP_STM, AST_STM, AST_LBL_CASE, AST_INT_CONSTANT,
			AST_RETURN, AST_INT_CONSTANT, AST_UNKNOWN,
		});
}

static void astWhileTest() {
	tStartSection("Testing whiles");

	tAstSuccess(
		"int main() {\n"
		"	while (1);\n"
		"}\n",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_WHILE, AST_INT_CONSTANT,
			AST_STM, AST_EMPTY_STM, AST_UNKNOWN,
		});

	tAstSuccess(
		"int main() {\n"
		"	while (*c) c++;\n"
		"}\n",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_WHILE,
			AST_PREFIX_OPERATION, AST_IDENTIFIER, AST_STM, AST_POSTFIX_OPERATION,
			AST_IDENTIFIER, AST_UNKNOWN,
		});

	tAstSuccess(
		"int main() {\n"
		"	while (*c) {\n"
		"		c++;\n"
		" }\n"
		"}\n",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_WHILE,
			AST_PREFIX_OPERATION, AST_IDENTIFIER, AST_STM, AST_COMP_STM, 
			AST_STM, AST_POSTFIX_OPERATION, AST_IDENTIFIER, AST_UNKNOWN,
		});

}

static void astDoWhileTest() {
	tStartSection("test do while");

	tAstSuccess(
		"int main() {\n"
		"	do; while (1);\n"
		"}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_DO_WHILE, AST_STM,
			AST_EMPTY_STM, AST_INT_CONSTANT, AST_UNKNOWN,
		});

	tAstSuccess(
		"int main() {\n"
		"	do stuff(); while (wait());\n"
		"}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_DO_WHILE, AST_STM,
			AST_FUNC_OPERATION, AST_IDENTIFIER, AST_FUNC_OPERATION,
			AST_IDENTIFIER, AST_UNKNOWN,
		});
}

static void astEmptyStmTest() {
	tStartSection("Test empty statements");

	tAstSuccess(
		"int main() {;;;}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_EMPTY_STM,
			AST_STM, AST_EMPTY_STM, AST_STM, AST_EMPTY_STM, AST_UNKNOWN,
		});
}

static void astGotoTest() {
	tStartSection("test goto statements");

	tAstSuccess(
		"int main() {\n"
		"	start: goto start;\n"
		"}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_LBL_IDENTIFIER,
			AST_GOTO, AST_UNKNOWN
		});

	tAstSuccess(
		"int main() {\n"
		"	start: start_2: goto start;\n"
		"	goto start_2;\n"
		"}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_LBL_IDENTIFIER,
			AST_LBL_IDENTIFIER, AST_GOTO, AST_STM, AST_GOTO, AST_UNKNOWN,
		});

	tAstSuccess(
		"int main() {\n"
		"	case 5: start: goto start;\n"
		"}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_LBL_CASE, AST_INT_CONSTANT,
			AST_LBL_IDENTIFIER, AST_GOTO, AST_UNKNOWN,
		});
}

static void astForTest() {
	tStartSection("Test for loop");

	tAstSuccess(
		"int main() {\n"
		"	for (;;);\n"
		"}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_FOR, AST_STM,
			AST_EMPTY_STM, AST_UNKNOWN,
		});

	tAstSuccess(
		"int main() {\n"
		"	int i = 0;\n"
		" for (i = 0; i++; i < 10) {\n"
		"		do_stuff(i);\n"
		"	}\n"
		"}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_DECLARATION, AST_TYPE_SPEC,
			AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_INT_CONSTANT, AST_STM,
			AST_FOR, AST_BINARY_OPERATION, AST_IDENTIFIER, AST_INT_CONSTANT,
			AST_POSTFIX_OPERATION, AST_IDENTIFIER, AST_BINARY_OPERATION, AST_IDENTIFIER,
			AST_INT_CONSTANT, AST_STM, AST_COMP_STM, AST_STM, AST_FUNC_OPERATION,
			AST_IDENTIFIER, AST_IDENTIFIER, AST_UNKNOWN,
		});

	tAstSuccess(
		"int main() {\n"
		" for (int i = 0; i++; i < 10) {\n"
		"		do_stuff(i);\n"
		"	}\n"
		"}",
		(ASTNodeType[]) {
			AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
			AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_FOR, AST_DECLARATION,
			AST_TYPE_SPEC, AST_DECLARATOR, AST_IDENTIFIER_DECL, AST_INT_CONSTANT, 
			AST_POSTFIX_OPERATION, AST_IDENTIFIER, AST_BINARY_OPERATION,
			AST_IDENTIFIER, AST_INT_CONSTANT, AST_STM, AST_COMP_STM, AST_STM,
			AST_FUNC_OPERATION, AST_IDENTIFIER, AST_IDENTIFIER, AST_UNKNOWN,
		});
}

static void astReturnTest() {
	tStartSection("test return things");

	tAstSuccess(
			"void test() {return;}",
			(ASTNodeType[]) {
				AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
				AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_RETURN, AST_UNKNOWN,
			});

	tAstSuccess(
			"int main() {return 5;}",
			(ASTNodeType[]) {
				AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL,
				AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_RETURN, AST_INT_CONSTANT,
				AST_UNKNOWN,
			});

	tAstSuccess(
			"int main() {return 4 * 2 >> thing;}",
			(ASTNodeType[]) {
				AST_FILE, AST_FUNC_DEF, AST_TYPE_SPEC, AST_DECLARATOR, AST_FUNC_DECL, 
				AST_IDENTIFIER_DECL, AST_COMP_STM, AST_STM, AST_RETURN,
				AST_BINARY_OPERATION, AST_BINARY_OPERATION, AST_INT_CONSTANT,
				AST_INT_CONSTANT, AST_IDENTIFIER, AST_UNKNOWN,
			});
}

void astTests() {
	astTestFile();
	astSimpleDecl();
	astDeclaratorTest();
	astInitializersTest();
	astConstantTest();
	astFuncOperationTest();
	astSubsOperationTest();
	astCondOperationTest();
	astCastOperationTest();
	astSizeofOperation();
	astBinaryOperationTest();
	astUnaryOperationTest();
	astTestIf();
	astSwitchTest();
	astWhileTest();
	astDoWhileTest();
	astEmptyStmTest();
	astGotoTest();
	astForTest();
	astReturnTest();
}
