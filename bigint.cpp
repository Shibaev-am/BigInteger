#include "biginteger.h"

BigInteger::BigInteger() : sign_(0) {
    data_.push_back(0);
}

BigInteger::BigInteger(long long x) {
    data_.clear();
    if (x > 0) {
        sign_ = 1;
    } else {
        sign_ = -1;
    }
    if (x == 0) {
        sign_ = 0;
        data_.push_back(0);
    } else {
        std::string temp = std::to_string(x);
        from_string(temp, data_);
    }
}

BigInteger::BigInteger(const std::string& string) {
    data_.clear();
    if (string[0] == '-') {
        sign_ = -1;
    } else {
        sign_ = 1;
    }
    from_string(string, data_);
    if (data_.size() == 1 && data_[0] == 0) {
        sign_ = 0;
    }
}

void BigInteger::from_string(const std::string& string,
                             std::vector<long long>& answer) {
    answer.clear();
    size_t end = string.size() + 1;
    size_t start = 0;
    if (string[0] == '-' || string[0] == '+') {
        start = 1;
    }
    for (size_t i = start; i < string.size(); ++i) {
        if (string[i] != '0') {
            end = i;
            break;
        }
    }
    if (string[end] == '+' || string[end] == '-') {
        ++end;
    }
    if (end == string.size() + 1) {
        answer.push_back(0);
    }
    for (size_t i = string.size() - 1;;) {
        std::string temp;
        for (size_t j = 0; j < block_size; ++j) {
            if (i >= j + end) {
                temp += string[i - j];
            } else {
                break;
            }
        }
        std::reverse(temp.begin(), temp.end());
        answer.push_back(std::stoi(temp));
        if (i == end || i < block_size + end) {
            break;
        }
        i -= block_size;
    }
}

BigInteger& BigInteger::operator++() {
    *this += 1;
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger copy = *this;
    *this += 1;
    return copy;
}

BigInteger& BigInteger::operator--() {
    *this -= 1;
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger copy = *this;
    *this -= 1;
    return copy;
}

BigInteger BigInteger::operator-() {
    BigInteger copy = *this;
    copy.sign_ *= -1;
    return copy;
}

BigInteger BigInteger::abs() {
    BigInteger copy = *this;
    copy.sign_ *= copy.sign_;
    return copy;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
    BigInteger temp(0);
    if (other.sign_ == sign_) {
        temp.sum_of_modules(*this, other);
        data_ = temp.data_;
    } else if (other.sign_ == -1) {
        temp.module_difference(*this, other);
        sign_ = temp.sign_;
        data_ = temp.data_;
    } else if (other.sign_ == 1) {
        temp.module_difference(other, *this);
        sign_ = temp.sign_;
        data_ = temp.data_;
    }
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
    if (this == &other) {
        data_ = {0};
        sign_ = 0;
        return *this;
    }
    BigInteger temp(0);
    if (other.sign_ == 0) {
        return *this;
    }
    if (sign_ == 0) {
        sign_ = -1 * other.sign_;
        data_ = other.data_;
        return *this;
    }
    if (sign_ != other.sign_) {
        temp.sum_of_modules(*this, other);
        data_ = temp.data_;
    } else if (sign_ == -1 && other.sign_ == -1) {
        temp.module_difference(other, *this);
        sign_ = temp.sign_;
        data_ = temp.data_;
    } else if (other.sign_ == 1) {
        temp.module_difference(*this, other);
        sign_ = temp.sign_;
        data_ = temp.data_;
    }
    return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
    if (sign_ == 0) {
        return *this;
    }
    if (other.sign_ == 0) {
        sign_ = 0;
        data_ = {0};
        return *this;
    }
    if (sign_ != other.sign_) {
        sign_ = -1;
    } else {
        sign_ = 1;
    }
    BigInteger temp;
    temp.multiplications_of_modules(*this, other);
    data_ = temp.data_;
    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
    if (sign_ == 0) {
        return *this;
    }
    if (other.sign_ == 0) {
        sign_ = 0;
        data_ = {0};
        return *this;
    }
    if (sign_ != other.sign_) {
        sign_ = -1;
    } else {
        sign_ = 1;
    }
    BigInteger temp;
    temp.division_of_modules(*this, other);
    data_ = temp.data_;
    if (data_.size() == 1 && data_[0] == 0) {
        sign_ = 0;
    }
    return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
    BigInteger temp = *this / other;
    temp *= other;
    *this -= temp;
    return *this;
}

BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger temp = lhs;
    temp %= rhs;
    return temp;
}

BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger temp = lhs;
    temp *= rhs;
    return temp;
}

BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger ans = lhs;
    ans -= rhs;
    return ans;
}

BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger ans = lhs;
    ans += rhs;
    return ans;
}

BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger ans = lhs;
    ans /= rhs;
    return ans;
}

void BigInteger::division_of_modules(const BigInteger& lhs,
                                     const BigInteger& rhs) {
    if (lhs.sign_ == 0 || compare_modules(lhs, rhs) == -1) {
        sign_ = 0;
        data_ = {0};
        return;
    }
    BigInteger remainder;
    data_.clear();
    remainder.data_.clear();
    remainder.sign_ = 1;
    for (size_t i = lhs.data_.size() - 1;; --i) {
        if (remainder.data_.size() > 1 || remainder.data_[0] != 0 ||
            remainder.data_.empty()) {
            remainder.data_.insert(remainder.data_.begin(), 0);
        }
        remainder.data_[0] = lhs.data_[i];
        remainder.delete_leading_zeros();
        if (remainder.data_.size() > 1 || remainder.data_[0] != 0) {
            remainder.sign_ = 1;
        }
        long long l = 0, r = MAX_IN_DISCHARGE - 1;
        long long curr_discharge = 0;
        while (r - l >= 0) {
            long long md = (r + l) / 2;
            BigInteger bi_md;
            bi_md = rhs * md;
            bi_md.sign_ = rhs.sign_ * rhs.sign_;
            if (compare_modules(bi_md, remainder) != 1) {
                curr_discharge = md;
                l = md + 1;
            } else {
                r = md - 1;
            }
        }
        data_.push_back(curr_discharge);
        if (rhs.sign_ >= 0) {
            remainder -= (rhs * curr_discharge);
        } else {
            remainder += (rhs * curr_discharge);
        }
        if (i == 0) {
            break;
        }
    }
    std::reverse(data_.begin(), data_.end());
    delete_leading_zeros();
}

bool operator<(const BigInteger& lhs, const BigInteger& rhs) {
    if (lhs.sign_ != rhs.sign_) {
        return lhs.sign_ < rhs.sign_;
    }
    BigInteger bi;
    if (lhs.sign_ == 1 && rhs.sign_ == 1) {
        return BigInteger::compare_modules(lhs, rhs) == -1;
    }
    if (lhs.sign_ == -1 && rhs.sign_ == -1) {
        return BigInteger::compare_modules(lhs, rhs) == 1;
    }
    return false;
}

bool operator>=(const BigInteger& lhs, const BigInteger& rhs) {
    return !(lhs < rhs);
}

bool operator>(const BigInteger& lhs, const BigInteger& rhs) {
    return (rhs < lhs);
}

bool operator<=(const BigInteger& lhs, const BigInteger& rhs) {
    return !(lhs > rhs);
}

bool operator==(const BigInteger& lhs, const BigInteger& rhs) {
    return (rhs >= lhs && lhs >= rhs);
}

bool operator!=(const BigInteger& lhs, const BigInteger& rhs) {
    return !(lhs == rhs);
}

void BigInteger::multiplications_of_modules(const BigInteger& lhs,
                                            const BigInteger& rhs) {
    if (lhs.sign_ == 0 || rhs.sign_ == 0) {
        sign_ = 0;
        data_ = {0};
        return;
    }
    data_.clear();
    long long bonus = 0;
    for (size_t i = 0; i < rhs.data_.size(); ++i) {
        for (size_t j = 0; j < lhs.data_.size(); ++j) {
            long long temp = rhs.data_[i] * lhs.data_[j] + bonus;
            if (i + j >= data_.size()) {
                data_.push_back(temp % MAX_IN_DISCHARGE);
            } else {
                temp += data_[i + j];
                data_[i + j] = (temp % MAX_IN_DISCHARGE);
            }
            bonus = temp / MAX_IN_DISCHARGE;
        }
        if (bonus > 0) {
            data_.push_back(bonus);
        }
        bonus = 0;
    }
    delete_leading_zeros();
}

