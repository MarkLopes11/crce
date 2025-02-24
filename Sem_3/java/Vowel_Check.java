import java.util.Scanner;

public class Vowel_Check {
  public static void main(String[] args) {
    Scanner scan = new Scanner(System.in);
    System.out.print("Enter the string: ");
    try {
      String text = scan.nextLine();
      System.out.println("Original string: " + text);
      checkVowels(text);
      // the next line will execute only if there are vowels
      System.out.println("String contains vowels.");
    } catch (NoVowelsException e) {
      // print error
      System.out.println("Error: " + e.getMessage());
    }
  }

  public static void checkVowels(String text) throws NoVowelsException {
    boolean containsVowels = false;
    String vowels = "aeiouAEIOU";

    // disintegrate the string and check if any character of the string is a vowel
    for (int i = 0; i < text.length(); i++) {
      char ch = text.charAt(i);
      if (vowels.contains(String.valueOf(ch))) {
        containsVowels = true;
        break;
      }
    }
    // if no vowels then throw error
    if (!containsVowels) {
      throw new NoVowelsException("String does not contain any vowels.");
    }
  }
}

// custom exception class
class NoVowelsException extends Exception {
  public NoVowelsException(String message) {
    super(message);
  }
}
