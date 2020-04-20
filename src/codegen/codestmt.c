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

int labelId = -1;
void generateSwitch(STMT *s);

void traverseExpForPrint(EXP *e, bool newLine, bool last) {
    if (e == NULL) return;
    traverseExpForPrint(e->next, newLine, false);

    generateINDENT(indent);

    // Format floats in scientific notation:
    // 6 decimal digits, 3 mantissa digits, positive or negative prefixes for base and mantissa
    TYPE *rt = resolveType(e->type);
    if (rt->kind == k_typeFloat) 
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

void generateUndeclaredIdentifiers(EXP *lhs, EXP *rhs) {
    for (EXP *e = lhs; e; e=e->next) {
        if (e->kind != k_expKindIdentifier) {
            fprintf(outputFile, "Undeclared LHS expressions so far only work for identifer type LHS expressions.");
            continue;
        }
        // If LHS expression has been declared, no need to declare
        if (e->val.identExp.colonAssignDeclared) continue;
        // Else generate variable declaration
        IDENT *id = makeIDENT(e->val.identExp.ident);
        id->next = NULL;
        VARSPEC *vs = makeVarSpec(id, NULL, e->type);
        generateVarDecl(vs);
    }
}

void generateAssignStmt(EXP *lhs, EXP *rhs) {
    int temp_counter_start = temp_counter;
    char temp_variable[80];
    while (rhs != NULL) {
        char *type = getStringFromType(rhs->type, rhs->type->kind != k_typeSlice);
        sprintf(temp_variable, "__golite__tmp_%d", temp_counter++);
        generateINDENT(indent); 
        fprintf(outputFile, "%s %s = ", type, temp_variable);
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
        } else if (lhs->type->kind == k_typeArray) { // Copy over the elements of the array
            // Edge case: If the array is of an undeclared length, like from a struct, initialize the array with the base type and needed length
            // Example: programs/3-semantics+codegen/valid/typespec1.go (line 175) would otherwise result in a null pointer exception
            generateEXP(lhs, false); 
            fprintf(outputFile, " = new %s[%s.length];\n", getStringFromType(lhs->type->val.arrayType.type, true), temp_variable);

            generateINDENT(indent);
            fprintf(outputFile, "for (int i=0; i<%s.length; i++) ", temp_variable);
            generateEXP(lhs, false);
            fprintf(outputFile, "[i] = %s[i]", temp_variable);
        } else if (lhs->type->kind == k_typeStruct) {
            generateEXP(lhs, false);
            fprintf(outputFile, " = %s.copy()", temp_variable);
        } else {
            generateEXP(lhs, false);
            fprintf(outputFile, " = ");
            fprintf(outputFile, "%s", temp_variable);
        }
        lhs = lhs->next;
        fprintf(outputFile, ";\n");
    }
    fprintf(outputFile, "\n");
}

void generateForLoop(STMT *s)
{
    labelId++;

    // Keep all for loop init and post statements in a new scope
    generateINDENT(indent);
    fprintf(outputFile, "{\n");
    indent++;
    if (s->val.forLoop.initStmt != NULL)
    {
        generateINDENT(indent);
        fprintf(outputFile, "//Init statements\n");
        generateSTMT(s->val.forLoop.initStmt);
    }

    // We assume there's a continue statement, so we need to duplicate the 
    // post statements before the while loop
    generateINDENT(indent);
    fprintf(outputFile, "boolean do_post_loop_%d = false;\n", labelId);
    generateINDENT(indent);
    fprintf(outputFile, "boolean continue_loop_%d = true;\n", labelId);
    generateINDENT(indent);
    // Need a variable to be used a true literal for infinite loops, otherwise we can get 
    // unreachable statement errors
    fprintf(outputFile, "boolean true_literal_%d = true;\n", labelId);
    generateINDENT(indent);
    fprintf(outputFile, "Loop_%d:\n", labelId);
    generateINDENT(indent);
    fprintf(outputFile, "while(continue_loop_%d)\n", labelId);
    generateINDENT(indent);
    fprintf(outputFile, "{\n");

    // Duplicate post statements needed for continue statement
    // We add an if statement to avoid running any post statements before the loop's first
    // iteration
    indent++;
    generateINDENT(indent);
    fprintf(outputFile, "if (do_post_loop_%d)\n", labelId);
    generateINDENT(indent);
    fprintf(outputFile, "{\n");
    indent++;
    generateSTMT(s->val.forLoop.postStmt);
    indent--;
    generateINDENT(indent);
    fprintf(outputFile, "}\n");

    // While statement with condition
    generateINDENT(indent);
    fprintf(outputFile, "while (");
    if (s->val.forLoop.condition != NULL) generateEXP(s->val.forLoop.condition, false);
    else fprintf(outputFile, "true_literal_%d", labelId);
    fprintf(outputFile, ") \n");

    // Create the body of the for loop by extending the post statements
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

    generateINDENT(indent);
    fprintf(outputFile, "continue_loop_%d = false;\n", labelId);

   // Close the Loop
    indent--;
    generateINDENT(indent);
    fprintf(outputFile, "}\n");
    
    // Close the scope
    indent--;
    generateINDENT(indent);
    fprintf(outputFile, "}\n");

    labelId--;
}