void BigInteger::sum_of_modules(const BigInteger& lhs, const BigInteger& rhs) {
    const BigInteger *larger = nullptr, *lesser = nullptr;
    if (lhs.sign_ == 0 && rhs.sign_ == 0) {
        data_ = {0};
        sign_ = 0;
        return;
    }
    if (compare_modules(lhs, rhs) == 1) {
        larger = &lhs;
        lesser = &rhs;
        sign_ = 1;
    } else {
        larger = &rhs;
        lesser = &lhs;
        sign_ = 1;
    }
    data_.clear();
    long long bonus = 0;
    for (size_t i = 0; i < lesser->data_.size(); ++i) {
        if (lesser->data_[i] + bonus + larger->data_[i] >= MAX_IN_DISCHARGE) {
            data_.push_back(lesser->data_[i] + bonus + larger->data_[i] -
                            MAX_IN_DISCHARGE);
            bonus = 1;
        } else {
            data_.push_back(lesser->data_[i] + bonus + larger->data_[i]);
            bonus = 0;
        }
    }
    for (size_t i = lesser->data_.size(); i < larger->data_.size(); ++i) {
        if (larger->data_[i] + bonus >= MAX_IN_DISCHARGE) {
            data_.push_back(larger->data_[i] + bonus - MAX_IN_DISCHARGE);
        } else {
            data_.push_back(larger->data_[i] + bonus);
            bonus = 0;
        }
    }
    delete_leading_zeros();
}

void BigInteger::module_difference(const BigInteger& lhs,
                                   const BigInteger& rhs) {
    const BigInteger *larger = nullptr, *lesser = nullptr;
    if (compare_modules(lhs, rhs) == 1) {
        larger = &lhs;
        lesser = &rhs;
        sign_ = 1;
    } else if (compare_modules(lhs, rhs) == -1) {
        larger = &rhs;
        lesser = &lhs;
        sign_ = -1;
    } else {
        sign_ = 0;
        data_ = {0};
        return;
    }
    data_.clear();
    long long credit = 0;
    for (size_t i = 0; i < lesser->data_.size(); ++i) {
        if (larger->data_[i] - credit >= lesser->data_[i]) {
            data_.push_back(larger->data_[i] - credit - lesser->data_[i]);
            credit = 0;
        } else {
            data_.push_back(MAX_IN_DISCHARGE + larger->data_[i] - credit -
                            lesser->data_[i]);
            credit = 1;
        }
    }
    for (size_t i = lesser->data_.size(); i < larger->data_.size(); ++i) {
        if (larger->data_[i] - credit < 0) {
            data_.push_back(MAX_IN_DISCHARGE + larger->data_[i] - credit);
        } else {
            data_.push_back(larger->data_[i] - credit);
            credit = 0;
        }
    }
    delete_leading_zeros();
}

int BigInteger::compare_modules(const BigInteger& lhs, const BigInteger& rhs) {
    if (lhs.data_.size() < rhs.data_.size()) {
        return -1;
    }
    if (lhs.data_.size() > rhs.data_.size()) {
        return 1;
    }
    for (size_t i = lhs.data_.size() - 1;; --i) {
        if (lhs.data_[i] < rhs.data_[i]) {
            return -1;
        }
        if (lhs.data_[i] > rhs.data_[i]) {
            return 1;
        }
        if (i == 0) {
            return 0;
        }
    }
}

void BigInteger::delete_leading_zeros() {
    while (data_[data_.size() - 1] == 0) {
        if (data_.size() == 1) {
            sign_ = 0;
            return;
        }
        data_.erase(data_.end() - 1);
    }
}

std::ostream& operator<<(std::ostream& out, const BigInteger& bi) {
    if (bi.sign_ == -1) {
        out << "-";
    }
    for (size_t i = bi.data_.size() - 1;; --i) {
        std::string curr, temp = std::to_string(bi.data_[i]);
        if (i != bi.data_.size() - 1) {
            for (size_t j = temp.size(); j < 9; ++j) {
                curr += "0";
            }
        }
        curr += temp;
        out << curr;
        if (i == 0) {
            break;
        }
    }
    return out;
}

std::istream& operator>>(std::istream& in, BigInteger& bi) {
    std::string s;
    in >> s;
    bi.SetSign(s);
    bi.from_string(s, bi.data_);
    if (bi.data_.size() == 1 && bi.data_[0] == 0) {
        bi.sign_ = 0;
    }
    return in;
}

void BigInteger::SetSign(const std::string& string) {
    if (string[0] == '-') {
        sign_ = -1;
    } else {
        sign_ = 1;
    }
}

std::string BigInteger::toString() {
    if (sign_ == 0) {
        return "0";
    }
    std::string ans;
    if (sign_ == -1) {
        ans = "-";
    }
    for (size_t i = data_.size() - 1;; --i) {
        std::string temp = std::to_string(data_[i]);
        if (i != data_.size() - 1) {
            for (size_t j = temp.size(); j < 9; ++j) {
                ans += '0';
            }
        }
        ans += temp;
        if (i == 0) {
            break;
        }
    }
    return ans;
}

BigInteger operator""_bi(unsigned long long x) {
    std::string s = std::to_string(x);
    return BigInteger(s);
}

