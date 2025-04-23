#include <math.h>
#include "fp32_custom_sine.h"
// Custom sine implementation
float fp32_custom_sine(float x)
{
    x = fmodf(x, 2.0f * (float)M_PI);
    if (x > (float)M_PI)
        x -= 2.0f * (float)M_PI;
    else if (x < -(float)M_PI)
        x += 2.0f * (float)M_PI;
    float result = 0.0f;
    float term = x;
    float x_squared = x * x;
    int sign = 1;
    for (int n = 1; n <= 7; n += 2)
    {
        result += sign * term;
        sign = -sign;
        term = term * x_squared;
        term = term / (float)(n + 1);
        term = term / (float)(n + 2);
    }
    return result;
}