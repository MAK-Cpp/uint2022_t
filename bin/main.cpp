
#include <lib/number.h>
#include <iostream>
// why we need iostream here if
// we have included it in 
// lib/number.h?


int main() {
    uint2022_t test_value_a = from_uint(2147483647);
    uint2022_t test_value_b = from_string("2147483647");
    std::cout << (test_value_a == test_value_b) << ent;
    std::printf("Programm ended wock with code 0, time of work: %u msc\n", clock());
    return 0;
}
