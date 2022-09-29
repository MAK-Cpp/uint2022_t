#include "number.h"


uint2022_t from_uint(uint32_t i) {
    uint2022_t ans;
    ans.big_uint[ans.start] = i;
    return ans;
}

uint2022_t from_string(const char* buff) {
    return uint2022_t();
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    return uint2022_t();
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
    return uint2022_t();
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
    // перемножение будет больше чем uint64_t, возможно переделать под uint32_t, 
    // чтобы результат был uint_64t
    return uint2022_t();
}

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
    return uint2022_t();
}

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
    return false;
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
    return false;
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
    // stream << "Implement me";
    for (uint8_t i = value.start; i < value.end; ++i) {
        stream << value.big_uint[i];
    }
    return stream;
}
