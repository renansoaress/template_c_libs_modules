#include <stdio.h>
#include "test.h"
#include "lib/mylib/mylib.h"
#include "lib/mylib2/mylib2.h"
#include "lib/legal/legal.h"
#include "modules/oi/m_oi.h"

int main()
{
    printf("AAA RENAN SOARES!!!\n");
    int a = 5, b = 3;
    printf("Add: %d\n", add(a, b));
    printf("Subtract: %d\n", subtract(a, b));
    printf("Multiply: %d\n", multiply(a, b));
    printf("Divide: %d\n", divide(a, b));

    modulo_oi();
    teste();
    testesMyLIB();
    auto_lib();
    return 0;
}
