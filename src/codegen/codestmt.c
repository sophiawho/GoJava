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

void generateAssignStmt(AssignKind kind, EXP *lhs, EXP *rhs) {
    if (kind == k_stmtAssign) {
        generateINDENT(indent);
        if (lhs->kind == k_expKindArrayAccess && lhs->val.arrayAccess.arrayReference->type->kind == k_typeSlice) {
            char *arrayIdent = lhs->val.arrayAccess.arrayReference->val.identExp.ident;
            EXP *indexExp = lhs->val.arrayAccess.indexExp;  
            fprintf(outputFile, "%s.put(", prepend(arrayIdent));
            generateEXP(indexExp, false);
            fprintf(outputFile, ", ");
            generateEXP(rhs, false);
            fprintf(outputFile, ")");
        } else {
            // If regular assign stmt: [indendation] lhs = rhs
            generateEXP(lhs, false);
            fprintf(outputFile, " = ");
            generateEXP(rhs, false);
        }
        fprintf(outputFile, ";\n");
    }
}

void generateSTMT(STMT *s) {
    if (s == NULL) return;
    generateSTMT(s->next);
    switch (s->kind) {
        case k_stmtKindAssign:
            generateAssignStmt(s->val.assignStmt.kind, s->val.assignStmt.lhs, s->val.assignStmt.rhs);
            break;
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
    if (vs->type != NULL) {
        if (vs->type->kind == k_typeArray) {
            char *type = getStringFromType(vs->type->val.arrayType.type, true);
            generateINDENT(indent); fprintf(outputFile, "%s[] %s = new %s[%d]", type, prepend(vs->ident->ident), type, vs->type->val.arrayType.size);
        } else if (vs->type->kind == k_typeSlice) {
            char *type = getStringFromType(vs->type->val.sliceType.type, false);
            generateINDENT(indent); fprintf(outputFile, "Slice<%s> %s = ", type, prepend(vs->ident->ident));
            if (vs->rhs != NULL && vs->rhs->kind == k_expKindAppend) {
                generateEXP(vs->rhs, false);
            } else {
                fprintf(outputFile, " new Slice<>()");
            }
        } else {
            char *type = getStringFromType(vs->type, true);
            generateINDENT(indent); fprintf(outputFile, "%s %s", type, prepend(vs->ident->ident));
            if (vs->rhs != NULL) {
                fprintf(outputFile, " = ");
                generateEXP(vs->rhs, false);
            }
        }
    } else {
        generateINDENT(indent); fprintf(outputFile, "Type has not been specified. This is not yet supported by codegen.");
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
        case k_expKindLen: ;
            char *lenIdent = e->val.lenExp->val.identExp.ident;
            if (e->val.lenExp->type->kind == k_typeSlice) {
                fprintf(outputFile, "%s.len", prepend(lenIdent));
            } else if (e->val.lenExp->type->kind == k_typeArray) {
                // TODO ARRAY
                fprintf(outputFile, "%s.length", prepend(lenIdent));
            }
            break;
        case k_expKindCap: ;
            char *capIdent = e->val.capExp->val.identExp.ident;
            if (e->val.capExp->type->kind == k_typeSlice) {
                fprintf(outputFile, "%s.cap", prepend(capIdent));
            } else if (e->val.capExp->type->kind == k_typeArray) {
                fprintf(outputFile, "%s.length", prepend(capIdent));
            }
            break;
        case k_expKindArrayAccess: ;
            char *arrayIdent = e->val.arrayAccess.arrayReference->val.identExp.ident;
            EXP *indexExp = e->val.arrayAccess.indexExp;
            if (e->val.arrayAccess.arrayReference->type->kind == k_typeSlice) {
                fprintf(outputFile, "%s.get(", prepend(arrayIdent));
                generateEXP(indexExp, false);
                fprintf(outputFile, ")");
            } else if (e->val.arrayAccess.arrayReference->type->kind == k_typeArray) {
                fprintf(outputFile, "%s[", prepend(arrayIdent));
                generateEXP(indexExp, false);
                fprintf(outputFile, "]");
            }
            break;
        case k_expKindAppend: ;
            char *sliceIdent = e->val.append.slice->val.identExp.ident;
            EXP *addend = e->val.append.addend;
            fprintf(outputFile, "%s.append(", prepend(sliceIdent));
            generateEXP(addend, false);
            fprintf(outputFile, ")");
        default:
            break;
    }
}

char *getStringFromType(TYPE *t, bool isPrimitive){
    if (t != NULL) {
        switch(t->kind) {
            case k_typeInt:
            case k_typeRune:
                return isPrimitive ? "int" : "Integer";
            case k_typeBool:
                return isPrimitive ? "bool" : "Boolean";
            case k_typeString:
                return "String";
            case k_typeFloat:
                return isPrimitive ? "double" : "Double";
            default:
                return "Currently unsupported in `getStringFromType` func.";
        }
    }
    return "";
}