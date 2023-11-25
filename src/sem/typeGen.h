#pragma once

#include "../ast/file.h"
#include "../util/wordList.h"
#include "scope.h"

typedef struct TypeTravCtx {
	int errorCount;
	ASTScope *scope;
	ASTNode *node;
} TypeTravCtx;

void typeTravAddErr(TypeTravCtx *ctx, const char *errName);

void typeGen(ASTFile *file);
