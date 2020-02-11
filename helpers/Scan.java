import java.util.*;
// This is a helper Java class written by Sophia to avoid writing duplicated code in the scanner `golite.l`

class Scan {
    public static void main(String[] args) {
        System.out.println("\n");

        String[] keywords = {"break", "case", "chan", "const", "continue", "default", "defer", "else", "fallthrough", "for", "func",
                            "go", "goto", "if", "import", "interface", "map", "package", "range", "return", "select", "struct",
                            "switch", "type", "var"};
        for (String s: keywords) {
            prettyPrint(s, true);
        }

        System.out.println("\n");

        String[] operatorsAndDelimiters = {"+", "-", "*", "/", "%", "&", "|", "^", "<<", ">>", "&^", "+=", "-=", "*=", "/=", "%=",
                                            "&=", "|=", "^=", "<<=", ">>=", "&^=", "&&", "||", "<-", "++", "--", "==", "<", ">",
                                            "=", "!", "!=", "<=", ">=", ":=", "...", "(", "[", "{", ",", ".", ")", "]", "}", ";", ":"};
        for (String s: operatorsAndDelimiters) {
            prettyPrint(s, false);
        }
    }

    // switch 		{ if (g_tokens) printf("switch\n"); return 'switch'; }
    public static void prettyPrint(String str, boolean isKeyword) {
        if (isKeyword) {
            StringBuilder token = new StringBuilder();
            token.append("t");
            token.append(str.toUpperCase());
            System.out.printf("%s { if (g_tokens) printf(\"%s\\n\"); }\n", str, token.toString());
            // After declaring token types in golite.y
            // System.out.printf("%s { if (g_tokens) printf(\"%s\\n\"); return %s; }\n", str, token.toString(), token.toString());
            return;
        }
        if (str.length() == 1) {
            System.out.printf("\"%s\" \t { if (g_tokens) printf(\"%s\\n\"); }\n", str, str);
            // System.out.printf("\"%s\" \t { if (g_tokens) printf(\"%s\\n\"); return '%s'; }\n", str, str, str);
            return;
        }
        System.out.printf("\"%s\" \t { if (g_tokens) printf(\"%s\\n\"); }\n", str, str);
        // System.out.printf("\"%s\" \t { if (g_tokens) printf(\"%s\\n\"); return \"%s\"; }\n", str, str, str);
    }
}