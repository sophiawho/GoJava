#include "../ast/tree.h"
#include "../ast/stmt.h"
#include "codestmt.h"
#include "code.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void traverseExpForPrint(EXP *e, bool newLine, bool last) {
    if (e == NULL) return;
    traverseExpForPrint(e->next, newLine, false);
    generateINDENT(indent); fprintf(outputFile, "System.out.print(");
    generateEXP(e, false);
    fprintf(outputFile, ");\n");
    if (newLine && !last) {
        generateINDENT(indent); fprintf(outputFile, "System.out.print(\" \");\n");
    } 
}

void generateSTMT(STMT *s) {
    if (s == NULL) return;
    generateSTMT(s->next);
    switch (s->kind) {
        case k_stmtKindVarDecl:
            generateVarDecl(s->val.varDecl);
            break;
        case k_stmtKindPrint: ;
            bool newline = s->val.printStmt.newLine;
            traverseExpForPrint(s->val.printStmt.expList, newline, true);
            if (newline) {
                generateINDENT(indent); fprintf(outputFile, "System.out.println();\n");
            }
            break;
        default:
            break;
    }
}

// var a int = 3 becomes:
// Integer __golite__a = new Integer(3);
void generateVarDecl(VARSPEC *vs) {
    if (vs == NULL) return;
    generateVarDecl(vs->next);
    char *type = getStringFromType(vs->type);
    generateINDENT(indent); fprintf(outputFile, "%s %s", type, prepend(vs->ident->ident));
    if (vs->rhs != NULL) {
        fprintf(outputFile, " = new %s(", type);
        generateEXP(vs->rhs, false);
        fprintf(outputFile, ")");
    }
    fprintf(outputFile, ";\n");
    // TODO: multiple identifiers and expressions
    // TODO: arrays, slices, etc
    // TODO: if outside of a function, must prepend "public static" 
    // TODO: type is optional, right now we assume type is given
}

// If recurse = false, only print out current expression and ignore the rest of the expression list
void generateEXP(EXP *e, bool recurse) {
    if (e == NULL) return;
    if (recurse) generateEXP(e->next, recurse);
    switch (e->kind)
    {
        case k_expKindIdentifier:
            fprintf(outputFile, "%s", prepend(e->val.identExp.ident));
            break;
        case k_expKindIntLiteral:
            fprintf(outputFile, "%d", e->val.intLiteral);
            break;
        case k_expKindFloatLiteral:
            //  TODO format floats; print(0.12) should output //~+1.200000e-001
            fprintf(outputFile, "%f", e->val.floatLiteral);
            break;
        case k_expKindBoolLiteral:
            fprintf(outputFile, "%s", e->val.boolLiteral ? "true" : "false");
            break;
        case k_expKindRuneLiteral: // Interpret runes as integers
            fprintf(outputFile, "(int) '%c'", e->val.runeLiteral);
            break;
        case k_expKindStringLiteral: ;
            char *formatted = strdup(e->val.stringLiteral);
            formatted++;
            formatted[strlen(formatted)-1] = 0;
            fprintf(outputFile, "\"%s\"", formatted);
            break;
        default:
            break;
    }
}

char *getStringFromType(TYPE *t){
    if (t != NULL) {
        switch(t->kind) {
            case k_typeInt:
            case k_typeRune:
                return "Integer";
            case k_typeBool:
                return "Boolean";
            case k_typeString:
                return "String";
            case k_typeFloat:
                return "Double";
            default:
                return "Currently unsupported in `getStringFromType` func.";
        }
    }
    return "";
}