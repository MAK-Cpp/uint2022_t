#include "number.h"

const uint8_t kBitByID[8] = {0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001};

void ErrorExit(int error = -1) {
    switch (error) {
      case 1: {
        std::cout << "ERROR, divising on zero!";
        exit(1);
        break;
      }
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
    uint16_t block_id = value.size_in_bytes - 1;
    while (value.big_uint[block_id] == 0 && block_id > 0) {
        block_id--;
    }
    value.first_non_zero_bit = ((block_id + 1) << 3) - 1;
    while ((value.big_uint[block_id] & kBitByID[RemainderOfDivisionBy8(value.first_non_zero_bit)]) == 0 && value.first_non_zero_bit > 0) {
        value.first_non_zero_bit--;
    }
}

uint2022_t from_uint(uint32_t i) {
    uint2022_t ans;
    uint16_t block_id = 0;
    for (; i > 0; i >>= 8, block_id++) {
        ans.big_uint[block_id] = ReverseBlock(RemainderOfDivisionBy256(i));
    }
    ans.first_non_zero_bit = (block_id << 3);
    while ((ans.big_uint[(ans.first_non_zero_bit >> 3)] & kBitByID[RemainderOfDivisionBy8(ans.first_non_zero_bit)]) == 0 && ans.first_non_zero_bit > 0) {
        ans.first_non_zero_bit--;
    }
    return ans;
}

uint2022_t from_string(const char* buff) {
    uint2022_t ans;
    const uint2022_t kBase = from_uint(10);
    for (int i = 0; buff[i] != '\0'; ++i) {
        ans = ans * kBase + from_uint(buff[i] - '0');
    }
    return ans;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t ans;
    ans.first_non_zero_bit = (((std::max(lhs.first_non_zero_bit, rhs.first_non_zero_bit) >> 3) + 1) << 3) - 1;
    uint16_t razryad = 0;
    for (uint16_t block_id = 0; block_id <= (ans.first_non_zero_bit >> 3); block_id++) {
        uint16_t result = razryad;
        result += ReverseBlock(lhs.big_uint[block_id]);
        result += ReverseBlock(rhs.big_uint[block_id]);
        result += ReverseBlock(ans.big_uint[block_id]);
        ans.big_uint[block_id] = ReverseBlock(RemainderOfDivisionBy256(result));
        razryad = (result >> 8);
    }
    if (razryad != 0) {
        ans.big_uint[(ans.first_non_zero_bit >> 3) + 1] = ReverseBlock(razryad);
    }
    DeleteFirstZeroBits(ans);
    return ans;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t ans;
    if (rhs > lhs) {
        ErrorExit();
    } else if (rhs == lhs) {
        return ans;
    }
    ans = lhs;
    for (uint16_t block_id = 0; block_id <= (ans.first_non_zero_bit >> 3); block_id++) {
        if (ReverseBlock(ans.big_uint[block_id]) >= ReverseBlock(rhs.big_uint[block_id])) {
            ans.big_uint[block_id] = ReverseBlock(ReverseBlock(ans.big_uint[block_id]) - ReverseBlock(rhs.big_uint[block_id]));
        } else {
            uint16_t bigger_block_id = block_id + 1;
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
        uint16_t razryad = 0;
        for (uint16_t rhs_block_id = 0; rhs_block_id <= (rhs.first_non_zero_bit >> 3); ++rhs_block_id) {
            uint16_t result = ReverseBlock(lhs.big_uint[lhs_block_id]);
            result *= ReverseBlock(rhs.big_uint[rhs_block_id]);
            result += razryad;
            result += ReverseBlock(ans.big_uint[lhs_block_id + rhs_block_id]);
            ans.big_uint[lhs_block_id + rhs_block_id] = ReverseBlock(RemainderOfDivisionBy256(result));
            razryad = (result >> 8);
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

uint2022_t operator<<(const uint2022_t& value, const uint16_t& len) {
    uint2022_t ans;
    if (value.first_non_zero_bit + len >= value.size_in_bits) {
        ErrorExit();
    } else if (len == 0) {
        ans = value;
        return ans;
    }
    ans.first_non_zero_bit = value.first_non_zero_bit + len;
    uint16_t where = ((len - 1) >> 3);
    uint16_t len_a = RemainderOfDivisionBy8(len);
    if (len_a == 0) {
        len_a = 8;
    }
    uint16_t len_b = 8 - len_a;
    for (uint16_t block_id = 0; block_id <= (value.first_non_zero_bit >> 3); ++block_id) {
        ans.big_uint[block_id + where + 1] += ReverseBlock(ReverseBlock(value.big_uint[block_id]) >> len_b);
        ans.big_uint[block_id + where] += ReverseBlock(ReverseBlock(value.big_uint[block_id]) << len_a);
    }
    return ans;
}

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
    return DivisionWithRemainder(lhs, rhs).first;
}


std::pair<uint2022_t, uint2022_t> DivisionWithRemainder(const uint2022_t& lhs, const uint2022_t& rhs) {
    std::pair<uint2022_t, uint2022_t> ans;
    if (rhs == from_uint(0)) {
        ErrorExit(1);
    }
    uint2022_t divised, remainder;
    if (rhs > lhs) {
        remainder = lhs;
    } else if (rhs == lhs) {
        divised = from_uint(1);
    } else if (rhs == from_uint(1)) {
        divised = lhs;
    } else {
        remainder = lhs;
        for (int16_t divised_bit_id = lhs.first_non_zero_bit - rhs.first_non_zero_bit; divised_bit_id >= 0; divised_bit_id--) {
            uint2022_t moved = (rhs << divised_bit_id);
            if (moved > remainder) {
                continue;
            }
            divised.big_uint[(divised_bit_id >> 3)] += kBitByID[RemainderOfDivisionBy8(divised_bit_id)];
            remainder = remainder - moved;
            DeleteFirstZeroBits(divised);
        }
        DeleteFirstZeroBits(divised);
    }
    ans.first = divised;
    ans.second = remainder;
    return ans;
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
        return (lhs.first_non_zero_bit > rhs.first_non_zero_bit);
    }
    for (int16_t block_id = (lhs.first_non_zero_bit >> 3); block_id >= 0; block_id--) {
        if (lhs.big_uint[block_id] != rhs.big_uint[block_id]) {
            return ReverseBlock(lhs.big_uint[block_id]) > ReverseBlock(rhs.big_uint[block_id]);
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
    const uint2022_t kZero;
    if (value == kZero) {
        stream << 0;
        return stream;
    }
    const uint2022_t kBaseAns  = from_string("1000000000000000000");
    uint2022_t v_copy = value;
    std::stack <uint64_t> parts_in_dec;
    std::pair <uint2022_t, uint2022_t> division_and_remainder;
    while (v_copy > kZero) {
        division_and_remainder = DivisionWithRemainder(v_copy, kBaseAns);
        v_copy = division_and_remainder.first;
        uint64_t add = 0;
        for (int16_t block_id = (division_and_remainder.second.first_non_zero_bit >> 3); block_id >= 0; block_id--) {
            add = (add << 8) + ReverseBlock(division_and_remainder.second.big_uint[block_id]);
        }
        parts_in_dec.push(add);
    }
    bool is_first_part = true;
    while (!parts_in_dec.empty()) {
        if (is_first_part) {
            stream << parts_in_dec.top();
            is_first_part = false;
        } else {
            if (parts_in_dec.top() == 0) {
                for (uint8_t zero = 0; zero < 17; ++zero) {
                    stream << 0;
                }
            } else {
                for (uint8_t zero = 0; zero < 18 - ((uint8_t)log10(parts_in_dec.top()) + 1); ++zero) {
                    stream << 0;
                }
            }
            stream << parts_in_dec.top();
        }
        parts_in_dec.pop();
    }
    return stream;
}
