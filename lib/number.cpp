#include "number.h"

const uint8_t kBitByID[8] = {0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001};

void ErrorExit(int error = -1) {
    switch (error) {
      default: {
        std::cout << "ERROR: Undefined Behavior";
        exit(-1);
        break;
      }
    }
}

uint8_t RemainderOfDivisionBy8(const uint16_t& number) {
    return (number & 7);
}

uint8_t RemainderOfDivisionBy256(const uint16_t& number) {
    return (number & 255);
}

void EXTRACOUT(const uint2022_t& a) {
    std::cout << "EXTRA COUT: ";
    for (int16_t i = a.first_non_zero_bit; i >= 0; i--) {
        // std::cout << (uint16_t)a.big_uint[(i >> 3)] << ' ' << (uint16_t)kBitByID[RemainderOfDivisionBy8(i)] << ' ' << (uint16_t)RemainderOfDivisionBy8(i) << '\n';
        std::cout << ((a.big_uint[(i >> 3)] & kBitByID[RemainderOfDivisionBy8(i)]) == kBitByID[RemainderOfDivisionBy8(i)]);
        // std::cout << '\n';
    }
    std::cout << '\n';
}


uint8_t ReverseBlock(const uint8_t& block) {
    uint8_t ans = 0;
    for (uint8_t i = 0; i < 8; ++i) {
        if ((block & kBitByID[i]) == kBitByID[i]) {
            ans += kBitByID[7 - i];
        } 
    }
    return ans;
}

void DeleteFirstZeroBits(uint2022_t& value) {
    // std::cout << "--- START DELETE FIRST ZERO BITS ---\n";
    uint16_t block_id = value.size_in_bytes - 1;
    while (value.big_uint[block_id] == 0 && block_id > 0) {
        block_id--;
    }
    value.first_non_zero_bit = ((block_id + 1) << 3) - 1;
    while ((value.big_uint[block_id] & kBitByID[RemainderOfDivisionBy8(value.first_non_zero_bit)]) == 0 && value.first_non_zero_bit > 0) {
        value.first_non_zero_bit--;
    }
    if (value.first_non_zero_bit >= value.size_in_bits) {
        ErrorExit();
    }
    // EXTRACOUT(value);
    // std::cout << "--- END DELETE FIRST ZERO BITS ---\n";
}

uint2022_t from_uint(uint32_t i) {
    uint2022_t ans;
    if (i == 0) {
        return ans;
    }
    int16_t block_id = 0;
    for (;i > 0; i = (i >> 8), block_id++) {
        ans.big_uint[block_id] = ReverseBlock(RemainderOfDivisionBy256(i));
    }
    block_id--;
    ans.first_non_zero_bit = ((block_id + 1) << 3) - 1;
    while ((ans.big_uint[block_id] & kBitByID[RemainderOfDivisionBy8(ans.first_non_zero_bit)]) == 0 && ans.first_non_zero_bit > 0) {
        ans.first_non_zero_bit--;
    }
    return ans;
}

