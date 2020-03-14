#ifndef ERROR_H
#define ERROR_H
void throwError(char *description, int lineno);

void throwErrorUndefinedId(int lineno, char *id);
void throwErrorRedeclaredId(int lineno, char *id);
void throwInternalError(char *description);

#endif