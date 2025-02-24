import java.util.HashMap;
import java.util.Scanner;

public class Main {
  public static void main(String[] args) {
    Scanner scan = new Scanner(System.in);
    try {

      System.out.print("Enter the number: ");
      String input = scan.nextLine();

      extractWords(input);
      System.out.println("\nThe string only consists of numbers");
    } catch (NonDigitCharacterException e) {
      System.out.println(e.getMessage());
    }
  }

  public static void extractWords(String input) throws NonDigitCharacterException {
    // Define a HashMap of words for each digit.Works kinda like a 1:1 2D array but
    // with mostly O(1) fetching time.
    HashMap<Character, String> words = new HashMap<>();
    words.put('0', "Zero");
    words.put('1', "One");
    words.put('2', "Two");
    words.put('3', "Three");
    words.put('4', "Four");
    words.put('5', "Five");
    words.put('6', "Six");
    words.put('7', "Seven");
    words.put('8', "Eight");
    words.put('9', "Nine");

    // Iterate over each character in the input string
    for (char c : input.toCharArray()) {
      // Check if the character is a digit
      if (Character.isDigit(c)) {
        // Convert the digit to its word format
        String word = words.get(c);
        // Print the word format of the digit
        System.out.print(word + " ");
      } else {
        // Throw a custom exception if the character is not a digit
        throw new NonDigitCharacterException("\nInvalid input: non-digit character encountered");
      }
    }
  }
}

class NonDigitCharacterException extends Exception {
  public NonDigitCharacterException(String message) {
    super(message);
  }
}