uint2022_t from_string(const char* buff) {
    std::cout << "<-> START FROM STRING <->\n";
    uint2022_t ans;
    const uint2022_t base = from_uint(10);
    for (uint16_t i = 0; buff[i] != '\0'; ++i) {
        std::cout << buff[i];
    }
    std::cout << '\n';
    for (uint16_t i = 0; buff[i] != '\0'; ++i) {
        // std::cout << i + 1 << ") buff[i] = " << buff[i] << '\n';
        // EXTRACOUT(from_uint(buff[i] - '0'));
        // EXTRACOUT(ans * base);
        ans = ans * base + from_uint(buff[i] - '0');
        EXTRACOUT(ans);
    }
    // DeleteFirstZeroBits(ans);
    EXTRACOUT(ans);
    std::cout << "<-> END FROM STRING <->\n";
    return ans;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t ans;
    uint16_t maximum_block = std::max((lhs.first_non_zero_bit >> 3), (rhs.first_non_zero_bit >> 3));
    // std::cout << lhs.first_non_zero_bit << ' ' << rhs.first_non_zero_bit << ' ' << maximum_block << '\n';
    uint16_t razryad = 0;
    // std::cout << "<<< START OPERATOR + >>> \n";
    // EXTRACOUT(ans);
    // EXTRACOUT(lhs);
    // EXTRACOUT(rhs);
    for (uint16_t ans_block_id = 0; ans_block_id <= maximum_block; ans_block_id++) {
        uint16_t result =   razryad + 
                            ReverseBlock(lhs.big_uint[ans_block_id]) + 
                            ReverseBlock(rhs.big_uint[ans_block_id]) + 
                            ReverseBlock(ans.big_uint[ans_block_id]);
        // std::cout   << ans_block_id << ' ' 
                    // << (uint16_t)ReverseBlock(lhs.big_uint[ans_block_id]) << ' ' 
                    // << (uint16_t)ReverseBlock(rhs.big_uint[ans_block_id]) << ' '
                    // << razryad << ' '
                    // << (uint16_t)ReverseBlock(ans.big_uint[ans_block_id]) << ' ' 
                    // << result << '\n';
        ans.big_uint[ans_block_id] = ReverseBlock(RemainderOfDivisionBy256(result));
        razryad = (result >> 8);
    }
    if (razryad != 0) {
        ans.big_uint[maximum_block + 1] = ReverseBlock(razryad);
    }
    // EXTRACOUT(ans);
    DeleteFirstZeroBits(ans);
    // std::cout << "<<< END OPERATOR + >>> \n";
    return ans;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
    if (rhs > lhs) {
        ErrorExit();
    }
    uint2022_t ans;
    if (rhs == lhs) {
        return ans;
    }
    ans = lhs;
    // lhs > rhs 100%
    for (uint16_t block_id = 0; block_id <= (ans.first_non_zero_bit >> 3); block_id++) {
        if (ReverseBlock(ans.big_uint[block_id]) >= ReverseBlock(rhs.big_uint[block_id])) {
            ans.big_uint[block_id] = ReverseBlock(ReverseBlock(ans.big_uint[block_id]) - ReverseBlock(rhs.big_uint[block_id]));
        } else {
            int16_t bigger_block_id = block_id + 1;
            while (ans.big_uint[bigger_block_id] == 0) {
                ans.big_uint[bigger_block_id] = 255;
                bigger_block_id++;
            }
            ans.big_uint[bigger_block_id] = ReverseBlock(ReverseBlock(ans.big_uint[bigger_block_id]) - 1);
            ans.big_uint[block_id] = ReverseBlock(256 + ReverseBlock(ans.big_uint[block_id]) - ReverseBlock(rhs.big_uint[block_id]));
        }
    }
    DeleteFirstZeroBits(ans);
    return ans;
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t ans;
    if (lhs.first_non_zero_bit + rhs.first_non_zero_bit >= ans.size_in_bits) {
        ErrorExit();
    }
    for (uint16_t lhs_block_id = 0; lhs_block_id <= (lhs.first_non_zero_bit >> 3); ++lhs_block_id) {
        uint8_t razryad = 0;
        for (uint16_t rhs_block_id = 0; rhs_block_id <= (rhs.first_non_zero_bit >> 3); ++rhs_block_id) {
            uint16_t rezult = ReverseBlock(lhs.big_uint[lhs_block_id]) * ReverseBlock(rhs.big_uint[rhs_block_id]) + razryad + ReverseBlock(ans.big_uint[lhs_block_id + rhs_block_id]);
            ans.big_uint[lhs_block_id + rhs_block_id] = ReverseBlock(RemainderOfDivisionBy256(rezult));
            razryad = (rezult >> 8);
        }
        if (razryad != 0) {
            if (lhs_block_id + (rhs.first_non_zero_bit >> 3) + 1 >= ans.size_in_bits) {
                ErrorExit();
            }
            ans.big_uint[lhs_block_id + (rhs.first_non_zero_bit >> 3) + 1] = ReverseBlock(razryad);
        } 
    }
    DeleteFirstZeroBits(ans);
    return ans;
}

uint2022_t operator^(const uint2022_t& lhs, const uint32_t& rhs) {
    uint2022_t ans = from_uint(1);
    for (uint32_t i = 0; i < rhs; ++i) {
        ans = ans * lhs;
    }
    return ans;
}

uint2022_t operator<<(const uint2022_t& value, const uint16_t& len) {
    uint2022_t ans = value;
    if (ans == from_uint(0) || len == 0) {
        return ans;
    }
    ans.first_non_zero_bit += len;
    if (ans.first_non_zero_bit >= ans.size_in_bits) {
        ErrorExit();
    }
    for (uint16_t i = ans.first_non_zero_bit; i >= len; i--) {
        if ((ans.big_uint[((i - len) >> 3)] & kBitByID[RemainderOfDivisionBy8(i - len)]) == kBitByID[RemainderOfDivisionBy8(i - len)]) {
            ans.big_uint[((i - len) >> 3)] -= kBitByID[RemainderOfDivisionBy8(i - len)];
            if ((ans.big_uint[((i) >> 3)] & kBitByID[RemainderOfDivisionBy8(i)]) == 0) {
                ans.big_uint[((i) >> 3)] += kBitByID[RemainderOfDivisionBy8(i)];
            }
        }
    }
    return ans;
}

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t ans;
    if (rhs > lhs) {
        return ans;
    } else if (rhs == lhs) {
        return from_uint(1);
    } else if (rhs == from_uint(1)) {
        return lhs;
    }
    uint2022_t delt = lhs;
    while (delt.first_non_zero_bit >= rhs.first_non_zero_bit) {
        
        uint16_t ans_bit_id = (delt.first_non_zero_bit - rhs.first_non_zero_bit);
        if ((rhs << ans_bit_id) > delt) {
            if (ans_bit_id == 0) {
                break;
            }
            ans_bit_id--;
        }
        ans.big_uint[(ans_bit_id >> 3)] += kBitByID[RemainderOfDivisionBy8(ans_bit_id)];
        delt = delt - (rhs << ans_bit_id);
    }
    DeleteFirstZeroBits(ans);
    return ans;
}


