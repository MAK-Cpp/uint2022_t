
#include <lib/number.h>
#include <iostream>

int main() {
    uint2022_t value = from_string("999999999");
    uint2022_t del = from_string("12345678987654322");
    uint2022_t test = from_string("9999999999999999999");

    uint32_t a = 999999999;

    std::cout << (value * test == test * value) << std::endl;

    return 0;
}
