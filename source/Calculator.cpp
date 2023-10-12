#include "Calculator.h"

int Calculator::Add(const int a, const int b) const
{
    return a + b;
}

int Calculator::Subtract(const int a, const int b) const
{
    return a - b;
}

int Calculator::Multiply(const int a, const int b) const
{
    return a * b;
}

int Calculator::Divide(const int a, const int b) const
{
    if (b == 0) {
        return 0; // handle division by zero
    }
    return a / b;
}
