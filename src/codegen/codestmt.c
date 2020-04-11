#include "../ast/tree.h"
#include "../ast/stmt.h"
#include "codestmt.h"
#include "code.h"
#include "../typecheck/typecheck.h"
#include "../error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void traverseExpForPrint(EXP *e, bool newLine, bool last) {
    if (e == NULL) return;
    traverseExpForPrint(e->next, newLine, false);

    generateINDENT(indent);

    // Format floats in scientific notation:
    // 6 decimal digits, 3 mantissa digits, positive or negative prefixes for base and mantissa
    if (e->type->kind == k_typeFloat) 
    {
        fprintf(outputFile, "System.out.printf(");
        fprintf(outputFile, "\"%%+.6e\", ");
        generateEXP(e, false);
    }
    else 
    {
        fprintf(outputFile, "System.out.print(");
        generateEXP(e, false);
    }

    fprintf(outputFile, ");\n");

    if (newLine && !last) {
        generateINDENT(indent); fprintf(outputFile, "System.out.print(\" \");\n");
    } 
}

void generateAssignStmt(AssignKind kind, EXP *lhs, EXP *rhs) {

    switch (kind)
    {
    case k_stmtAssign: ;
        int temp_counter_start = temp_counter;
        char temp_variable[80];
        while (rhs != NULL) {
            sprintf(temp_variable, "__golite__tmp_%d", temp_counter++);
            // print LHS of temp variable expression
            generateINDENT(indent);
            if (rhs->type->kind == k_typeArray) {
                char *type = getStringFromType(rhs->type->val.arrayType.type, true);
                fprintf(outputFile, "%s[] %s = ", type, temp_variable);
            } else if (rhs->type->kind == k_typeSlice) {
                char *type = getStringFromType(rhs->type->val.sliceType.type, false);
                fprintf(outputFile, "Slice<%s> %s = ", type, temp_variable);
            } else if (rhs->type->kind == k_typeStruct) {
                char *type = getStringFromType(rhs->type, true);
                fprintf(outputFile, "%s %s = ", type, temp_variable);
            } else {
                char *type = getStringFromType(rhs->type, true);
                fprintf(outputFile, "%s %s = ", type, temp_variable);
            }
            generateEXP(rhs, false);
            rhs = rhs->next;
            fprintf(outputFile, ";\n");
        }
        while (lhs != NULL) {
            sprintf(temp_variable, "__golite__tmp_%d", temp_counter_start++);
            if (lhs->kind == k_expKindIdentifier && strcmp(lhs->val.identExp.ident, "_") == 0) {
                lhs = lhs->next;
                continue;
            }
            generateINDENT(indent);
            if (lhs->kind == k_expKindArrayAccess && lhs->val.arrayAccess.arrayReference->type->kind == k_typeSlice) {
                char *arrayIdent = lhs->val.arrayAccess.arrayReference->val.identExp.ident;
                EXP *indexExp = lhs->val.arrayAccess.indexExp;  
                fprintf(outputFile, "%s.put(", prepend(arrayIdent));
                generateEXP(indexExp, false);
                fprintf(outputFile, ", ");
                fprintf(outputFile, "%s", temp_variable);
                fprintf(outputFile, ")");
            } else {
                generateEXP(lhs, false);
                fprintf(outputFile, " = ");
                fprintf(outputFile, "%s", temp_variable);
            }
            lhs = lhs->next;
            fprintf(outputFile, ";\n");
        }
        fprintf(outputFile, "\n");
        break;
    
    case k_stmtColonAssign:
        while(lhs != NULL)
        {
            // We output one identifier at a time
            // TODO This needs to be fixed
            IDENT *id = makeIDENT(lhs->val.identExp.ident);
            id->next = NULL;

            if (id != NULL && rhs != NULL && lhs->type != NULL) 
            {
                VARSPEC *vs = makeVarSpec(id, rhs, lhs->type);
                vs->next = NULL;
                generateVarDecl(vs);
            }
            lhs = lhs->next;
            rhs = rhs->next;
        }
        break;
    
    default:
        throwInternalError("Not implemented yet", 0);
        break;
    }
}

