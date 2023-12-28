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
	tAstDebug("int;");
	tAstSuccess(
			"int;",
			(ASTNodeType[]) {AST_FILE, AST_DECLARATION, AST_TYPE_SPEC, AST_UNKNOWN});
}

void astTests() {
	astTestFile();
}
