#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#include "macroDict.h"
#include "callbacks.h"
#include "dict.h"
#include "dstr.h"
#include "hash.h"
#include "tokList.h"
#include "util.h"

const int MACRO_DICT_INIT_SIZE = 16;

void initMacroDict(MacroDict *macroDict) {
	initDict(macroDict);
}

void freeMacroDict(MacroDict *macroDict) {
	freeDict(macroDict, (FreeFunc) freeStr, (FreeFunc) freeASTMacroDef);
}

int macroDictInsert(
		MacroDict *macroDict,
		char *key,
		ASTMacroDef value)
{
	return dictInsert(
			macroDict,
			&key,
			&value,
			sizeof(char*),
			sizeof(ASTMacroDef),
			(HashFunc) hashStrp,
			(CmpFunc) cmpStrp,
			(FreeFunc) freeStr,
			(FreeFunc) freeASTMacroDef);
}

void _insertFile(TokList *list, Token const *tok) {
	Token new;
	cpToken(&new, tok);
	new.type = TT_STR_CONSTANT;
	new.contents = realloc(new.contents, PATH_MAX);
	realpath(new.filename, new.contents);

	tokListApp(list, &new);
}

void _insertLine(TokList *list, Token const *tok) {
	Token new;
	cpToken(&new, tok);
	new.type = TT_NUMB_CONSTANT;
	int strLength = (int) log10((double) tok->posLine) + 2;
	new.contents = malloc(strLength * sizeof(char));
	snprintf(new.contents, strLength, "%d", new.posLine);

	tokListApp(list, &new);
}

void _insertDate(TokList *list, Token const *tok) {
	Token new;
	cpToken(&new, tok);
	new.type = TT_STR_CONSTANT;
	int strLength = strlen("mmm dd yyyy") + 1;
	new.contents = malloc(strLength * sizeof(char));

	time_t curTime = time(NULL);
	struct tm *curLoc = localtime(&curTime);

	char *months[12] = {
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec"
	};

	snprintf(
			new.contents,
			strLength,
			"%s %2d %4d",
			months[curLoc->tm_mon],
			curLoc->tm_mday,
			curLoc->tm_year + 1900);

	tokListApp(list, &new);
}

void _insertTime(TokList *list, Token const *tok) {
	Token new;
	cpToken(&new, tok);
	new.type = TT_STR_CONSTANT;
	int strLength = strlen("hh:mm:ss") + 1;
	new.contents = malloc(strLength * sizeof(char));

	time_t curTime = time(NULL);
	struct tm *curLoc = localtime(&curTime);

	snprintf(
			new.contents,
			strLength,
			"%02d:%02d:%02d",
			curLoc->tm_hour,
			curLoc->tm_min,
			curLoc->tm_sec);

	tokListApp(list, &new);
}

void macroDictInsertDefault(MacroDict *macros) {
	ASTMacroDef macro;

	initASTMacroDefDefault(&macro, strdup("__FILE__"), _insertFile);
	macroDictInsert(macros, strdup("__FILE__"), macro);

	initASTMacroDefDefault(&macro, strdup("__LINE__"), _insertLine);
	macroDictInsert(macros, strdup("__LINE__"), macro);

	initASTMacroDefDefault(&macro, strdup("__DATE__"), _insertDate);
	macroDictInsert(macros, strdup("__DATE__"), macro);

	initASTMacroDefDefault(&macro, strdup("__TIME__"), _insertTime);
	macroDictInsert(macros, strdup("__TIME__"), macro);
}

int macroDictPresent(const MacroDict *macroDict, char const *key) {
	return dictPresent(macroDict, &key, (HashFunc) hashStrp, (CmpFunc) cmpStrp);
}

ASTMacroDef const *macroDictGet(const MacroDict *macroDict, char const *key) {
	return dictGet(macroDict, &key, (HashFunc) hashStrp, (CmpFunc) cmpStrp);
}

ASTMacroDef *macroDictGetm(MacroDict *macroDict, char const *key) {
	return dictGetm(macroDict, &key, (HashFunc) hashStrp, (CmpFunc) cmpStrp);
}

void macroDictDelete(MacroDict *macroDict, char const *key) {
	dictDelete(macroDict, &key, (HashFunc) hashStrp, (CmpFunc) cmpStrp, (FreeFunc) freeStr, (FreeFunc) freeASTMacroDef);
}

ASTMacroDef *macroDictRemove(MacroDict *macroDict, char const *key) {
	return dictRemove(macroDict, &key, (HashFunc) hashStrp, (CmpFunc) cmpStrp, (FreeFunc) freeStr);
}

int printMacroDictV(const MacroDict *dict) {
	return printDictV(dict, (PrintFunc) printJsonStr, (PrintFunc) printASTMacroDef);
}

int printMacroDict(MacroDict const *dict) {
	return printDict(dict, (PrintFunc) printJsonStr, (PrintFunc) printASTMacroDef);
}
