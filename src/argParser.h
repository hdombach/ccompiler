#pragma once

#include "util/wordList.h"

/**
 * @file
 *
 * Provides functions to parse ccompiler args and load the result into Args
 */

/**
 * @brief parseArgs result
 *
 * Contains all the information contained in the cmd-lines args but in a
 * friendly format
 * Must be initialized with @ref initArgs before using
 */
typedef struct {
	/** @brief List of files to compile */
	WordList files;
	/** @brief Whether the help message should be shown */
	int help;
	/** @brief The verbosity level */
	int verbose;
} Args;

/** @brief A global instance of the parsed args */
extern Args g_args;

/** @brief The help message to pring */
extern const char *ARGS_HELP_MSG;

/** 
 * @param[out] args
 */
void initArgs(Args *args);
/**
 * @param[in] args
 */
void freeArgs(Args *args);

/**
 * @brief Parse args for ccompiler
 * @param[in] argc Arg count
 * @param[in] argv List of args
 * @param[out] args
 *
 * Parses the args passed directly by main.  
 * Assumes that the first argv element is the name of the program
 */
int parseArgs(int argc, char **argv, Args *args);

/**
 * @brief Debug print args
 * @param[in] args
 */
int argsPrint(Args const *args);
