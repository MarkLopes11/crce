abstract class Shape 
{
    public abstract double getArea(); 
    public abstract double getPerimeter();
}

class Circle extends Shape 
{
    private double radius; 
    
    
    public Circle(double radius) 
    {
        this.radius = radius;
    }

    public double getArea() 
    {
        return Math.PI * radius * radius; 
    }

    public double getPerimeter() 
    {
        return 2 * Math.PI * radius; 
    }
}

class Rectangle extends Shape 
{
    private double length;
    private double breadth;
    public Rectangle(double length, double breadth) 
    {
        this.length = length;
        this.breadth = breadth;
    }

    public double getArea() 
    {
        return length * breadth;
    }

    public double getPerimeter() 
    {
        return 2 * (length + breadth);
    }
}
public class q4
{
    public static void main(String[] args) 
    {
        Circle circle = new Circle(7);
        System.out.println("Circle area: " + circle.getArea());
        System.out.println("Circle perimeter: " + circle.getPerimeter());
        Rectangle rectangle = new Rectangle(5, 10);
        System.out.println("Rectangle area: " + rectangle.getArea());
        System.out.println("Rectangle perimeter: " + rectangle.getPerimeter());
    }
}