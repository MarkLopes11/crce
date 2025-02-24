class Result {
    private String name;
    private int seatno;
    private String date;
    private int marks;
    private int centerNo;

    public Result(String name, int seatno, String date, int centerNo, int marks) throws MarksOutOfBoundsException {
        if (marks < 0 || marks > 100) {
            throw new MarksOutOfBoundsException("Marks cannot be less than 0 or greater than 100");
        }
        this.name = name;
        this.seatno = seatno;
        this.date = date;
        this.centerNo = centerNo;
        this.marks = marks;
    }

    public void display() {
        System.out.println("Name of the student is : " + name);
        System.out.println("Seat Number of the student is : " +
                seatno);
        System.out.println("Date of the exam is : " + date);
        System.out.println("Center Number of the student is : " + centerNo);
        System.out.println("Marks of the student is : " + marks);
    }
}

public class Q2 {
    public static void main(String[] args) {
        try {
            Result result = new Result("Mark", 157898, "24-10-2023", 9913, 99);
            result.display();
        } catch (MarksOutOfBoundsException e) {
            System.out.println("Error : " + e.getMessage());
        }
    }
}

class MarksOutOfBoundsException extends Exception {
    public MarksOutOfBoundsException(String message) {
        super(message);
    }
}
