#pragma once

int printStr(char const *str);
void freeStr(char **str);
int cmpStr(char const *a, char const *b);
int cmpStrp(char const **a, char const **b);
void cpStrp(char **dest, char const **src);
int printIntp(int const *value);

int printJsonStr(char const *str);
int printrStr(char const *str);
int printJsonStrp(char const **str);

#define COUNTOF(LIST) (sizeof(LIST) / sizeof(LIST[0]))
