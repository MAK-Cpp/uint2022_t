
#include <lib/number.h>
#include <iostream>
// why we need iostream here if
// we have included it in 
// lib/number.h?


int main() {
    uint2022_t a = from_uint(2147483647);
    uint2022_t b = from_string("2147483647");

    std::cout << a << ent << b << ent << (a + b) << ent;


    std::printf("Programm ended wock with code 0, time of work: %u msc\n", clock());
    return 0;
}
