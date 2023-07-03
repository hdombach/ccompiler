#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "lines.h"

const int LINES_INITIAL_CAP = 2;

Lines initLines() {
	Lines result;

	result.size = 0;
	result.capacity = LINES_INITIAL_CAP;
	result.data = malloc(sizeof(char *) * result.capacity);

	return result;
}

void freeLines(Lines *lines) {
	for (int i = 0; i < lines->size; i++) {
		free(lines->data[i]);
	}
	free(lines->data);
}

int linesFromFile(Lines *result, FILE *f) {
	char *curLine;

	if (!f) {
		return LINES_INVALID_ARGUMENT;
	}

	*result = initLines();

	while (getline(&curLine, NULL, f) > 0) {
		linesApp(result, curLine);
	}

	free(curLine);

	return 0;
}

void linesApp(Lines *lines, const char *line) {
	size_t lineLen;

	if (lines->size >= lines->capacity) {
		linesIncCap(lines);
	}
	
	lineLen = sizeof(char) * (strlen(line) + 1);
	lines->data[lines->size] = malloc(lineLen);
	strcpy(lines->data[lines->size], line);
	lines->size++;
}

void linesIncCap(Lines *lines) {
	size_t newLen;

	lines->capacity *= 2;
	newLen = lines->capacity * sizeof(char*);
	lines->data = realloc(lines->data, newLen);
}

void linesDecCap(Lines *lines) {
	size_t newLen;

	if (lines->capacity > 2) {
		lines->capacity /= 2;
		newLen =  lines->capacity * sizeof(char*);
		lines->data = realloc(lines->data, newLen);
	}
}

int linesRem(Lines *lines, int index) {
	size_t mvLen;

	if (index < 0 || index >= lines->size) {
		return LINES_INVALID_INDEX;
	}

	free(lines->data[index]);
	mvLen = (lines->size - 1 - index) * sizeof(char *);
	memmove(lines->data + index, lines->data + index + 1, mvLen);
	lines->size--;
	if (lines->size * 2 <= lines->capacity) {
		linesDecCap(lines);
	}

	return LINES_SUCCESS;
}

int linesIns(Lines *lines, const char *line, int index) {
	size_t mvLen;
	size_t lineLen;

	if (index < 0 || index > lines->size) {
		return LINES_INVALID_INDEX;
	}
	if (lines->size >= lines->capacity) {
		linesIncCap(lines);
	}

	lineLen = sizeof(char) * (strlen(line) + 1);
	mvLen = (lines->size - index) * sizeof(char *);
	memmove(lines->data + index + 1, lines->data + index, mvLen);
	lines->data[index] = malloc(lineLen);
	strcpy(lines->data[index], line);
	lines->size++;

	return LINES_SUCCESS;
}

int linesInsLines(Lines *lines, const Lines *newLines, int index) {
	size_t mvLen;
	size_t lineLen;

	if (index < 0 || index > lines->size) {
		return LINES_INVALID_INDEX;
	}
	while ((lines->size + newLines->size) > lines->capacity) {
		linesIncCap(lines);
	}

	mvLen = (lines->size - index) * sizeof(char *);
	memmove(lines->data + index + newLines->size, lines->data + index, mvLen);
	for (int i = 0; i < newLines->size; i++) {
		lineLen = sizeof(char) * (strlen(newLines->data[i]) + 1);
		lines->data[index + i] = malloc(lineLen);
		strcpy(lines->data[index + i], newLines->data[i]);
	}
	lines->size += newLines->size;

	return LINES_SUCCESS;
}
