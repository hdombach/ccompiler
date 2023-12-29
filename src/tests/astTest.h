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
	tAstDebug(
			"int;\n"
			"char c;");

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

void astTests() {
	astTestFile();
}
