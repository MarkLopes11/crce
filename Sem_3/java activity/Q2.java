class CreditCard
{
    String cardNumber;
    int expiry;
    CreditCard(String cardNumber,int expiry)
    {
        this.cardNumber=cardNumber; 
        this.expiry=expiry;
      
    }
    
    public String getCardNumber() 
    {
        return cardNumber; 
    }

   public int getExpiry() 
    {
        return expiry;
    }
}

class Person
{
    CreditCard cc; 
    String name;
    float balance;
    
    Person(CreditCard cc,String name,float balance)
    {
        this.cc=cc; 
        this.name=name;
        this.balance=balance;
    }

    public void displayInfo() 
    {
        System.out.println("Name: "+name);
        System.out.println("Balance: "+balance);
        System.out.println("Card Number: "+cc.getCardNumber()); 
        System.out.println("Expiry date: "+cc.getExpiry());
    }
}

public class Q2
{
    public static void main(String[] args) 
    {
        CreditCard cc=new CreditCard("1000-1001-1002",2026);
        Person pe=new Person(cc,"Mark",5000);
        pe.displayInfo();
    }
}