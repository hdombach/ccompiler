#pragma once

#include <stdarg.h>
#include <stdio.h>
#include "../token.h"

struct Token;

/**
 * @brief internal errors which mean I did something wrong
 */
void logErrHead(const char *fmt, ...);
void logErrTok(const struct Token *tok, const char *fmt, ...);
void logErr(const char *head, const char *fmt, ...);

extern const int isLogDebug;

void logDebugHead(const char *fmt, ...);
void logDebugTok(const struct Token *tok, const char *fmt, ...);
void logDebug(const char *head, const char *fmt, ...);

int logAssert(int exp, char *file, int line, char *expStr);

#define LOG_ASSERT(exp) \
	logAssert(exp, __FILE__, __LINE__, #exp)

#define TODO(msg) \
{ \
	logErrHead("TODO %s:%d", __FILE__, __LINE__); \
	fprintf(stderr, "%s\n", msg); \
}
