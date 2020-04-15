import java.util.Arrays;
import java.lang.Boolean;
import java.util.Arrays;
import java.util.ArrayList;
import java.lang.ArrayIndexOutOfBoundsException;

class Slice<T> {
    public int cap;
    public int len;
    public ArrayList<T> data;

    public Slice() {
        data = new ArrayList<>();
        cap = 0;
        len = 0;
    }

    public Slice(int cap, int len, ArrayList<T> data) {
        this.cap = cap;
        this.len = len;
        this.data = data;
    }

    public Slice<T> append(T node) {
        // If at capacity, double capacity and copy over all elements
        if (len == cap) {
            ArrayList<T> newData = new ArrayList<>();
            int doubleCap = cap == 0 ? 2 : cap*2;
            for (int i=0; i<len; i++) {
                newData.add(i, data.get(i));
            }
            newData.add(len, node);
            return new Slice<T>(doubleCap, len+1, newData);
        }
        data.add(len, node);
        return new Slice<T>(cap, len+1, data);
    }

    public T get(int index) throws ArrayIndexOutOfBoundsException {
        if (index < 0 || index >= len || index >= cap) {
            throw new ArrayIndexOutOfBoundsException("Index must be between 0 and the length/capacity of the slice.");
        }
        return data.get(index);
    }

    public void put(int index, T node) throws ArrayIndexOutOfBoundsException {
        if (index < 0 || index >= len || index >= cap) {
            throw new ArrayIndexOutOfBoundsException("Index must be between 0 and the length/capacity of the slice.");
        } 
        data.add(index, node);
    }
}
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

public class colonassign {
	static Boolean __golite__true = Boolean.valueOf(true);
	static Boolean __golite__false = Boolean.valueOf(false);

	@SuppressWarnings({"unchecked", "deprecation"})
	public static void __golite__main() {
		int __golite__b = 2;
		int __golite__a = 3;
		System.out.print(__golite__a);
		System.out.print(" ");
		System.out.print(__golite__b);
		System.out.println();
		double __golite__c = 0.0;
		double __golite__tmp_0 = 4.000000;
		int __golite__tmp_1 = __golite__a;
		int __golite__tmp_2 = __golite__b;
		__golite__c = __golite__tmp_0;
		__golite__b = __golite__tmp_1;
		__golite__a = __golite__tmp_2;

		System.out.print(__golite__a);
		System.out.print(" ");
		System.out.print(__golite__b);
		System.out.print(" ");
		System.out.printf("%+.6e", __golite__c);
		System.out.println();
	}

	@SuppressWarnings({"unchecked", "deprecation"})
	public static void main(String[] args) {
		__golite__main();
	}
}
