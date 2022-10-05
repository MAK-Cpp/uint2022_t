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

// void addToBit(uint2022_t& value, const uint16_t& bit, uint16_t add) {
//     uint16_t razryad = 0;
//     for (uint16_t i = bit; i < 253 * 8 && (add > 0 || razryad > 0); i++) {
//         if ((add & 1) == 0) {
//             if (razryad == 1) {
//                 if ((value.big_uint[i / 8] & kBitByID[(i & 7)]) == 0) {
//                     // 0 + 1 + 0 = 1, razryad = 0
//                     value.big_uint[i / 8] += kBitByID[(i & 7)];
//                     razryad = 0;
//                 } else {
//                     // 0 + 1 + 1 = 0, razryad = 1
//                     value.big_uint[i / 8] -= kBitByID[(i & 7)];
//                 }
//             }
//             // 0 + 0 + x = x, razryad = 0
//         } else { // (add & 1) == 1
//             if (razryad == 0) {
//                 if ((value.big_uint[i / 8] & kBitByID[(i & 7)]) == 0) {
//                     // 1 + 0 + 0 = 1, razryad = 0
//                     value.big_uint[i / 8] += kBitByID[(i & 7)];
//                 } else {
//                     // 1 + 0 + 1 = 0, razryad = 1
//                     value.big_uint[i / 8] -= kBitByID[(i & 7)];
//                     razryad = 1;
//                 }
//             }
//             // 1 + 1 + x = x, razryad = 1
//         }
//         add /= 2;
//         if ((value.big_uint[i / 8] & kBitByID[(i & 7)]) == kBitByID[(i & 7)] && i > value.first_non_zero_bit) {
//             value.first_non_zero_bit = i;
//         }
//     }
//     if (add > 0 || razryad > 0) {
//         std::cout << "addToBit\n";
//         errorExit();
//     }
// }

uint8_t reverseBlock(const uint8_t& block) {
    uint8_t ans = 0;
    for (uint8_t i = 0; i < 8; ++i) {
        if ((block & kBitByID[i]) == kBitByID[i]) {
            ans += kBitByID[7 - i];
        } 
    }
    return ans;
}

uint2022_t from_uint(uint32_t i) {
    uint2022_t ans;
    if (i == 0) {
        return ans;
    }
    for (;i > 0; i = (i >> 1), ans.first_non_zero_bit++) {
        if (i & 1 == 1) {
            ans.big_uint[ans.first_non_zero_bit / 8] += kBitByID[ans.first_non_zero_bit & 7];
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
        // std::cout << ans << ent;
    }
    return ans;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t ans;
    uint16_t maximum_bit = std::max(lhs.first_non_zero_bit, rhs.first_non_zero_bit);
    uint16_t razryad = 0;
    for (uint16_t ans_block_id = 0; ans_block_id <= (maximum_bit + 8 - 1) / 8; ans_block_id++) {
        uint16_t result = reverseBlock(lhs.big_uint[ans_block_id]) + reverseBlock(rhs.big_uint[ans_block_id]) + razryad + reverseBlock(ans.big_uint[ans_block_id]);
        ans.big_uint[ans_block_id] = reverseBlock(result & 255);
        razryad = result / 256;
    }
    if (razryad != 0) {
        ans.big_uint[maximum_bit / 8 + 1] = reverseBlock(razryad);
    }
    ans.first_non_zero_bit = ans.big_uint.size() * 8 - 1;
    while (ans.big_uint[ans.first_non_zero_bit / 8] == 0 && ans.first_non_zero_bit > 7) {
        ans.first_non_zero_bit -= 8;
    }
    while ((ans.big_uint[ans.first_non_zero_bit / 8] & kBitByID[(ans.first_non_zero_bit & 7)]) == 0 && ans.first_non_zero_bit > 0) {
        ans.first_non_zero_bit--;
    }
    if (ans.first_non_zero_bit >= 2022) {
        errorExit();
    }
    return ans;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
    return uint2022_t();
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t ans;
    if (lhs.first_non_zero_bit + rhs.first_non_zero_bit >= 2022) {
        std::cout << "OPERATOR * ERROR\n";
        // std::cout << lhs.first_non_zero_bit _ rhs.first_non_zero_bit << ent;
        errorExit();
    }
    for (uint16_t lhs_block_id = 0; lhs_block_id <= lhs.first_non_zero_bit / 8; ++lhs_block_id) {
        // std::printf("check 3\n");
        uint8_t razryad = 0;
        for (uint16_t rhs_block_id = 0; rhs_block_id <= rhs.first_non_zero_bit / 8; ++rhs_block_id) {
            uint16_t rezult = reverseBlock(lhs.big_uint[lhs_block_id]) * reverseBlock(rhs.big_uint[rhs_block_id]) + razryad + reverseBlock(ans.big_uint[lhs_block_id + rhs_block_id]);
            ans.big_uint[lhs_block_id + rhs_block_id] = reverseBlock((rezult & 255));
            razryad = rezult / 256;
        }
        if (razryad != 0) {
            if (lhs_block_id + rhs.first_non_zero_bit / 8 + 1 >= 2022) {
                errorExit();
            }
            ans.big_uint[lhs_block_id + rhs.first_non_zero_bit / 8 + 1] = reverseBlock(razryad);
        } 
    }
    ans.first_non_zero_bit = ans.big_uint.size() * 8 - 1;
    while (ans.big_uint[ans.first_non_zero_bit / 8] == 0 && ans.first_non_zero_bit > 7) {
        ans.first_non_zero_bit -= 8;
    }
    while ((ans.big_uint[ans.first_non_zero_bit / 8] & kBitByID[(ans.first_non_zero_bit & 7)]) == 0 && ans.first_non_zero_bit > 0) {
        ans.first_non_zero_bit--;
    }
    if (ans.first_non_zero_bit >= 2022) {
        errorExit();
    }
    /*
    uint16_t maximum_bit = lhs.first_non_zero_bit + rhs.first_non_zero_bit;
    for (uint16_t ans_bit_id = 0; ans_bit_id <= maximum_bit; ans_bit_id++) {
        // bit_id - степень двойки, поэтому начинается с нуля
        uint16_t count_to_add = 0;
        for (uint16_t lhs_bit_id = ans_bit_id, rhs_bit_id = 0; rhs_bit_id <= ans_bit_id; lhs_bit_id--, rhs_bit_id++) {
            if ((lhs.big_uint[lhs_bit_id / 8] & kBitByID[(lhs_bit_id & 7)]) == kBitByID[(lhs_bit_id & 7)] && 
                (rhs.big_uint[rhs_bit_id / 8] & kBitByID[(rhs_bit_id & 7)]) == kBitByID[(rhs_bit_id & 7)]) {
                count_to_add++;
            }
        }
        addToBit(ans, ans_bit_id, count_to_add);
    }
    */
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
        if ((lhs.big_uint[bit_id / 8] & kBitByID[(bit_id & 7)]) != (rhs.big_uint[bit_id / 8] & kBitByID[(bit_id & 7)])) {
            return (lhs.big_uint[bit_id / 8] & kBitByID[(bit_id & 7)]) > (rhs.big_uint[bit_id / 8] & kBitByID[(bit_id & 7)]);
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
    // вывод в двоичном формате, переделать
    
    const bool is_in_binary = true;
    if (is_in_binary) {
        // stream << "\nTEST OUT IN BINARY BASE, REMAKE\n";

        for (int16_t i = value.first_non_zero_bit; i >= 0; i--) {
            stream << ((value.big_uint[i / 8] & kBitByID[(i & 7)]) == kBitByID[(i & 7)]);
        }
    }

    return stream;
}
