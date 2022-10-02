#pragma once
#include <cinttypes>
#include <iostream>
#include <cmath>


struct uint2022_t {
    uint32_t big_uint[68] = {};

    // каждое число от 0 до 999999999, 9 цифр кодирует
    // 0 <= big_uint[i] < 1e9

    uint32_t start = 67;
    const uint32_t end = 68;
    
};

static_assert(sizeof(uint2022_t) <= 300, "Size of uint2022_t must be no higher than 300 bytes");

uint2022_t from_uint(uint32_t i);

uint2022_t from_string(const char* buff);

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs);

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value);
