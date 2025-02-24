#include <stdio.h>

// complex number separated into numerator and denominator part
typedef struct
{
  float num;
  float denom;
} Rational;

// sets the values of complex number separately into num and denom part
void setData(Rational *a)
{
  printf("Please enter the numerator and denominator of the number  :    ");
  scanf(" %f %f", &(a->num), &(a->denom));
}

// Add 2 complex numbers.'z' points to (Rational)result in main
void addRational(Rational a, Rational b, Rational *z)
{
  (*z).num = (a.num * b.denom) + (b.num * a.denom);
  (*z).denom = a.denom * b.denom;
}

// Subtract 2 complex numbers.'z' points to (Rational)result in main
void subRational(Rational a, Rational b, Rational *z)
{
  (*z).num = (a.num * b.denom) - (b.num * a.denom);
  (*z).denom = a.denom * b.denom;
}

// Multiply 2 complex numbers.'z' points to (Rational)result in main
void mulRational(Rational a, Rational b, Rational *z)
{
  (*z).num = a.num * b.num;
  (*z).denom = a.denom * b.denom;
}

// gets the result depending on the operator.'result' points to (Rational)result in main
void getResult(Rational a, Rational b, Rational *result)
{
  char c;
  printf("Enter the operator(+/-/*)  :    ");
  scanf(" %c", &c);
  switch (c)
  {
  case '+':
    addRational(a, b, result);
    break;
  case '-':
    subRational(a, b, result);
    break;
  case '*':
    mulRational(a, b, result);
    break;
  default:
    printf("Error: Enter a valid operator\n");
  }
}

// display value based on result
void display(Rational x)
{
  if (x.denom == 0.0)
  {
    printf("Error: The denominator of the result is 0\n");
  }
  else if (x.num == 0.0)
  {
    printf("The result of the operation is 0\n");
  }
  else
  {
    printf("The result of the operation is %.2f/%.2f\n", x.num, x.denom);
  }
}

int main()
{
  Rational r1;
  Rational r2;
  Rational result;

  setData(&r1);
  setData(&r2);

  getResult(r1, r2, &result);
  display(result);
  // can use display(getResult(r1, r2, &result))
  return 0;
}