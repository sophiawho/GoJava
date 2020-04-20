
class Cast {
    public static int castToInteger(int i) {
        return i;
    }
    public static int castToInteger(double d) {
        return (int) d;
    }
    public static int castToInteger(char c) {
        return (int) c;
    }
    public static double castToDouble(int i) {
        return (double) i;
    }
    public static double castToDouble(double d) {
        return d;
    }
    public static double castToDouble(char c) {
        return (double) c;
    }
    public static String castToString(int i) {
        Character c = (char) i;
        return c.toString();
    }
    public static String castToString(char c) {
        Character ch = c;
        return ch.toString();
    }
    public static String castToString(String s) {
        return s;
    }
    public static Boolean castToBoolean(boolean b) {
        return b;
    }
}
