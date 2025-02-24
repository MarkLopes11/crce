import java.util.*;

class Circle{
	static double area(double rad){
		return rad*rad*Math.PI;
	}
}

class DemoCircle{
	public static void main(String args[]){
		Scanner sc = new Scanner(System.in);
		System.out.print("Enter the radius of the circle :\t");
		double rad = sc.nextDouble();
		System.out.println("The area of the circle is :\t"+ Circle.area(rad));
	}
}
