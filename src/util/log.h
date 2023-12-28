#pragma once

#include <stdarg.h>
#include <stdio.h>
#include "../token.h"

struct Token;

typedef enum CError {
	CERR_UNKNOWN = 0,
	CERR_TOKENIZER = 1,
} CError;

void initCerr();
void freeCerr();
void cerrDisablePrint();
int cerrCount();
CError const *getCerr();
void logCerr(CError err, const struct Token *tok, const char *fmt, ...);
const char *cerrStr(CError err);

extern const int isLogDebug;

void logDebug(const char *file, int line, const char *label, const char *fmt, ...);
#define DEBUG_MSG(msg, ...) \
	logDebug(__FILE__, __LINE__, "DEBUG", msg __VA_OPT__(,) __VA_ARGS__)

void logDebugTok(const char *file, int line, const Token *tok, const char *fmt, ...);
#define DEBUG_TOK(tok, msg, ...) \
	logDebugTok(__FILE__, __LINE__, tok, msg __VA_OPT__(,) __VA_ARGS__)

int logAssert(int exp, char *file, int line, char *expStr);
#define ASSERT(exp) \
	logAssert(exp, __FILE__, __LINE__, #exp)

void logTodo(const char *file, int line, const char *fmt, ...);
#define TODO(msg, ...) \
	logTodo(__FILE__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__)

void logTestFailed(const char *file, int line, const char *msg, const char *exp);
