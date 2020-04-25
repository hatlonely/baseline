#include <iostream>

#include "big_int.hpp"

BigInt S(BigInt j, BigInt n) {
    BigInt sum;
    for (BigInt k = 0; k < (n + 1); ++k) {
        sum += (16_bi ^ (n - k)) / (8_bi * k + j);
    }
    return sum;
}

BigInt PI(BigInt n) {
    return (4_bi * S(1, n) - 2_bi * S(4, n) - S(5, n) - S(6, n)) * (10_bi ^ n) / (16_bi ^ n);
}

int main(int argc, const char* argv[]) {
    std::cout << PI(100) << std::endl;

    return 0;
}
