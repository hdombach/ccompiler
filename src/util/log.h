#pragma once

#include <stdarg.h>
#include <stdio.h>
#include "../token.h"

struct Token;

void logTokErr(const struct Token *tok, const char *fmt, ...);
void logTokDebug(const struct Token *tok, const char *fmt, ...);
