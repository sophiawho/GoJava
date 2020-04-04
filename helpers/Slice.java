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