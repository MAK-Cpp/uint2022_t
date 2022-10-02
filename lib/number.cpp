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
    std::cout << ans.big_uint[ans.start] << std::endl;
    uint32_t len = 0;
    for (uint32_t count = 0; buff[count] != '\0'; ++count) {
        len++;
    }
    if (len > ans.end * 9) {
        errorExit();
    }
    uint32_t remainder = len % 9;
    ans.start = ans.end - (len + 9 - 1) / 9;

    std::cout << len << ' ' << ans.start << std::endl;

    for (uint32_t i = 0, block_id = ans.start, i_remainder = 0; buff[i] != '\0'; ++i, i_remainder++) {
        std::cout << i << ' ' << buff[i] << ' ' << block_id << std::endl;
        if (i_remainder == 9) {
            i_remainder = 0;
        }
        if (i_remainder == remainder) {
            block_id++;
        }
        ans.big_uint[block_id] = ans.big_uint[block_id] * 10 + (buff[i] - '0');
        std::cout << ans.big_uint[block_id] << std::endl;
    }
    return ans;
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
    for (uint8_t i = value.start; i < value.end; ++i) {
        if (i != value.start) {
            if (value.big_uint[i] >= 1e8) {
                stream << value.big_uint[i];
            } else {
                for (int i = log10(value.big_uint[i]) + 1; i < 9; ++i) {
                    stream << 0;
                }
                stream << value.big_uint[i];
            }
        } else {
            stream << value.big_uint[i];
        }
    }
    return stream;
}
