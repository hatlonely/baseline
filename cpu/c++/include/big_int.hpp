#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>

const int kMaxExp = 3;
const int64_t kFactor = pow(10, kMaxExp);

class BigInt {
    bool sign = true;
    std::vector<int64_t> vi;
    
  public:
    BigInt() {}

    BigInt(int64_t i) {
        if (i < 0) {
            sign = false;
            i = - i;
        }
        while (i != 0) {
            vi.push_back(i % kFactor);
            i /= kFactor;
        }
    }

    BigInt(const std::string& str) {
        int e = 0;
        if (str[0] == '-') {
            sign = false;
            e = 1;
        } else if (str[0] == '+') {
            sign = true;
            e = 1;
        }
        for (int i = str.length(); i > e; i -= kMaxExp) {
            auto j = std::max(e, i - kMaxExp);
            vi.push_back(std::stoll(str.substr(j, i - j)));
        }
    }

    BigInt(const BigInt&& bi) {
        vi = std::move(bi.vi);
    }

    BigInt(const BigInt& bi) {
        vi = bi.vi;
    }
    
    BigInt& operator=(const BigInt& bi) {
        if (&bi == this) {
            return *this;
        }
        
        vi = bi.vi;
        return *this;
    }
    
    std::string ToString() const {
        std::stringstream ss;
        for (auto it = vi.crbegin(); it != vi.crend(); ++it) {
            ss << std::setw(3) << std::setfill('0') << (*it);
        }
        auto str = ss.str();
        auto idx = std::find_if(str.begin(), str.end(), [](char ch) {return ch != '0';});
        auto res = str.substr(idx - str.begin(), str.length());
        if (res.empty()) {
            return "0";
        }
        
        if (!sign) {
            return "-" + res;
        }
        return res;
    }

    operator std::string() const { 
        return ToString();
    }

    BigInt operator+(const BigInt& bi) {
        BigInt b = BigInt(*this);
        b += bi;
        return b;
    }

    BigInt operator-(const BigInt& bi) const {
        BigInt b(*this);
        b -= bi;
        return b;
    }

    BigInt& operator+=(const BigInt& bi) {
        if (sign) {
            if (bi.sign) {
                return AddAbs(bi);
            }
            if (CompareAbs(bi) > 0) {
                return SubAbs(bi);
            }
            BigInt tmp(bi);
            tmp.SubAbs(*this);
            vi = std::move(tmp.vi);
            sign = false;
            return *this;
        }
        if (!bi.sign) {
            return AddAbs(bi);
        }
        if (CompareAbs(bi) > 0) {
            return SubAbs(bi);
        }
        BigInt tmp(bi);
        tmp.SubAbs(*this);
        vi = std::move(tmp.vi);
        sign = true;
        return *this;
    }

    BigInt& operator-=(const BigInt& bi) {
        if (sign) {
            if (!bi.sign) {
                return AddAbs(bi);
            }
            if (CompareAbs(bi) > 0) {
                return SubAbs(bi);
            }
            BigInt tmp(bi);
            tmp.SubAbs(*this);
            vi = std::move(tmp.vi);
            sign = false;
            return *this;
        }
        if (bi.sign) {
            return AddAbs(bi);
        }
        if (CompareAbs(bi) > 0) {
            return SubAbs(bi);
        }
        BigInt tmp(bi);
        tmp.SubAbs(*this);
        vi = std::move(tmp.vi);
        sign = true;
        return *this;
    }

    BigInt operator*(const BigInt& bi) {
        auto res = MulAbs(bi);
        res.sign = !(sign ^ bi.sign);
        return res;
    }

    BigInt& operator++() {
        return AddShiftAbs(1, 0);
    }

    BigInt operator^(const BigInt& bi) {
        BigInt res(1);
        
        for (BigInt i = 0; i < bi; ++i) {
            res = (*this) * res;
        }
        
        return res;
    }
    
    BigInt operator/(const BigInt& bi) {
        
    }

    BigInt MulAbs(const BigInt& bi) {
        BigInt res;

        for (int i = 0; i < bi.vi.size(); i++) {
            BigInt tmp = 0;
            for (int j = 0; j < vi.size(); j++) {
                tmp.AddShiftAbs(bi.vi[i] * vi[j], j);
            }
            res.AddShiftAbs(tmp, i);
        }

        return res;
    }

    BigInt& SubAbs(const BigInt& bi) {
        return SubShiftAbs(bi, 0);
    }

    BigInt& SubShiftAbs(const BigInt& bi, int n) {
        if (bi.vi.empty()) {
            return *this;
        }

        BigInt b;
        b.vi = std::vector<int64_t>(bi.vi.size() + n, 0);
        for (int i = 0; i < bi.vi.size(); i++) {
            b.vi[i+n] = bi.vi[i];
        }

        int idx = 0;
        int64_t carry = 0;
        while (idx < b.vi.size()) {
            int64_t m = vi[idx] - b.vi[idx] - carry;
            if (m < 0) {
                carry = 1;
                m = kFactor + m;
            } else {
                carry = 0;
            }
            vi[idx] = m;
            idx++;
        }

        for (int i = vi.size() - 1; i >= 0; i--) {
            if (vi[i] == 0) {
                vi.pop_back();
            } else {
                break;
            }
        }

        return *this;
    }

    BigInt& AddAbs(const BigInt& bi) {
        return AddShiftAbs(bi, 0);
    }

    BigInt& AddShiftAbs(const BigInt& bi, int n) {
        if (bi.vi.empty()) {
            return *this;
        }

        while (vi.size() < n) {
            vi.push_back(0);
        }

        int idx = n;
        int64_t carry = 0;
        while (idx < bi.vi.size() + n && idx < vi.size()) {
            int64_t m = bi.vi[idx - n] + vi[idx] + carry;
            vi[idx] = m % kFactor;
            carry = m / kFactor;
            idx++;
        }
        while (idx < vi.size()) {
            int64_t m = vi[idx] + carry;
            vi[idx] = m % kFactor;
            carry = m / kFactor;
            idx++;
        }
        while (idx < bi.vi.size() + n) {
            int64_t m = bi.vi[idx - n] + carry;
            vi.emplace_back(m % kFactor);
            carry = m / kFactor;
            idx++;
        }
        if (carry != 0) {
            vi.emplace_back(carry);
        }
        return *this;
    }

    int CompareAbs(const BigInt& bi) const {
        if (vi.size() < bi.vi.size()) {
            return -1;
        }
        if (vi.size() > bi.vi.size()) {
            return 1;
        }

        for (int i = vi.size() - 1; i >= 0; i--) {
            if (vi[i] == bi.vi[i]) {
                continue;
            }
            return vi[i] - bi.vi[i];
        }
        return 0;
    }

    int Compare(const BigInt& bi) const {
        if (sign) {
            if (bi.sign) {
                return CompareAbs(bi);
            } else {
                return 1;
            }
        }
        if (bi.sign) {
            return -1;
        }
        return -CompareAbs(bi);
    }

    bool operator==(const BigInt& bi) const {
        return Compare(bi) == 0;
    }

    bool operator< (const BigInt& bi) const {
        return Compare(bi) < 0;
    }

    bool operator>(const BigInt& bi) const {
        return Compare(bi) > 0;
    }

    bool operator>= (const BigInt& bi) const {
        return !(*this < bi);
    }

    bool operator<= (const BigInt& bi) const {
        return !(*this > bi);
    }
};

BigInt operator"" _bi(const char* str) {
    return BigInt(std::string(str));
}

std::ostream& operator<< (std::ostream& out, const BigInt& b) {
    out << b.ToString();
    return out;
}
