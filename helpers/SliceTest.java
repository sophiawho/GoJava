//~Cap: 0 , len: 0
//~Cap: 2 , len: 1
//~Cap: 2 , len: 2
//~Cap: 4 , len: 3
//~Cap: 4 , len: 4
//~Cap: 8 , len: 5
//~Cap: 8 , len: 6
//~Cap: 8 , len: 7
//~Cap: 8 , len: 8
//~Cap: 16 , len: 9
//~Cap: 5 , len: 5
//~

public class SliceTest {
    public static void main(String[] args) {
        testBuiltins();
    }

    private static void testBuiltins() {
        Slice<Integer> a = new Slice<>();
        for (int i=0; i<10; i++) {
            System.out.println("Cap: " + a.cap + ", len: " + a.len);
            a = a.append(0);
        }

        int[] b = new int[5];
        System.out.println("Cap: " + b.length + ", len: " + b.length);
    }
}