
#include <lib/number.h>
#include <iostream>

int main() {
    uint2022_t value = from_string("999999998");
    std::cout << value << std::endl;
    // uint32_t a = 999999999;
    // uint32_t delta = 1e9;
    // uint64_t check = (uint64_t)a * a;
    // std::cout << check << '\n';
    // std::cout << check / delta << '\n';
    // std::cout << check - check / delta * delta << '\n';
    // std::cout << check / delta * 70 << '\n';
    return 0;
}
