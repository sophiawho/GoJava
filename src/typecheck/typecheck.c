#include "typecheck.h"
#include "../ast/tree.h"
#include "../ast/stmt.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// ====================== HELPER FUNCTIONS =====================================
// 1.2.1 Resolve Type: Find the underlying type a defined type T given the set of types in scope T
TYPE *resolveType(TYPE *t) {
    if (t->kind == k_typeInt ||
        t->kind == k_typeBool ||
        t->kind == k_typeFloat ||
        t->kind == k_typeRune || 
        t->kind == k_typeString || 
        t->kind == k_typeSlice ||
        t->kind == k_typeArray || 
        t->kind == k_typeStruct ) return t;
    return resolveType(t->parent);
}

bool isOrdered(TYPE *t) {
    if (t->kind == k_typeInt ||
        t->kind == k_typeBool ||
        t->kind == k_typeFloat ||
        t->kind == k_typeRune || 
        t->kind == k_typeString ) return true;
    return false;
}

bool resolveToBaseType(TYPE *t){
    TYPE* rt = resolveType(t);
    if (rt->kind == k_typeInt ||
        rt->kind == k_typeFloat ||
        rt->kind == k_typeRune ||
        rt->kind == k_typeString ||
        rt->kind == k_typeBool) return true;
    return false;
}

bool resolveToNumbericBaseType(TYPE *t){
    TYPE* rt = resolveType(t);
    if (rt->kind == k_typeInt ||
        rt->kind == k_typeFloat ||
        rt->kind == k_typeRune) return true;
    return false;
}

bool resolveToIntegerBaseType(TYPE* t){
    TYPE* rt = resolveType(t);
    if (rt->kind == k_typeInt ||
        rt->kind == k_typeRune) return true;
    return false;
}

// =============================== TYPECHECK ===================================

void typecheckPROG(PROG *root) {
	if (root == NULL) { return; }
	typeTOPLEVELDECL(root->rootTopLevelDecl);
}

void typeTOPLEVELDECL(TOPLEVELDECL *tld) {
    if (tld == NULL) return;
    typeTOPLEVELDECL(tld->next);

    switch (tld->kind) {
    case k_topLevelDeclFunc:
        typeFUNC(tld->val.funcDecl);
        break;
    case k_topLevelDeclType:
        // 2.2 Well-typed as per symbol table
        break;
    case k_topLevelDeclVar:
        typeVARSPEC(tld->val.varDecl);
        break;
    }
}

/* 2.3 A function declaration typechecks if the statements of its body type check
2.4 Special functions `init` and `main` must have no parameters or return type
They may only be declared as functions in the top-level scope
`init` does not introduce a binding and thus may be declared multiple times.
*/
void typeFUNC(FUNC *f) {
}

/* 2.1
Case 1: var x T = expr
If expr is well-typed and its type is T1 and T1=T, the mapping x:T is 
added to the symbol table

Case 2: var x = expr
If expr is well-typed and its type is T, the mapping x:T is added to the 
symbol table.
*/
void typeVARSPEC(VARSPEC *varspec) {
    return;
}