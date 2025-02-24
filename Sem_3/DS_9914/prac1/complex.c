#include <stdio.h>
// complex number separated into real and imaginary part
typedef struct
{
  float real;
  float imag;
} Num;
// sets the values of complex number by separating into real and imaginary
void setData(Num *a)
{
  printf("Please enter real and imaginary part of the number: ");
  scanf(" %f %f", &(a->real), &(a->imag));
}
// Adds complex numbers and return Num
Num addComplex(Num x, Num y)
{
  Num z;
  z.real = x.real + y.real;
  z.imag = x.imag + y.imag;
  return z;
}
// subtracts complex numbers and returns Num
Num subComplex(Num x, Num y)
{
  Num z;
  z.real = x.real - y.real;
  z.imag = x.imag - y.imag;
  return z;
}
// gets the result depending on the operator

Num getResult(Num a, Num b)
{
  char c;
  printf("Enter the operator(+/-) : ");
  scanf(" %c", &c);
  switch (c)
  {
  case '+':
    return addComplex(a, b);
  case '-':
    return subComplex(a, b);
  default:
    printf("Error: Enter a valid operator");
    return (Num){0.0, 0.0}; // return a zero complex number
  }
}

// display value based on result
void display(Num x)
{
  if (x.imag == 0)
  {
    printf("The result of the operation is %.2f", x.real);
  }
  else if (x.imag > 0)
  {
    printf("The result of the operation is %.2f + %.2fi", x.real, x.imag);
  }
  else
  {
    printf("The result of the operation is %.2f%.2fi", x.real, x.imag);
  }
}

int main()
{

  Num c1;
  Num c2;
  Num result;
  setData(&c1);
  setData(&c2);
  result = getResult(c1, c2);
  display(result);
  return 0;
}