void generateOpAssignStmt(EXP *lhs, EXP* rhs, AssignKind kind) {
    generateINDENT(indent);
    generateEXP(lhs, false);
    fprintf(outputFile, " = ");
    switch (kind) {
        case k_opAssignKindPlusEq:
            generateEXP_binary(lhs, rhs, false, "+");
            break;
        case k_opAssignKindMinusEq:
            generateEXP_binary(lhs, rhs, false, "-");
            break;
        case k_opAssignKindMultEq:
            generateEXP_binary(lhs, rhs, false, "*");
            break;
        case k_opAssignKindDivEq:
            generateEXP_binary(lhs, rhs, false, "/");
            break;
        case k_opAssignKindModEq:
            generateEXP_binary(lhs, rhs, false, "\%");
            break;
        case k_opAssignKindBitAndEq:
            generateEXP_binary(lhs, rhs, false, "&");
            break;
        case k_opAssignKindBitOrEq:
            generateEXP_binary(lhs, rhs, false, "|");
            break;
        case k_opAssignKindBitXorEq:
            generateEXP_binary(lhs, rhs, false, "^");
            break;
        case k_opAssignKindLeftShiftEq:
            generateEXP_binary(lhs, rhs, false, "<<");
            break;
        case k_opAssignKindRightShiftEq:
            generateEXP_binary(lhs, rhs, false, ">>");
            break;
        case k_opAssignKindBitClearEq:
            generateEXP_binary(lhs, rhs, false, "& ~");
            break;
        default:
            break;
    }
    fprintf(outputFile, ";\n");
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
            switch (s->val.assignStmt.kind) {
                case k_stmtAssign:
                    generateAssignStmt(s->val.assignStmt.lhs, s->val.assignStmt.rhs);
                    break;
                case k_stmtColonAssign:
                    generateUndeclaredIdentifiers(s->val.assignStmt.lhs, s->val.assignStmt.rhs);
                    generateAssignStmt(s->val.assignStmt.lhs, s->val.assignStmt.rhs);
                    break;
                default:
                    generateOpAssignStmt(s->val.assignStmt.lhs, s->val.assignStmt.rhs, s->val.assignStmt.kind);
                    break;
            }
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
            generateTYPESPEC(s->val.typeDecl, false);
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
            generateSwitch(s);
            break;

        case k_stmtKindFor: 
            generateForLoop(s);
            break;

        case k_stmtKindBreak:
            // Need to create an if statement to avoid getting unreachable statement errors
            generateINDENT(indent);
            if (labelId >= 0) fprintf(outputFile, "if (true) {break Loop_%d;}\n", labelId);
            break;

        case k_stmtKindContinue:
            // Need to create an if statement to avoid getting unreachable statement errors
            generateINDENT(indent);
            if (labelId >= 0) fprintf(outputFile, "if (true) {do_post_loop_%d = true; continue Loop_%d;}\n", labelId, labelId);
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

// Generate a switch statement, its expression case clauses, and each expression 
// case clause's list of statements
void generateSwitch(STMT *s)
{
    labelId++;

    // Need a new scope to contain any variable declarations (from intial statements) within
    generateINDENT(indent);
    fprintf(outputFile, "{\n");
    indent++;
    
    generateINDENT(indent);
    fprintf(outputFile, "// Switch statement\n");
    generateSTMT(s->val.switchStmt.simpleStmt);
    generateINDENT(indent);
    fprintf(outputFile, "boolean continue_loop_%d = true;\n", labelId);

    // Need a label to be able to use switch break statements
    generateINDENT(indent);
    fprintf(outputFile, "Loop_%d:\n", labelId);

    // Need a while loop to be able to use a label
    generateINDENT(indent);
    fprintf(outputFile, "while (continue_loop_%d) {\n", labelId);
    indent++;

    // Create a temporary variable for the switch tag, since we use if and else-if statements
    // to recreate GoLite's switch statement behavior
    generateINDENT(indent);
    if (s->val.switchStmt.exp != NULL)
    {
        fprintf(outputFile, "%s switch_tag_%d = ", 
            getStringFromType(s->val.switchStmt.exp->type, true), labelId);
        generateEXP(s->val.switchStmt.exp, false);
        fprintf(outputFile, ";\n");
    }
    else
    {
        fprintf(outputFile, "boolean switch_tag_%d = true;\n", labelId);
    }

    // Need an initial if statement that never occurs because for some reason ecc->next == NULL
    // never passes in an if statement, so every expression case clause is an `else if` statement
    generateINDENT(indent);
    fprintf(outputFile, "if (false) {}\n");

    generateEXPRCASECLAUSE(s->val.switchStmt.caseClauses);

    generateINDENT(indent);
    fprintf(outputFile, "continue_loop_%d = false;\n", labelId);

    indent--;
    generateINDENT(indent);
    fprintf(outputFile, "}\n");

    indent--;
    generateINDENT(indent);
    fprintf(outputFile, "}\n");

    labelId--;
}

// Helper function to generate case clauses (no default cases)
// To be used within the context of generating expression case clauses (generateEXPRCASECLAUSE)
void generateEXPRCASECLAUSE_caseClauses(EXPRCASECLAUSE *ecc)
{
    if (ecc == NULL) return;
    generateEXPRCASECLAUSE_caseClauses(ecc->next);

    switch (ecc->kind)
    {
    case k_caseClause:
        
        generateINDENT(indent);
        fprintf(outputFile, "else if (");
        
        EXP *e = ecc->expList;
        while (e != NULL)
        {
            if (e->next != NULL) 
            {   
                fprintf(outputFile, "switch_tag_%d == (", labelId);
                generateEXP(e, false);
                fprintf(outputFile, ") || ");
            }
            else
            {
                fprintf(outputFile, "switch_tag_%d == (", labelId);
                generateEXP(e, false);
                fprintf(outputFile, "))\n");
            }
            e = e->next;
        }
        generateINDENT(indent);
        fprintf(outputFile, "{\n");
        indent++;

        generateSTMT(ecc->stmtList);

        indent--;
        generateINDENT(indent);
        fprintf(outputFile, "}\n");
        break;

    case k_defaultClause:
        break;
    }
}

// Generates switch's expression case clauses
// To be used within the context of generating switch statements
void generateEXPRCASECLAUSE(EXPRCASECLAUSE *ecc)
{
    EXPRCASECLAUSE *defaultEcc = NULL;
    for (EXPRCASECLAUSE *current = ecc; current; current = current->next)
    {
        if (current->kind == k_defaultClause) 
        {
            defaultEcc = current;
            break;
        }
    }
    
    // Need to recurse from tail to head to generate code in correct order
    generateEXPRCASECLAUSE_caseClauses(ecc);

    if (defaultEcc != NULL)
    {
        generateINDENT(indent);
        fprintf(outputFile, "else\n");
        generateINDENT(indent);
        fprintf(outputFile, "{\n");
        indent++;
        
        generateINDENT(indent);
        fprintf(outputFile, "// Default case\n");
        generateSTMT(defaultEcc->stmtList);

        indent--;
        generateINDENT(indent);
        fprintf(outputFile, "}\n");
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
        // We need to initialize slices with their object type (ie: new Slice<Integer> instead of new Slice<int>)
        char *type = getStringFromType(vs->type, !containsSlice(vs->type));
        generateINDENT(indent); 
        if (indent == 1) fprintf(outputFile, "static ");
        fprintf(outputFile, "%s %s = ", type, prepend(curIdent->ident));
        if (vs->type->kind == k_typeArray) {
            char *arrayType = getStringFromType(vs->type->val.arrayType.type, true);
            fprintf(outputFile, "new %s[%d]", arrayType, vs->type->val.arrayType.size);
            if (strcmp(arrayType, "String") == 0) { // Edge case: Set uninitialized value of strings to be ""
                fprintf(outputFile, ";\n");
                generateINDENT(indent);
                fprintf(outputFile, "Arrays.fill(%s, \"\")", prepend(curIdent->ident));
            }
        } else if (vs->type->kind == k_typeSlice) {
            if (vs->rhs != NULL && vs->rhs->kind == k_expKindAppend) {
                generateEXP(vs->rhs, false);
            } else {
                fprintf(outputFile, "new Slice<>()");
            }
        } else if (vs->type->kind == k_typeStruct) {
            // Structs are implemented as Classes in Java, so we need to allocate memory with `new` and
            // construct the variable
            fprintf(outputFile, "new %s()", type);
        } else {
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

// Need to use a global variable to determine whether to add a comma at the end
// of an input parameter 
int generateTYPESPEC_paramCount = 0;

// Generate the parameter list for a given typespec and identifier list.
// Only use this in generateTYPESPEC
void generateTYPESPEC_paramList(TYPESPEC *ts, IDENT *id)
{
    if (id == NULL) return;
    generateTYPESPEC_paramList(ts, id->next);

    generateTYPESPEC_paramCount--;
    fprintf(outputFile, "%s ", getStringFromType(ts->type, !containsSlice(ts->type)));
    if (isBlankId(id->ident))
    {
        if (generateTYPESPEC_paramCount > 0) fprintf(outputFile, "blank_id_%d, ", generateTYPESPEC_paramCount);
        else fprintf(outputFile, "blank_id_%d", generateTYPESPEC_paramCount);
    }
    else
    {
        if (generateTYPESPEC_paramCount > 0) fprintf(outputFile, "%s, ", prepend(id->ident));
        else fprintf(outputFile, "%s", prepend(id->ident));
    }
}

// Generate type declarations (NOTE: only for struct types!) and input parameters
void generateTYPESPEC(TYPESPEC *ts, bool isTopLevelTypeDecl)
{
    if (ts == NULL) return;

    switch (ts->kind)
    {
    case k_typeSpecKindTypeDeclaration:
        generateTYPESPEC(ts->next, isTopLevelTypeDecl);
        break;
    
    case k_typeSpecKindParameterList: // TODO (Sophia) Do stuff
        generateTYPESPEC_paramCount = generateTYPESPEC_paramCount + countIDENT(ts->ident);
        generateTYPESPEC(ts->next, false);
        generateTYPESPEC_paramList(ts, ts->ident);
        break;
    }
}

// Generate a struct's attribute and type specifiers. This is to be used within the 
// context of emitting the declaration of a struct type
void generateSTRUCTSPEC(STRUCTSPEC *ss)
{
    if (ss == NULL) return;
    generateSTRUCTSPEC(ss->next);

    char *type = getStringFromType(ss->type, !containsSlice(ss->type));

    for (IDENT *id = ss->attribute; id; id=id->next)
    {
        if (isBlankId(id->ident)) continue;
        generateINDENT(indent);
        fprintf(outputFile, "%s %s", type, prepend(id->ident));
        if (strcmp(getStringFromType(ss->type, true), "String") == 0) {
            fprintf(outputFile, " = \"\"");
        } else if (ss->type->kind == k_typeArray) {
            char *arrayType = getStringFromType(ss->type->val.arrayType.type, true);
            fprintf(outputFile, " = new %s[%d]", arrayType, ss->type->val.arrayType.size);
        } else if (ss->type->kind == k_typeSlice) {
            fprintf(outputFile, " = new Slice<>()");
        }
        fprintf(outputFile, ";\n");
    }
}

// Generate function call arguments in order
// Set currArg = numArgs for proper behavior
void generateFuncCallArgs(EXP *e, int numArgs, int currArg)
{
    if (e == NULL) return;
    generateFuncCallArgs(e->next, numArgs, --currArg);

    generateEXP(e, false);
    // If the current argument isn't the last one, add a comma
    if (numArgs != currArg + 1) fprintf(outputFile, ", ");
    else fprintf(outputFile, ")");
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
            generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "&&");
            break;
        case k_expKindOr:
            generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "||");
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
            else generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "==");
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
            else generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "!=");
            break;
        case k_expKindLess:
            if (e->val.binary.lhs->type->kind == k_typeString && e->val.binary.rhs->type->kind == k_typeString) generateEXP_stringCompare(e, recurse, "<");
            else generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "<");
            break;
        case k_expKindLessEq:
            if (e->val.binary.lhs->type->kind == k_typeString && e->val.binary.rhs->type->kind == k_typeString) generateEXP_stringCompare(e, recurse, "<=");
            else generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "<=");
            break;
        case k_expKindGrtr:
            if (e->val.binary.lhs->type->kind == k_typeString && e->val.binary.rhs->type->kind == k_typeString) generateEXP_stringCompare(e, recurse, ">");
            else generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, ">");
            break;
        case k_expKindGrtrEq:
            if (e->val.binary.lhs->type->kind == k_typeString && e->val.binary.rhs->type->kind == k_typeString) generateEXP_stringCompare(e, recurse, ">=");
            else generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, ">=");
            break;
        // Additive binary operators
        case k_expKindAdd:
            generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "+");
            break;
        case k_expKindMinus:
            generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "-");
            break;
        case k_expKindBitOr:
            generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "|");
            break;
        case k_expKindBitXOR:
            generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "^");
            break;
        // Multiplicative binary operators
        case k_expKindMult:
            generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "*");
            break;
        case k_expKindDiv:
            generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "/");
            break;
        case k_expKindMod:
            generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "\%");
            break;
        case k_expKindLeftShift:
            generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "<<");
            break;
        case k_expKindRightShift:
            generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, ">>");
            break;
        case k_expKindBitAnd:
            generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "&");
            break;
        case k_expKindBitClear:
            // Bit clear is equivalent to `AND NOT` (Java: & ~)
            generateEXP_binary(e->val.binary.lhs, e->val.binary.rhs, recurse, "& ~");
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
            int numArgs = countEXP(e->val.funcCall.expList);
            if (numArgs != 0) generateFuncCallArgs(e->val.funcCall.expList, numArgs, numArgs);
            else fprintf(outputFile, ")");
            break;
        
        case k_expKindArrayAccess: ;
            EXP *refExp = e->val.arrayAccess.arrayReference;
            EXP *indexExp = e->val.arrayAccess.indexExp;
            if (refExp->type->kind == k_typeSlice) {
                generateEXP(refExp, false);
                fprintf(outputFile, ".get(");
                generateEXP(indexExp, false);
                fprintf(outputFile, ")");
            } else if (refExp->type->kind == k_typeArray) {
                generateEXP(refExp, false);
                fprintf(outputFile, "[");
                generateEXP(indexExp, false);
                fprintf(outputFile, "]");
            }
            break;
        case k_expKindFieldAccess:
            generateEXP(e->val.fieldAccess.object, recurse);
            fprintf(outputFile, ".%s", prepend(e->val.fieldAccess.field));
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
        case k_expKindCast: ;
            TYPE *baseType = resolveType(e->val.cast.type);
            fprintf(outputFile, "Cast.castTo%s(", getStringFromType(baseType, false));
            generateEXP(e->val.cast.exp, recurse);
            fprintf(outputFile, ")");
            break;

        default:
            break;
    }
}

// Generate a binary expression given a binary operator. To be used within the 
// context of generateEXP for a binary expression kind
void generateEXP_binary(EXP *lhs, EXP *rhs, int recurse, char *operator)
{
    generateEXP(lhs, recurse);
    fprintf(outputFile, " %s ", operator);
    generateEXP(rhs, recurse);
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
            case k_typeStruct: ;
                // Structs are implemented as Classes in Java so we need their declared type name
                char structname[100];
                sprintf(structname, "__golite__struct__%d", t->val.structType.codegenTag);
                return strdup(structname);
            case k_typeSlice: ;
                char slice[100];
                sprintf(slice, "Slice<%s>", getStringFromType(t->val.sliceType.type, isPrimitive));
                return strdup(slice);
            case k_typeArray: ;
                char array[100];
                sprintf(array, "%s[]", getStringFromType(t->val.arrayType.type, isPrimitive));
                return strdup(array);
            case k_typeInfer:
                return getStringFromType(t->parent, isPrimitive);
            default:
                fprintf(stderr, "Unsupported %s\n", t->typeName);
                throwInternalError("Currently unsupported in `getStringFromType` func.", t->lineno);
        }
    }
    return "";
}