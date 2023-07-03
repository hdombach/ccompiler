#ifndef LINES_H
#define LINES_H

#include <stdio.h>

typedef struct {
	char **data;
	int size;
	int capacity;
} Lines;

typedef enum {
	LINES_SUCCESS = 0,
	LINES_INVALID_ARGUMENT = 1,
	LINES_INVALID_INDEX = 2,
} LinesError;

extern char * linesErrorMsg;

Lines initLines();
void freeLines(Lines *lines);
/*
 * Initiliazes a lines struct from a file pointer
 * returns LinesError
 */
int linesFromFile(Lines *lines, FILE *f);
/*
 * Append a line to lines
 */
void linesApp(Lines *lines, const char *line);
/*
 * Increases the capacity of lines
 */
void linesIncCap(Lines *lines);
/*
 * Decrease the capacity of lines
 */
void linesDecCap(Lines *lines);

/*
 * Removes a line at index
 */
int linesRem(Lines *lines, int index);

/*
 * Inserts a line at the index
 */
int linesIns(Lines *lines, const char *line, int index);

#endif
