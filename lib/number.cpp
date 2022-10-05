#include "number.h"

const uint8_t kBitByID[8] = {0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001};


void errorExit(int error = -1) {
    switch (error) {
      default: {
        std::cout << "ERROR: Undefined Behavior";
        exit(-1);
        break;
      }
    }
}

uint8_t reverseBlock(const uint8_t& block) {
    uint8_t ans = 0;
    for (uint8_t i = 0; i < 8; ++i) {
        if ((block & kBitByID[i]) == kBitByID[i]) {
            ans += kBitByID[7 - i];
        } 
    }
    return ans;
}

void deleteFirstZeroBits(uint2022_t& value) {
    uint16_t block_id = (((value.big_uint.size() << 3) - 1) >> 3);
    while (value.big_uint[block_id] == 0 && block_id > 0) {
        block_id--;
    }
    value.first_non_zero_bit = (block_id << 3) + 7;
    while ((value.big_uint[block_id] & kBitByID[(value.first_non_zero_bit & 7)]) == 0 && value.first_non_zero_bit > 0) {
        value.first_non_zero_bit--;
    }
    if (value.first_non_zero_bit >= 2022) {
        errorExit();
    }
}

uint2022_t from_uint(uint32_t i) {
    uint2022_t ans;
    if (i == 0) {
        return ans;
    }
    for (;i > 0; i = (i >> 1), ans.first_non_zero_bit++) {
        if (i & 1 == 1) {
            ans.big_uint[(ans.first_non_zero_bit >> 3)] += kBitByID[ans.first_non_zero_bit & 7];
        }
    }
    ans.first_non_zero_bit--;

    return ans;
}

uint2022_t from_string(const char* buff) {
    uint2022_t ans;
    const uint2022_t step = from_uint(10);
    for (uint16_t i = 0; buff[i] != '\0'; ++i) {
        uint2022_t add_number = from_uint(buff[i] - '0');
        ans = ans * step + add_number;
    }
    return ans;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t ans;
    uint16_t maximum_bit = std::max(lhs.first_non_zero_bit, rhs.first_non_zero_bit);
    uint16_t razryad = 0;
    for (uint16_t ans_block_id = 0; ans_block_id <= ((maximum_bit + 7) >> 3); ans_block_id++) {
        uint16_t result = reverseBlock(lhs.big_uint[ans_block_id]) + reverseBlock(rhs.big_uint[ans_block_id]) + razryad + reverseBlock(ans.big_uint[ans_block_id]);
        ans.big_uint[ans_block_id] = reverseBlock(result & 255);
        razryad = (result >> 8);
    }
    if (razryad != 0) {
        ans.big_uint[maximum_bit / 8 + 1] = reverseBlock(razryad);
    }
    deleteFirstZeroBits(ans);
    return ans;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
    return uint2022_t();
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t ans;
    if (lhs.first_non_zero_bit + rhs.first_non_zero_bit >= 2022) {
        std::cout << "OPERATOR * ERROR\n";
        errorExit();
    }
    for (uint16_t lhs_block_id = 0; lhs_block_id <= (lhs.first_non_zero_bit >> 3); ++lhs_block_id) {
        uint8_t razryad = 0;
        for (uint16_t rhs_block_id = 0; rhs_block_id <= (rhs.first_non_zero_bit >> 3); ++rhs_block_id) {
            uint16_t rezult = reverseBlock(lhs.big_uint[lhs_block_id]) * reverseBlock(rhs.big_uint[rhs_block_id]) + razryad + reverseBlock(ans.big_uint[lhs_block_id + rhs_block_id]);
            ans.big_uint[lhs_block_id + rhs_block_id] = reverseBlock((rezult & 255));
            razryad = (rezult >> 8);
        }
        if (razryad != 0) {
            if (lhs_block_id + (rhs.first_non_zero_bit >> 3) + 1 >= 2022) {
                errorExit();
            }
            ans.big_uint[lhs_block_id + (rhs.first_non_zero_bit >> 3) + 1] = reverseBlock(razryad);
        } 
    }
    deleteFirstZeroBits(ans);
    return ans;
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

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs) {
    if (lhs.first_non_zero_bit != rhs.first_non_zero_bit) {
        return lhs.first_non_zero_bit > rhs.first_non_zero_bit;
    }
    for (int16_t bit_id = lhs.first_non_zero_bit; bit_id >= 0; bit_id--) {
        if ((lhs.big_uint[(bit_id >> 3)] & kBitByID[(bit_id & 7)]) != (rhs.big_uint[(bit_id >> 3)] & kBitByID[(bit_id & 7)])) {
            return (lhs.big_uint[(bit_id >> 3)] & kBitByID[(bit_id & 7)]) > (rhs.big_uint[(bit_id >> 3)] & kBitByID[(bit_id & 7)]);
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
    // вывод в двоичном формате, переделать
    
    const bool is_in_binary = true;
    if (is_in_binary) {
        for (int16_t i = value.first_non_zero_bit; i >= 0; i--) {
            stream << ((value.big_uint[(i >> 3)] & kBitByID[(i & 7)]) == kBitByID[(i & 7)]);
        }
    }

    return stream;
}
