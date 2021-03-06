#ifndef ERROR_H
#define ERROR_H
void throwError(char *description, int lineno);

void throwSpecialFunctionDeclarationError(char *id, int lineno);
void throwSpecialFunctionParameterError(char *id, int lineno);
void throwErrorUndefinedId(int lineno, char *id);
void throwErrorRedeclaredId(int lineno, char *id);
void throwInternalError(char *description, int lineno);

#endif