void generateSTMT(STMT *s) {
    if (s == NULL) return;
    generateSTMT(s->next);

    switch (s->kind) {
        case k_stmtKindEmpty:
            // do nothing, can be removed
            break;
        case k_stmtKindExpStmt:
            generateINDENT(indent);
            generateEXP(s->val.expStmt, true);
            fprintf(outputFile, ";\n");
            break;
        case k_stmtKindIncDec:
            generateINDENT(indent);
            generateEXP(s->val.incDecStmt.exp, false);
            if (s->val.incDecStmt.amount == 1) fprintf(outputFile, "++;\n");
            else fprintf(outputFile, "--;\n");
            break;
        case k_stmtKindAssign:
            generateAssignStmt(s->val.assignStmt.kind, s->val.assignStmt.lhs, 
                s->val.assignStmt.rhs);
            break;
        case k_stmtKindPrint: ;
            bool newline = s->val.printStmt.newLine;
            traverseExpForPrint(s->val.printStmt.expList, newline, true);
            if (newline) {
                generateINDENT(indent); fprintf(outputFile, "System.out.println();\n");
            }
            break;
        case k_stmtKindVarDecl:
            generateVarDecl(s->val.varDecl);
            break;
        case k_stmtKindTypeDecl:
            generateTYPESPEC(s->val.typeDecl);
            break;
        case k_stmtKindBlock:
            generateINDENT(indent);
            fprintf(outputFile, "{\n");
            indent++;
            generateSTMT(s->val.blockStmt);
            indent--;
            generateINDENT(indent);
            fprintf(outputFile, "}\n");
            break;
        case k_stmtKindIfStmt:
            if (s->val.ifStmt.simpleStmt != NULL) 
            {
                generateINDENT(indent);
                fprintf(outputFile, "{\n");
                indent++;

                generateSTMT(s->val.ifStmt.simpleStmt);
            }

            generateINDENT(indent);

            fprintf(outputFile, "if (");
            generateEXP(s->val.ifStmt.condition, false);
            fprintf(outputFile, ") \n");

            // Block Statment generation takes care of curly brackets, indentation
            // and new lines
            generateSTMT(s->val.ifStmt.trueBody);

            if (s->val.ifStmt.falseBody != NULL)
            {
                generateINDENT(indent);
                fprintf(outputFile, "else \n");

                generateSTMT(s->val.ifStmt.falseBody);
            }

            if (s->val.ifStmt.simpleStmt != NULL) 
            {
                indent--;
                generateINDENT(indent);
                fprintf(outputFile, "}\n");
            }
            break;
            
        case k_stmtKindSwitch:
            // TODO
            break;

        case k_stmtKindFor: 

            if (s->val.forLoop.initStmt != NULL)
            {
                generateINDENT(indent);
                fprintf(outputFile, "{\n");
                indent++;
                generateINDENT(indent);
                fprintf(outputFile, "//Init statements\n");
                generateSTMT(s->val.forLoop.initStmt);
                
            }

            generateINDENT(indent);
            fprintf(outputFile, "while (");

            if (s->val.forLoop.condition != NULL) generateEXP(s->val.forLoop.condition, false);
            else fprintf(outputFile, "true");

            fprintf(outputFile, ") \n");

            if (s->val.forLoop.postStmt != NULL)
            {
                STMT *body = s->val.forLoop.postStmt;
                STMT *next = body;
                while (next->next != NULL) next = next->next;
                next->next = s->val.forLoop.body->val.blockStmt;

                generateINDENT(indent);
                fprintf(outputFile, "{\n");

                indent++;
                generateSTMT(body);
                indent--;

                generateINDENT(indent);
                fprintf(outputFile, "}\n");
            }
            else
            {
                generateSTMT(s->val.forLoop.body);
            }

            if (s->val.forLoop.initStmt != NULL)
            {
                indent--;
                generateINDENT(indent);
                fprintf(outputFile, "}\n");
            }
            break;

        case k_stmtKindBreak:
            generateINDENT(indent);
            fprintf(outputFile, "break;\n");
            break;
        case k_stmtKindContinue:
            generateINDENT(indent);
            fprintf(outputFile, "continue;\n");
            break;
        case k_stmtKindReturn:
            if (s->val.returnExp != NULL)
            {
                generateINDENT(indent);
                fprintf(outputFile, "return ");
                generateEXP(s->val.returnExp, false);
                fprintf(outputFile, ";\n");
            }
            break;

        default:
            break;
    }
}

