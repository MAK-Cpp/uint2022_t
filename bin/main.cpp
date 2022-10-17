
#include <lib/number.h>
#include <iostream>
// why we need iostream here if
// we have included it in 
// lib/number.h?


int main() {
    uint2022_t a = from_string("0");
    // uint2022_t b = from_uint(0);
    
    uint2022_t b = from_string("1230000000000000000000000321");
                                

    EXTRACOUT(a);
    EXTRACOUT(b << 3);
    EXTRACOUT(b);
    // EXTRACOUT(a + a);
    // EXTRACOUT(b + b);    
    // 111111100101101110101000011001101110011100100111011011101110001110000000000000000101000001
    // 111111100101101110101000011001101110011100100111011011101110001110000000000000000101000001000

    std::cout << a << '\n';
    std::cout << b << '\n';
    std::cout << (a + b) << '\n';
    std::cout << a * b << '\n';
    

    std::printf("Programm ended wock with code 0, time of work: %u msc\n", clock());
    return 0;
}
