class Employee
{
    String name,position;
    int id;
    
    Employee(String name,int id,String position)
    {
        this.name=name;
        this.id=id;
        this.position=position;
    }
    
    public void displayInfo()
    {
        System.out.println("Employee name: "+name);
        System.out.println("Employee ID: "+id);
        System.out.println("Employee position: "+position);
    }
}
    
class ContractEmployee extends Employee
{
    String work;
    float salary;
ContractEmployee(String name,int id,String position,String work,float salary){
super(name,id,position);
this.work=work;
this.salary=salary;
}
public void displayInfo() {
super.displayInfo();
System.out.println("Area of work: "+work);
System.out.println("Monthly salary: "+salary);
}
}
class PermanentEmployee extends Employee{
String fire,perfomance;
PermanentEmployee(String name,int id,String position,String work,float salary,String fire,String perfomance){
super(name,id,position);
this.fire=fire;
this.perfomance=perfomance;
}
public void displayInfo() {
super.displayInfo();
System.out.println("Employee perfomance: "+perfomance);
System.out.println("Should we fire him? "+fire);
}
}
public class Question3 {
public static void main(String[] args) {
Employee e=new Employee("Shwen",9881,"SDE 1");
e.displayInfo();
System.out.println("-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
ContractEmployee ce=new ContractEmployee("Rohit",9900,"SDE 1","Database",50000);
ce.displayInfo();
System.out.println("-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
PermanentEmployee pe=new PermanentEmployee("Ronit",9909,"SDE 1","FrontEnd",50000,"No","Excellent");
pe.displayInfo();
}
}