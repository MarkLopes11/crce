class Car {
    int speed, noOfGear;

    public void drive(int speed, int noOfGear) {
        this.speed = speed;
        this.noOfGear = noOfGear;
    }

    public void display() {
        System.out.println("Speed is : " + speed + " kmph");
        System.out.println("No. of Gears are : " + noOfGear);
    }
}

class SportCar extends Car {
    String airBallonType;

    public void drive(int speed, int noOfGear, String airBallonType) {
        super.drive(speed, noOfGear);
        this.airBallonType = airBallonType;
        System.out.println();
        System.out.println("Initial speed of Sport Car : " + speed + "kmph");
        System.out.println("Initial No. of Gears in Sport Car are : " + noOfGear);
    }

    public void display() {
        System.out.println();
        System.out.println("Sport Car : ");
        super.display();
        System.out.println("The Air Balloon Type is : " + airBallonType);
    }
}

public class Q1 {
    public static void main(String[] args) {
        Car car = new Car();
        car.drive(160, 4);
        System.out.println("Car : ");
        car.display();
        SportCar sportCar = new SportCar();
        sportCar.drive(400, 8, "Helium");
        sportCar.display();
    }
}