void generateZeroValue(TYPE *t) {
    switch (t->kind) {
        case k_typeBool:
            fprintf(outputFile, "false");
            break;
        case k_typeString:
            fprintf(outputFile, "\"\"");
            break;
        case k_typeRune:
        case k_typeInt:
            fprintf(outputFile, "0");
            break;
        case k_typeFloat:
            fprintf(outputFile, "0.0");
            break;
        default:
            break;
    }
}

// TODO: if outside of a function, must prepend "public static" [check indent level]
// TODO: blank identifers
void generateVarDecl(VARSPEC *vs) {
    if (vs == NULL) return;
    generateVarDecl(vs->next);
    if (vs->type == NULL) {
        generateINDENT(indent); fprintf(outputFile, "Type has not been specified. There was an error in typecheck.");
        return;
    }
    IDENT *curIdent = vs->ident;
    EXP *e = vs->rhs == NULL ? NULL : vs->rhs; // Refactor
    while (curIdent != NULL) {
        if (vs->type->kind == k_typeArray) {
            char *type = getStringFromType(vs->type->val.arrayType.type, true);
            generateINDENT(indent); fprintf(outputFile, "%s[] %s = new %s[%d]", type, prepend(curIdent->ident), type, vs->type->val.arrayType.size);
        } else if (vs->type->kind == k_typeSlice) {
            char *type = getStringFromType(vs->type->val.sliceType.type, false);
            generateINDENT(indent); fprintf(outputFile, "Slice<%s> %s = ", type, prepend(curIdent->ident));
            if (vs->rhs != NULL && vs->rhs->kind == k_expKindAppend) {
                generateEXP(vs->rhs, false);
            }
            else 
            {
                fprintf(outputFile, " new Slice<>()");
            }
        } else if (vs->type->kind == k_typeStruct) {
            char *type = getStringFromType(vs->type, true);
            generateINDENT(indent); 

            // Structs are implemented as Classes in Java, so we need to allocate memory with `new` and
            // construct the variable
            fprintf(outputFile, "%s %s = new %s()", type, prepend(curIdent->ident), type);
        } else {
            char *type = getStringFromType(vs->type, true);
            generateINDENT(indent); fprintf(outputFile, "%s %s", type, prepend(curIdent->ident));
            fprintf(outputFile, " = ");
            if (e != NULL) {
                generateEXP(e, false);
                e = e->next;
            } else {
                generateZeroValue(resolveType(vs->type));
            }
        }
        fprintf(outputFile, ";\n");
        curIdent = curIdent->next;
    }
}

// Generate type declarations (NOTE: only for struct types!) and input parameters
void generateTYPESPEC(TYPESPEC *ts)
{
    if (ts == NULL) return;
    generateTYPESPEC(ts->next);

    switch (ts->kind)
    {
    case k_typeSpecKindTypeDeclaration:
        // The only type required to emit is struct types
        // TODO
        if (ts->type->kind == k_typeStruct)
        {
            generateINDENT(indent);
            // Structs are implemented as Classes in Java
            fprintf(outputFile, "class %s {\n", prepend(ts->ident->ident));
            indent++;
            
            generateSTRUCTSPEC(ts->type->val.structType);
            
            indent--;
            generateINDENT(indent);
            fprintf(outputFile, "}\n");
        }
        break;
    
    case k_typeSpecKindParameterList:
        // Need to write as a loop for commas
        for (IDENT *id = ts->ident; id; id = id->next)
        {
            fprintf(outputFile, "%s ", getStringFromType(ts->type, true)); 
            if (id->next != NULL) fprintf(outputFile, "%s, ", id->ident);
            else fprintf(outputFile, "%s;\n", id->ident);
        }
        break;
    }
}

// Generate a struct's attribute and type specifiers. This is to be used within the 
// context of emitting the declaration of a struct type
void generateSTRUCTSPEC(STRUCTSPEC *ss)
{
    if (ss == NULL) return;
    generateSTRUCTSPEC(ss->next);

    generateINDENT(indent);

    // There is only 1 TYPE per STRUCTSPEC
    fprintf(outputFile, "%s ", getStringFromType(ss->type, true));

    for (IDENT *id = ss->attribute; id; id=id->next)
    {
        if (id->next != NULL) fprintf(outputFile, "%s, ", id->ident);
        else fprintf(outputFile, "%s;\n", id->ident);
    }
}