BigInteger operator""_bi(const char* c, size_t /*unused*/) {
    return BigInteger(c);
}

BigInteger::operator bool() const {
    return sign_ != 0;
}

Rational::Rational() : numerator_(0), denominator_(1) {}

Rational::Rational(const BigInteger& numerator, const BigInteger& denominator)
    : numerator_(numerator), denominator_(denominator) {
    if (denominator < 0) {
        numerator_ = -numerator_;
        denominator_ = -denominator_;
    }
    normalization();
}

Rational::Rational(const BigInteger& num) : numerator_(num), denominator_(1) {}

Rational::Rational(long long num) : numerator_(num), denominator_(1) {}

BigInteger Rational::gcd(BigInteger a, BigInteger b) {
    if (a < 0) {
        a = -a;
    }
    while (a != 0 && b != 0) {
        if (a > b) {
            a %= b;
        } else {
            b %= a;
        }
    }
    return (a + b);
}

void Rational::normalization() {
    if (numerator_ != 0 && denominator_ != 0) {
        BigInteger curr_gcd = gcd(numerator_, denominator_);
        numerator_ /= curr_gcd;
        denominator_ /= curr_gcd;
    } else {
        numerator_ = 0;
        denominator_ = 1;
    }
}

Rational& Rational::operator+=(const Rational& other) {
    numerator_ *= other.denominator_;
    numerator_ += (denominator_ * other.numerator_);
    denominator_ *= other.denominator_;
    normalization();
    return *this;
}

Rational& Rational::operator-=(const Rational& other) {
    numerator_ *= other.denominator_;
    numerator_ -= (denominator_ * other.numerator_);
    denominator_ *= other.denominator_;
    normalization();
    return *this;
}

Rational& Rational::operator*=(const Rational& other) {
    numerator_ *= other.numerator_;
    denominator_ *= other.denominator_;
    normalization();
    return *this;
}

Rational& Rational::operator/=(const Rational& other) {
    numerator_ *= other.denominator_;
    denominator_ *= other.numerator_;
    if (denominator_ < 0) {
        numerator_ = -numerator_;
        denominator_ = -denominator_;
    }
    normalization();
    return *this;
}

Rational Rational::operator-() {
    return {(-numerator_), denominator_};
}

std::string Rational::toString() {
    std::string ans;
    ans += numerator_.toString();
    if (denominator_ != 1) {
        ans += "/";
        ans += denominator_.toString();
    }
    return ans;
}

std::string Rational::asDecimal(size_t precision) {
    std::string temp(precision, '0');
    BigInteger prec("1" + temp);
    std::string ans;
    if (numerator_ < 0) {
        ans += "-";
    }
    ans += (numerator_.abs() / denominator_).toString();
    BigInteger frac = numerator_.abs() % denominator_;
    if (frac != 0 && precision > 0) {
        ans += ".";
        frac *= prec;
        frac /= denominator_;
        temp = frac.toString();
        for (size_t i = temp.size(); i < precision; ++i) {
            ans += "0";
        }
        ans += temp;
    }
    while (ans[ans.size() - 1] == '0') {
        if (ans.size() == 1) {
            break;
        } else {
            ans.erase(ans.end() - 1);
        }
    }
    if (ans[ans.size() - 1] == '.') {
        ans.erase(ans.end() - 1);
    }
    return ans;
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
    Rational temp = lhs;
    temp += rhs;
    return temp;
}

Rational operator-(const Rational& lhs, const Rational& rhs) {
    Rational temp = lhs;
    temp -= rhs;
    return temp;
}

Rational operator*(const Rational& lhs, const Rational& rhs) {
    Rational temp = lhs;
    temp *= rhs;
    return temp;
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    Rational temp = lhs;
    temp /= rhs;
    return temp;
}

bool operator<(const Rational& lhs, const Rational& rhs) {
    Rational temp = (lhs - rhs);
    return temp.numerator_ < 0;
}

bool operator>=(const Rational& lhs, const Rational& rhs) {
    return !(lhs < rhs);
}

bool operator>(const Rational& lhs, const Rational& rhs) {
    return rhs < lhs;
}

bool operator<=(const Rational& lhs, const Rational& rhs) {
    return !(rhs < lhs);
}

bool operator==(const Rational& lhs, const Rational& rhs) {
    return !(lhs > rhs || lhs < rhs);
}

bool operator!=(const Rational& lhs, const Rational& rhs) {
    return !(lhs == rhs);
}

Rational::operator double() {
    std::string temp = asDecimal(10);
    double x = std::stod(temp);
    return x;
}
