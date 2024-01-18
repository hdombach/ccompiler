#pragma once

#include "../ast/file.h"
#include "../util/wordList.h"
#include "scope.h"

typedef struct TypeTravCtx {
	int errorCount;
	ASTScope *scope;
	ASTNode *node;
} TypeTravCtx;

void typeGen(ASTFile *file);