// If recurse = false, only print out current expression and ignore the rest of the expression list
void generateEXP(EXP *e, bool recurse)
{
    if (e == NULL) return;
    if (recurse) generateEXP(e->next, recurse);

    switch (e->kind)
    {
        case k_expKindIdentifier:
            fprintf(outputFile, "%s", prepend(e->val.identExp.ident));
            break;
        
        // Literals
        case k_expKindIntLiteral:
            fprintf(outputFile, "%d", e->val.intLiteral);
            break;
        case k_expKindFloatLiteral:
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

        // Binary operators
        case k_expKindAnd:
            generateEXP_binary(e, recurse, "&&");
            break;
        case k_expKindOr:
            generateEXP_binary(e, recurse, "||");
            break;
        // Relational binary operators
        case k_expKindEq:
            if (e->val.binary.lhs->type->kind == k_typeArray && e->val.binary.rhs->type->kind == k_typeArray)
            {
                fprintf(outputFile, "Arrays.equals(");
                generateEXP(e->val.binary.lhs, recurse);
                fprintf(outputFile, ", ");
                generateEXP(e->val.binary.rhs, recurse);
                fprintf(outputFile, ")");
            }
            else if (e->val.binary.lhs->type->kind == k_typeString && e->val.binary.rhs->type->kind == k_typeString) generateEXP_stringCompare(e, recurse, "==");
            else generateEXP_binary(e, recurse, "==");
            break;
        case k_expKindNotEq:
            if (e->val.binary.lhs->type->kind == k_typeArray && e->val.binary.rhs->type->kind == k_typeArray)
            {
                fprintf(outputFile, "!(Arrays.equals(");
                generateEXP(e->val.binary.lhs, recurse);
                fprintf(outputFile, ", ");
                generateEXP(e->val.binary.rhs, recurse);
                fprintf(outputFile, "))");
            }
            else if (e->val.binary.lhs->type->kind == k_typeString && e->val.binary.rhs->type->kind == k_typeString) generateEXP_stringCompare(e, recurse, "!=");
            else generateEXP_binary(e, recurse, "!=");
            break;
        case k_expKindLess:
            if (e->val.binary.lhs->type->kind == k_typeString && e->val.binary.rhs->type->kind == k_typeString) generateEXP_stringCompare(e, recurse, "<");
            else generateEXP_binary(e, recurse, "<");
            break;
        case k_expKindLessEq:
            if (e->val.binary.lhs->type->kind == k_typeString && e->val.binary.rhs->type->kind == k_typeString) generateEXP_stringCompare(e, recurse, "<=");
            else generateEXP_binary(e, recurse, "<=");
            break;
        case k_expKindGrtr:
            if (e->val.binary.lhs->type->kind == k_typeString && e->val.binary.rhs->type->kind == k_typeString) generateEXP_stringCompare(e, recurse, ">");
            else generateEXP_binary(e, recurse, ">");
            break;
        case k_expKindGrtrEq:
            if (e->val.binary.lhs->type->kind == k_typeString && e->val.binary.rhs->type->kind == k_typeString) generateEXP_stringCompare(e, recurse, ">=");
            else generateEXP_binary(e, recurse, ">=");
            break;
        // Additive binary operators
        case k_expKindAdd:
            generateEXP_binary(e, recurse, "+");
            break;
        case k_expKindMinus:
            generateEXP_binary(e, recurse, "-");
            break;
        case k_expKindBitOr:
            generateEXP_binary(e, recurse, "|");
            break;
        case k_expKindBitXOR:
            generateEXP_binary(e, recurse, "^");
            break;
        // Multiplicative binary operators
        case k_expKindMult:
            generateEXP_binary(e, recurse, "*");
            break;
        case k_expKindDiv:
            generateEXP_binary(e, recurse, "/");
            break;
        case k_expKindMod:
            generateEXP_binary(e, recurse, "\%");
            break;
        case k_expKindLeftShift:
            generateEXP_binary(e, recurse, "<<");
            break;
        case k_expKindRightShift:
            generateEXP_binary(e, recurse, ">>");
            break;
        case k_expKindBitAnd:
            generateEXP_binary(e, recurse, "&");
            break;
        case k_expKindBitClear:
            // Bit clear is equivalent to `AND NOT` (Java: & ~)
            generateEXP_binary(e, recurse, "& ~");
            break;
            
        // Golite supports four unary operators
        case k_expKindUPlus:
            // fprintf(outputFile, "%s", "+"); // TODO is this needed?
            generateEXP(e->val.unary.rhs, recurse);
            break;
        case k_expKindUMinus:
            fprintf(outputFile, "%s", "-");
            generateEXP(e->val.unary.rhs, recurse);
            break;
        case k_expKindBang:
            fprintf(outputFile, "%s", "!");
            generateEXP(e->val.unary.rhs, recurse);
            break;
        case k_expKindUBitXOR:
            // Unary Bitwise XOR is equivalent to 2's complement
            fprintf(outputFile, "%s", "~");
            generateEXP(e->val.unary.rhs, recurse);
            break;
        
        case k_expKindFuncCall:
            generateEXP(e->val.funcCall.primaryExpr, recurse);
            fprintf(outputFile, "(");
            for (EXP *f_e = e->val.funcCall.expList; f_e; f_e = f_e->next)
            {
                generateEXP(f_e, false);
                if (f_e->next != NULL) fprintf(outputFile, ", ");
            }
            fprintf(outputFile, ")");
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
        case k_expKindFieldAccess:
            generateEXP(e->val.fieldAccess.object, recurse);
            fprintf(outputFile, ".%s", e->val.fieldAccess.field);
            break;

        // Builtins
        case k_expKindAppend: ;
            char *sliceIdent = e->val.append.slice->val.identExp.ident;
            EXP *addend = e->val.append.addend;
            fprintf(outputFile, "%s.append(", prepend(sliceIdent));
            generateEXP(addend, false);
            fprintf(outputFile, ")");
            break;
        case k_expKindLen: ;
            char *lenIdent = e->val.lenExp->val.identExp.ident;
            if (e->val.lenExp->type->kind == k_typeSlice) {
                fprintf(outputFile, "%s.len", prepend(lenIdent));
            } else if (e->val.lenExp->type->kind == k_typeArray) {
                fprintf(outputFile, "%s.length", prepend(lenIdent));
            } else if (e->val.lenExp->type->kind == k_typeString) { 
                // may have to change condition to resolveToStringBaseType - not sure
                generateEXP(e->val.lenExp, recurse);
                fprintf(outputFile, ".length()"); 
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

        // Parenthesized expressions
        case k_expKindUParenthesized:
            fprintf(outputFile, "(");
            generateEXP(e->val.unary.rhs, recurse);
            fprintf(outputFile, ")");
            break;
        
        // Type Cast
        case k_expKindCast:
            fprintf(outputFile, "%s(", getStringFromType(e->val.cast.type, true));
            generateEXP(e->val.cast.exp, recurse);
            fprintf(outputFile, ")");
            break;

        default:
            break;
    }
}

// Generate a binary expression given a binary operator. To be used within the 
// context of generateEXP for a binary expression kind
void generateEXP_binary(EXP *e, int recurse, char *operator)
{
    generateEXP(e->val.binary.lhs, recurse);
    fprintf(outputFile, " %s ", operator);
    generateEXP(e->val.binary.rhs, recurse);
}

void generateEXP_stringCompare(EXP *e, int recurse, char *operator)
{
    generateEXP(e->val.binary.lhs, recurse);
    fprintf(outputFile, ".compareTo(");
    generateEXP(e->val.binary.rhs, recurse);
    fprintf(outputFile, ") %s 0", operator);
}

char *getStringFromType(TYPE *t, bool isPrimitive){
    if (t != NULL) {
        switch(t->kind) {
            case k_typeInt:
            case k_typeRune:
                return isPrimitive ? "int" : "Integer";
            case k_typeBool:
                return isPrimitive ? "boolean" : "Boolean";
            case k_typeString:
                return "String";
            case k_typeFloat:
                return isPrimitive ? "double" : "Double";
            case k_typeStruct:
                // Structs are implemented as Classes in Java so we need their declared type name
                return prepend(t->typeName);
            default:
                return "Currently unsupported in `getStringFromType` func.";
        }
    }
    return "";
}