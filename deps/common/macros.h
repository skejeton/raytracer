#pragma once

/*
 * Macro to easily understand what type thing is of, syntax highlighted comment
 */
#define OF(...)

/*
 * Max range of type
 */
#define MAX_OF(type)                                                          \
    (((type)(~0LLU) > (type)((1LLU << ((sizeof(type) << 3) - 1)) - 1LLU))     \
         ? (long long unsigned int)(type)(~0LLU)                              \
         : (long long unsigned int)(type)(                                    \
             (1LLU << ((sizeof(type) << 3) - 1)) - 1LLU))
