#include <iostream>
extern "C" {
    #include "fp32_custom_sine.h"
}

int main(void)
{
    auto t = fp32_custom_sine(1.4);
    std::cout << t << std::endl;

    return 0;
}