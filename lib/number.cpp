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

uint2022_t Copy(const uint2022_t& from) {
    uint2022_t ans(from.size_in_bits);
    ans.first_non_zero_bit = from.first_non_zero_bit;
    for (uint16_t i = 0; i < ans.size_in_bytes; i++) {
        ans.big_uint[i] = from.big_uint[i];
    }
    return ans;
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
        std::cout << ((a.big_uint[(i >> 3)] & kBitByID[RemainderOfDivisionBy8(i)]) == kBitByID[RemainderOfDivisionBy8(i)]);
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
    uint16_t block_id = (((value.size_in_bytes << 3) - 1) >> 3);
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
    uint2022_t ans;
    const uint2022_t base = from_uint(10);
    for (uint16_t i = 0; buff[i] != '\0'; ++i) {
        ans = ans * base + from_uint(buff[i] - '0');
    }
    return ans;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t ans;
    uint16_t maximum_bit = std::max(lhs.first_non_zero_bit, rhs.first_non_zero_bit);
    uint16_t razryad = 0;
    for (uint16_t ans_block_id = 0; ans_block_id <= (maximum_bit >> 3); ans_block_id++) {
        uint16_t result = ReverseBlock(lhs.big_uint[ans_block_id]) + ReverseBlock(rhs.big_uint[ans_block_id]) + razryad + ReverseBlock(ans.big_uint[ans_block_id]);
        ans.big_uint[ans_block_id] = ReverseBlock(RemainderOfDivisionBy256(result));
        razryad = (result >> 8);
    }
    if (razryad != 0) {
        ans.big_uint[(maximum_bit >> 3) + 1] = ReverseBlock(razryad);
    }
    DeleteFirstZeroBits(ans);
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
    uint2022_t ans = Copy(value);
    if (len == 0) {
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
    uint2022_t delt = Copy(lhs);
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
    uint2022_t ans;
    if (rhs > lhs) {
        return {ans, lhs};
    } else if (rhs == lhs) {
        return {from_uint(1), ans};
    } else if (rhs == from_uint(1)) {
        return {lhs, ans};
    }
    uint2022_t delt = Copy(lhs);
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
        stream << 0;
        return stream;
    }
    uint2022_t value_copy = Copy(value);
    const uint2022_t base_ans = from_string("1000000000000000000");
    std::stack <uint64_t> ans;
    while (value_copy > zero) {
        std::pair<uint2022_t, uint2022_t> get_ans_in_pair = DivisionWithRemainder(value_copy, base_ans);
        uint2022_t next_value = get_ans_in_pair.first;
        uint2022_t cif = get_ans_in_pair.second;
        value_copy = next_value;
        uint64_t add = 0;
        for (int16_t i = (cif.first_non_zero_bit >> 3); i >= 0; --i) {
            add = (add << 8) + ReverseBlock(cif.big_uint[i]);
        }
        ans.push(add);
    }
    bool is_first = true;
    uint64_t len_of_num = 100000000000000000;
    while (!ans.empty()) {
        if (is_first) {
            stream << ans.top();
            is_first = false;
        } else {
            if (ans.top() >= len_of_num) {
                stream << ans.top();
            } else {
                while (ans.top() < len_of_num) {
                    stream << 0;
                    len_of_num /= 10;
                }
                stream << ans.top();
                len_of_num = 100000000000000000;
            }
        }
        
        ans.pop();
    }

    return stream;
}
