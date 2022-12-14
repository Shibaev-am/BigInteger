#pragma once

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

class BigInteger {
  public:
    void SetSign(const std::string& string);
    std::string toString();
    BigInteger();
    BigInteger(long long x);
    explicit BigInteger(const std::string& string);
    BigInteger& operator+=(const BigInteger& other);
    BigInteger& operator-=(const BigInteger& other);
    BigInteger& operator*=(const BigInteger& other);
    BigInteger& operator/=(const BigInteger& other);
    BigInteger& operator%=(const BigInteger& other);
    BigInteger& operator++();
    BigInteger operator++(int);
    BigInteger& operator--();
    BigInteger operator--(int);
    BigInteger operator-();
    BigInteger abs();
    explicit operator bool() const;
    friend std::ostream& operator<<(std::ostream& out, const BigInteger& bi);
    friend std::istream& operator>>(std::istream& in, BigInteger& bi);
    friend bool operator<(const BigInteger& lhs, const BigInteger& rhs);

  private:
    size_t block_size = 9;
    long long MAX_IN_DISCHARGE = pow(10, block_size);
    int sign_;
    std::vector<long long> data_;
    void from_string(const std::string& string, std::vector<long long>& answer);
    static int compare_modules(const BigInteger& lhs, const BigInteger& rhs);
    void module_difference(const BigInteger& lhs, const BigInteger& rhs);
    void sum_of_modules(const BigInteger& lhs, const BigInteger& rhs);
    void multiplications_of_modules(const BigInteger& lhs,
                                    const BigInteger& rhs);
    void division_of_modules(const BigInteger& lhs, const BigInteger& rhs);
    void delete_leading_zeros();
};

BigInteger operator""_bi(unsigned long long x);
BigInteger operator""_bi(const char* c, size_t n);

bool operator==(const BigInteger& lhs, const BigInteger& rhs);
bool operator!=(const BigInteger& lhs, const BigInteger& rhs);
bool operator>=(const BigInteger& lhs, const BigInteger& rhs);
bool operator>(const BigInteger& lhs, const BigInteger& rhs);
bool operator<=(const BigInteger& lhs, const BigInteger& rhs);

BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs);
BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs);
BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs);
BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs);
BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs);

class Rational {
  public:
    Rational();
    Rational(const BigInteger& numerator, const BigInteger& denominator);
    Rational(long long num);
    Rational(const BigInteger& num);
    Rational& operator+=(const Rational& other);
    Rational& operator-=(const Rational& other);
    Rational& operator/=(const Rational& other);
    Rational& operator*=(const Rational& other);
    Rational operator-();
    std::string toString();
    std::string asDecimal(size_t precision = 0);
    explicit operator double();
    friend bool operator<(const Rational& lhs, const Rational& rhs);

  private:
    BigInteger numerator_, denominator_;
    static BigInteger gcd(BigInteger a, BigInteger b);
    void normalization();
};

Rational operator+(const Rational& lhs, const Rational& rhs);
Rational operator-(const Rational& lhs, const Rational& rhs);
Rational operator*(const Rational& lhs, const Rational& rhs);
Rational operator/(const Rational& lhs, const Rational& rhs);
bool operator>=(const Rational& lhs, const Rational& rhs);
bool operator>(const Rational& lhs, const Rational& rhs);
bool operator<=(const Rational& lhs, const Rational& rhs);
bool operator==(const Rational& lhs, const Rational& rhs);
bool operator!=(const Rational& lhs, const Rational& rhs);
