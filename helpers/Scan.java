import java.util.*;
// This is a helper Java class written by Sophia to avoid writing duplicated code in the scanner `golite.l`

class Scan {

    static Map<String, String> opToToken = new HashMap<>();

    public static void main(String[] args) {
        initMap(); 

        String[] keywords = {"break", "case", "chan", "const", "continue", "default", "defer", "else", "fallthrough", "for", "func",
                            "go", "goto", "if", "import", "interface", "map", "package", "range", "return", "select", "struct",
                            "switch", "type", "var"};

        // printOpTokens();

        // for (String s: keywords) {
        //     printTokens(s);
        // }

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

    public static void printTokens(String str) {
        StringBuilder token = new StringBuilder();
        token.append("t");
        token.append(str.toUpperCase());
        System.out.printf("%%token %s\n", token.toString());
    }

    public static void printOpTokens() {
        for (String s: opToToken.keySet()) {
             System.out.printf("%%token %s\n", opToToken.get(s));
        }
    }

    // switch 		{ if (g_tokens) printf("switch\n"); return 'switch'; }
    public static void prettyPrint(String str, boolean isKeyword) {

        if (isKeyword) {
            StringBuilder token = new StringBuilder();
            token.append("t");
            token.append(str.toUpperCase());
            // System.out.printf("%s { if (g_tokens) printf(\"%s\\n\"); }\n", str, token.toString());
            // After declaring token types in golite.y
            System.out.printf("%s { if (g_tokens) printf(\"%s\\n\"); RETURN(%s); }\n", str, token.toString(), token.toString());
            return;
        }
        System.out.printf("\"%s\" \t { if (g_tokens) printf(\"%s\\n\"); RETURN(%s); }\n", str, opToToken.get(str), opToToken.get(str));
    }

    private static void initMap() {
        opToToken.put("+", "tADD");
        opToToken.put("-", "tMINUS");
        opToToken.put("*", "tMULT");
        opToToken.put("/", "tDIV");
        opToToken.put("%", "tMOD");
        opToToken.put("&", "tBITAND");
        opToToken.put("|", "tBITOR");
        opToToken.put("^", "tBITXOR");
        opToToken.put("<<", "tLEFTSHIFT");
        opToToken.put(">>", "tRIGHTSHIFT");
        opToToken.put("&^", "tBITCLEAR");
        opToToken.put("+=", "tPLUSEQ");
        opToToken.put("-=", "tMINUSEQ");
        opToToken.put("*=", "tMULTEQ");
        opToToken.put("/=", "tDIVEQ");
        opToToken.put("%=", "tMODEQ");
        opToToken.put("&=", "tBITANDEQ");
        opToToken.put("|=", "tBITOREQ");
        opToToken.put("^=", "tBITXOREQ");
        opToToken.put("<<=", "tLEFTSHIFTEQ");
        opToToken.put(">>=", "tRIGHTSHIFTEQ");
        opToToken.put("&^=", "tBITCLEAREQ");
        opToToken.put("&&", "tAND");
        opToToken.put("||", "tOR");
        opToToken.put("<-", "tLEFTARROW");
        opToToken.put("++", "tINCREMENT");
        opToToken.put("--", "tDECREMENT");
        opToToken.put("==", "tEQ");
        opToToken.put("<", "tLESS");
        opToToken.put(">", "tGRTR");
        opToToken.put("=", "tASSIGN");
        opToToken.put("!", "tBANG");
        opToToken.put("!=", "tNOTEQ");
        opToToken.put("<=", "tLESSEQ");
        opToToken.put(">=", "tGRTREQ");
        opToToken.put(":=", "tCOLONASSIGN");
        opToToken.put("...", "tELLIPSIS");
        opToToken.put("(", "tLPAR");
        opToToken.put(")", "tRPAR");
        opToToken.put("[", "tLBRACKET");
        opToToken.put("]", "tRBRACKET");
        opToToken.put("{", "tLBRACE");
        opToToken.put("}", "tRBRACE");
        opToToken.put(",", "tCOMMA");
        opToToken.put(".", "tPERIOD");
        opToToken.put(";", "tSEMICOLON");
        opToToken.put(":", "tCOLON");
    }

}