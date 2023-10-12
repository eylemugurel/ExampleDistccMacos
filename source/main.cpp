#include <iostream>
#include "Calculator.h"

int main()
{
    Calculator calculator;

    std::cout << "5 + 3 = " << calculator.Add(5, 3) << std::endl;
    std::cout << "5 - 3 = " << calculator.Subtract(5, 3) << std::endl;
    std::cout << "5 * 3 = " << calculator.Multiply(5, 3) << std::endl;
    std::cout << "5 / 3 = " << calculator.Divide(5, 3) << std::endl;

    return 0;
}
