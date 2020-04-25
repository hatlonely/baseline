#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>

// #include "big_int.hpp"

typedef boost::multiprecision::cpp_int BigInt;

BigInt operator"" _bi(const char* str) {
    return BigInt(std::string(str));
}

BigInt operator^(const BigInt& a, int b) {
    return boost::multiprecision::pow(a, b);
}

BigInt S(BigInt j, BigInt n) {
    BigInt sum;
    for (BigInt k = 0; k < (n + 1); ++k) {
        sum += (16_bi ^ (n - k).convert_to<int>()) / (8_bi * k + j);
    }
    return sum;
}

BigInt PI(BigInt n) {
    return (4_bi * S(1, n) - 2_bi * S(4, n) - S(5, n) - S(6, n)) * (10_bi ^ n.convert_to<int>()) / (16_bi ^ n.convert_to<int>());
}

int main(int argc, const char* argv[]) {
    std::cout << PI(10000) << std::endl;

    return 0;
}