std::pair<uint2022_t, uint2022_t> DivisionWithRemainder(const uint2022_t& lhs, const uint2022_t& rhs) {
    std::cout << ">> START DIVISION WITH REMAINDER << \n";
    uint2022_t ans;
    if (rhs > lhs) {
        return {ans, lhs};
    } else if (rhs == lhs) {
        return {from_uint(1), ans};
    } else if (rhs == from_uint(1)) {
        return {lhs, ans};
    }
    uint2022_t delt = lhs;
    EXTRACOUT(ans);
    EXTRACOUT(delt);
    EXTRACOUT(lhs);
    EXTRACOUT(rhs);
    while (delt.first_non_zero_bit >= rhs.first_non_zero_bit) {
        
        uint16_t ans_bit_id = (delt.first_non_zero_bit - rhs.first_non_zero_bit);
        if ((rhs << ans_bit_id) > delt) {
            if (ans_bit_id == 0) {
                break;
            }
            ans_bit_id--;
        }
        ans.big_uint[(ans_bit_id >> 3)] += kBitByID[RemainderOfDivisionBy8(ans_bit_id)];
        delt = delt - (rhs << ans_bit_id);
    }
    DeleteFirstZeroBits(ans);
    std::cout << ">> END DIVISION WITH REMAINDER << \n";
    return {ans, delt};
}

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
    if (lhs.first_non_zero_bit != rhs.first_non_zero_bit) {
        return false;
    }
    for (int16_t block_id = (lhs.first_non_zero_bit >> 3); block_id >= 0; block_id--) {
        if (lhs.big_uint[block_id] != rhs.big_uint[block_id]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
    return !(lhs == rhs);
}

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs) {
    if (lhs.first_non_zero_bit != rhs.first_non_zero_bit) {
        return lhs.first_non_zero_bit > rhs.first_non_zero_bit;
    }
    for (int16_t block_id = (lhs.first_non_zero_bit >> 3); block_id >= 0; block_id--) {
        if (lhs.big_uint[block_id] != rhs.big_uint[block_id]) {
            return ReverseBlock(lhs.big_uint[block_id]) > ReverseBlock(rhs.big_uint[block_id]);
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
    const uint2022_t zero;
    if (value == zero) {
        // std::cout << "value == zero \n";
        stream << 0;
        return stream;
    }
    EXTRACOUT(value);
    
    uint2022_t value_copy = value;
    EXTRACOUT(value_copy);
    EXTRACOUT(value_copy);
    const uint2022_t base_ans = from_string("1000000000000000000");
    EXTRACOUT(value_copy);
    std::stack <uint64_t> ans;
    EXTRACOUT(value_copy);
    while (value_copy > zero) {
        EXTRACOUT(value_copy);
        std::pair<uint2022_t, uint2022_t> get_ans_in_pair = DivisionWithRemainder(value_copy, base_ans);
        uint2022_t next_value = get_ans_in_pair.first;
        uint2022_t cif = get_ans_in_pair.second;
        EXTRACOUT(next_value);
        EXTRACOUT(cif);
        value_copy = next_value;
        uint64_t add = 0;
        for (int16_t i = (cif.first_non_zero_bit >> 3); i >= 0; --i) {
            add = (add << 8) + ReverseBlock(cif.big_uint[i]);
        }
        ans.push(add);
    }
    bool is_first = true;
    // uint64_t len_of_num = 100000000000000000;
    while (!ans.empty()) {
        std::cout << ans.top() << '\n';
        if (is_first) {
            stream << ans.top();
            is_first = false;
        } else {
            if (ans.top() == 0) {
                for (uint8_t i = 0; i < 17; ++i) {
                    stream << 0;
                }
            } else {
                for (uint8_t i = 0; i < 18 - ((uint8_t)log10(ans.top()) + 1); ++i) {
                    stream << 0;
                }
            }
            stream << ans.top();
            is_first = false;
        }
        
        ans.pop();
    }

    return stream;
}
