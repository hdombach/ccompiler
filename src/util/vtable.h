#pragma once

#include "callbacks.h"

/**
 * @brief Defines general vtable
 */

/**
 * @brief A general vtable becuase classes are very conveniant
 */
typedef struct VTable {
	FreeFunc freeFunc;
	PrintFunc printFunc;
} VTable;
