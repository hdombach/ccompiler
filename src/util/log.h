#pragma once

#include <stdarg.h>
#include <stdio.h>
#include "../token.h"

struct Token;

void logTokErr(const struct Token *tok, const char *fmt, ...);
/**
 * @brief internal errors which mean I did something wrong
 */
void logTokIntError(const struct Token *tok, const char *fmt, ...);
void logTokDebug(const struct Token *tok, const char *fmt, ...);

void logIntError(const char *fmt, ...);

int logAssert(int exp, char *file, int line, char *expStr);

#define LOG_ASSERT(exp) \
	logAssert(exp, __FILE__, __LINE__, #exp)

#define TODO(msg) \
	logIntError("%s:%d Unfinished todo: %s", __FILE__, __LINE__, msg)
