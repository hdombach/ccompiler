#pragma once

/**
 * @file
 *
 * Provides functions to parse all_tests args and load the result into TestArgs
 */

/**
 * @brief parseTestArgs result
 *
 * Contains all the information contained in the cmd-line args but in a 
 * friendlier format
 */
typedef struct {
	/** @brief Whether the help message should be shown */
	int help;
	/** @brief The verbosity level */
	int verbose;
} TestArgs;

/** @brief A global instance of the parsed args */
extern TestArgs gTestArgs;

extern const char *TEST_ARGS_HELP_MSG;

/**
 * @param[out] args
 */
void initTestArgs(TestArgs *args);

/**
 * @param[in] args
 */
void freeTestArgs(TestArgs *args);

/**
 * @brief Parse args for all_tests
 * @param[in] argc Arg count
 * @param[in] argv List of args
 * @param[out] args
 *
 * Parses the args passed directly by main.  
 * Assumes that the first argv element is the name of the program
 */
int parseTestArgs(int argc, char **argv, TestArgs *args);

/**
 * @brief Debug print args
 * @param[in] args
 * @return The number of printed characters
 */
int testArgsPrint(TestArgs const *args);
