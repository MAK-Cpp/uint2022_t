#include "number.h"

const uint32_t KBaseOfNumericSystem = 1e9;

const uint2022_t KMaxOfStruct = {{481560, 916771158, 684800786, 922703235, 625631274, 322714142, 263414417, 884163925, 873322306, 437689024, 231009526, 751394401, 758326916, 367106052, 34484602, 375642882, 110959089, 521812209, 947069992, 139877256, 8949136, 579813164, 413834190, 131240610, 432508865, 633901300, 457687591, 589632190, 325582710, 683886781, 973951695, 733384278, 544896131, 740867054, 246692573, 31629150, 247882082, 682647773, 168904426, 336814855, 367810693, 467547461, 780797071, 163567159, 452928068, 892906992, 787178135, 839959347, 223507647, 240845924, 670958716, 173279750, 751341651, 541295792, 537288393, 481542519, 773223140, 547524361, 834615428, 274169543, 954961376, 881442030, 303829940, 191406452, 725012875, 774576546, 969913778, 507874304}, 0};

void errorExit(int error = -1) {
    switch (error) {
      default: {
        std::cout << "ERROR: Undefined Behavior";
        exit(-1);
        break;
      }
    }
}

uint2022_t from_uint(uint32_t i) {
    uint2022_t ans;
    ans.big_uint[ans.start] = i;
    return ans;
}

uint2022_t from_string(const char* buff) {
    // один блок - 9 цифр -> buff / 9 с округлением вверх =
    // = кол-ву блоков, если buff % 9 == 0 все легко, иначе
    // первый блок - [0; buff % 9 - 1], остальные так же как 
    // до этого
    uint2022_t ans;
    uint32_t len = 0;
    for (uint32_t count = 0; buff[count] != '\0'; ++count) {
        len++;
    }
    if (len > ans.end * 9) {
        errorExit();
    }
    uint32_t remainder = len % 9;
    ans.start = ans.end - (len + 9 - 1) / 9;

    for (uint32_t i = 0, block_id = ans.start, i_remainder = 0; buff[i] != '\0'; ++i, i_remainder++) {
        if (i_remainder == 9) {
            i_remainder = 0;
        }
        if (i != 0 && i_remainder == remainder) {
            block_id++;
        }
        ans.big_uint[block_id] = ans.big_uint[block_id] * 10 + (buff[i] - '0');
    }
    if (ans > KMaxOfStruct || ans == KMaxOfStruct) {
        errorExit();
    }
    return ans;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t ans;
    ans.start = std::min(lhs.start, rhs.start);
    for (uint32_t i = ans.end - 1; i > ans.start; i--) {
        ans.big_uint[i] += lhs.big_uint[i] + rhs.big_uint[i];
        if (i > 0 && ans.big_uint[i] > KBaseOfNumericSystem) {
            ans.big_uint[i - 1]++;
            ans.big_uint[i] -= KBaseOfNumericSystem;
            if (i == ans.start) {
                ans.start--;
            }
        }
    }
    if (ans.start == 0) {
        uint32_t i = 0;
        ans.big_uint[i] += lhs.big_uint[i] + rhs.big_uint[i];
        if (i > 0 && ans.big_uint[i] > KBaseOfNumericSystem) {
            ans.big_uint[i - 1]++;
            ans.big_uint[i] -= KBaseOfNumericSystem;
            if (i == ans.start) {
                ans.start--;
            }
        }
    }
    if (ans > KMaxOfStruct || ans == KMaxOfStruct) {
        return ans - KMaxOfStruct;
    }
    return ans;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t ans;
    if (lhs > rhs) {
        ans.start = lhs.start;
        for (uint32_t i = ans.start; i < ans.end; ++i) {
            ans.big_uint[i] = lhs.big_uint[i];
            if (ans.big_uint[i] >= rhs.big_uint[i]) {
                ans.big_uint[i] -= rhs.big_uint[i];
            } else {
                uint32_t j = i - 1;
                while (j > 0 && ans.big_uint[j] == 0) {
                    j--;
                }
                ans.big_uint[j++]--;
                while (j < i) {
                    ans.big_uint[j++] = KBaseOfNumericSystem - 1;
                }
                ans.big_uint[i] = (KBaseOfNumericSystem - rhs.big_uint[i]) + ans.big_uint[i];
            }
            if (ans.big_uint[ans.start] == 0) {
            ans.start++;
        }
        }
        return ans;
    } else if (lhs == rhs) {
        return ans;
    } else {
        return KMaxOfStruct - (rhs - lhs);
    }
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
    if (lhs.start != rhs.start) {
        return false;
    }
    for (uint32_t i = lhs.start; i < lhs.end; ++i) {
        if (lhs.big_uint[i] != rhs.big_uint[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
    if (lhs.start != rhs.start) {
        return true;
    }
    for (uint32_t i = lhs.start; i < lhs.end; ++i) {
        if (lhs.big_uint[i] != rhs.big_uint[i]) {
            return true;
        }
    }
    return false;
}

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs) {
    if (lhs.start != rhs.start) {
        return lhs.start < rhs.start;
    }
    for (uint32_t i = lhs.start; i < lhs.end; ++i) {
        if (lhs.big_uint[i] != rhs.big_uint[i]) {
            return lhs.big_uint[i] > rhs.big_uint[i];
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
    for (uint8_t i = value.start; i < value.end; ++i) {
        if (i != value.start) {
            if (value.big_uint[i] >= 1e8) {
                stream << value.big_uint[i];
                // stream << std::endl;
            } else {
                uint32_t num_of_len = 1;
                uint32_t len = 1;
                while (num_of_len < value.big_uint[i]) {
                    num_of_len *= 10;
                    len++;
                }
                if (value.big_uint[i] != num_of_len) {
                    len--;
                }
                // std::cout << len << std::endl;
                for (uint32_t i = 0; i < 9 - len; ++i) {
                    stream << 0;
                }
                stream << value.big_uint[i];
                // stream << std::endl;
            }
        } else {
            stream << value.big_uint[i];
            // stream << std::endl;
        }
    }
    return stream;
}
