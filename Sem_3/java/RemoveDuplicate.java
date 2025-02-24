public class RemoveDuplicate {
    public static int removeDuplicates(int[] arr) {
        // when array has 2 or less
        if (arr.length <= 2) {
            return arr.length;
        }

        int currentIndex = 2;

        for (int i = 2; i < arr.length; i++) {
            int currentNumber = arr[i];
            int prevPrevNumber = arr[currentIndex - 2];

            if (currentNumber != prevPrevNumber) {
                arr[currentIndex] = currentNumber;
                currentIndex++;
            }
        }

        return currentIndex;
    }

    public static void main(String[] args) {
        int[] arr = { 1, 1, 2, 3, 3, 3, 4, 5, 6, 7, 7, 7, 7 };
        int newLength = removeDuplicates(arr);

        System.out.println("Original array: " + java.util.Arrays.toString(arr));
        System.out.println("The length of the original array is: " + arr.length);
        System.out.println("After removing duplicates, the new length of the array is: " + newLength);
    }
}
