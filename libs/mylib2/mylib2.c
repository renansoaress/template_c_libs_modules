#include "mylib2.h"

int multiply(int a, int b)
{
    return a * b;
}

int divide(int a, int b)
{
    if (b != 0)
    {
        return a / b;
    }
    else
    {
        return 0; // ou algum valor indicativo de erro
    }
}
