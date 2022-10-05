#pragma once
#include <cinttypes>
#include <iostream>
#include <cmath>
#include <vector> 
#include <ctime>

#define _ << ' ' <<
#define ent '\n'

struct uint2022_t {
    // будем хранить число справа налево, т.е. чем больше
    // block_id, тем больше число
    std::vector<uint8_t> big_uint;

    uint16_t len_in_decimal;
    uint16_t first_non_zero_bit;

    uint2022_t() {
        big_uint.resize(253, 0);
        len_in_decimal = 1;
        first_non_zero_bit = 0;
    }

    uint16_t getCountOfBlocks() {
        return first_non_zero_bit / 8;
    }
    /*
    253 блока по 8 бит = 2024 бит -> 0 <= first_non_zero_bit <= 2023
    
    каждый блок - число в 8 бит, т.е. с каждым битом можно работать так:
    I. Узнать, что стоит на k-том бите одного блока
        1) = big_uint[i] & kBitByID[0];     // 128 = 0b10000000
        2) = big_uint[i] & kBitByID[1];     //  64 = 0b01000000
        3) = big_uint[i] & kBitByID[2];     //  32 = 0b00100000
        4) = big_uint[i] & kBitByID[3];     //  16 = 0b00010000
        5) = big_uint[i] & kBitByID[4];     //   8 = 0b00001000
        6) = big_uint[i] & kBitByID[5];     //   4 = 0b00000100
        7) = big_uint[i] & kBitByID[6];     //   2 = 0b00000010
        8) = big_uint[i] & kBitByID[7];     //   1 = 0b00000001

    II. Узнать k-ый бит всего числа:
        1) узнаем блок, в котором этот бит:
        block = big_uint[k / 8];
        2) узнаем номер бита:
        bit_id = (k & 7); 
        3) узнаем значение бита месте по пункту I:
        = block & kBitByID[bit_id];
    */
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

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs);

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value);
