
class FloatFormat {
    
    static String format(double number) {
        DecimalFormatSymbols otherSymbols = new DecimalFormatSymbols(Locale.getDefault());
        otherSymbols.setDecimalSeparator('.');
        otherSymbols.setExponentSeparator("e");

        DecimalFormat formatter = new DecimalFormat("0.000000E000", otherSymbols);
        formatter.setPositivePrefix("+");
        String res = formatter.format(number);
        int index = res.indexOf('e') + 1;
        if (res.charAt(index) != '-') {
            res = res.substring(0, index) + "+" + res.substring(index);
        }
        return res;
    }
}