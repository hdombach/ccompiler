#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "../token.h"
#include "../util/color.h"

#ifdef DEBUG
const int isLogDebug = 1;
#else
const int isLogDebug = 0;
#endif

void logErrHead(FILE *file, const char *fmt, ...) {
	fprintf(file, "[" COLOR_RED);

	va_list args;
	va_start(args, fmt);
	vfprintf(file, fmt, args);
	va_end(args);

	fprintf(file, COLOR_RESET "] ");
}

void logDebugHead(FILE *file, const char *fmt, ...) {
	fprintf(file, "[" COLOR_BLUE);

	va_list args;
	va_start(args, fmt);
	vfprintf(file, fmt, args);
	va_end(args);

	fprintf(file, COLOR_RESET "] ");
}



int _cerrCount = 0;
int _cerrShouldPrint = 1;
CError *_cerrs = NULL;

void initCerr() {
	_cerrShouldPrint = 1;
	freeCerr();
}

void freeCerr() {
	if (_cerrs) free(_cerrs);
	_cerrs = NULL;
	_cerrCount = 0;
}

void cerrDisablePrint() {
	_cerrShouldPrint = 0;
}

int cerrCount() {
	return _cerrCount;
}

CError const *getCerr() {
	return _cerrs;
}

void logCerr(CError err, const struct Token *tok, const char *fmt, ...) {
	if (_cerrShouldPrint) {
		if (tok) {
			logErrHead(stderr, "ERROR %s:%d,%d", tok->filename, tok->posLine, tok->posColumn);
		} else {
			logErrHead(stderr, "ERROR");
		}

		fprintf(stderr, "%s: ", cerrStr(err));

		va_list args;
		va_start(args, fmt);
		vfprintf(stderr, fmt, args);
		va_end(args);

		fprintf(stderr, "\n");
	}

	_cerrs = realloc(_cerrs, (_cerrCount + 1) * sizeof(CError));
	_cerrs[_cerrCount] = err;
	_cerrCount++;
}

const char *cerrStr(CError err) {
	return (const char *[]){
		"unknown",
		"tokenizer",
	}[err];
}

void logDebug(const char *file, int line, const char *label, const char *fmt, ...) {
	logDebugHead(stderr, "%s %s:%d", label, file, line);

	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fprintf(stderr, "\n");
}

void logDebugTok(const char *file, int line, const struct Token *tok, const char *fmt, ...) {
	logDebugHead(stderr, "DEBUG %s:%d,%d", tok->filename, tok->posLine, tok->posColumn);

	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fprintf(stderr, "\n");
}

int logAssert(int exp, char *file, int line, char *expStr) {
	if (!exp) {
		logErrHead(stderr, "ASSERT FAILED");
		fprintf(stderr, "%s:%d (%s)\n", file, line, expStr);
	}
	return exp;
}

void logTodo(const char *file, int line, const char *fmt, ...) {
	logErrHead(stderr, "TODO %s:%d", file, line);

	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fprintf(stderr, "\n");
}

void logTestFailed(
		const char *file,
		int line,
		const char *section,
		const char *msg,
		const char *exp)
{
	if (file) {
		logErrHead(stderr, "TEST FAILED %s:%d", file, line);
	} else {
		logErrHead(stderr, "TEST FAILED");
	}

	if (exp) {
		fprintf(stderr, "%s ", exp);
	}

	if (msg) {
		fprintf(stderr, "(%s:%s)\n", section, msg);
	} else {
		fprintf(stderr, "(%s)\n", section);
	}
}
