#include "bigInt.hpp"

// ---------------------------------------------------------------------------------
BigInt::BigInt(const BigInt& other)
{
    digits.resize(other.size());
    for (int i = 0; i < other.size(); ++i)
    {
        digits[i] = other.digits[i];
    }
}

// ---------------------------------------------------------------------------------
BigInt::BigInt (const std::string &str)
{
    int len = str.size();
    digits.resize(len);
    for (int k = 0; k < len; ++k)
    {
        char c = str[len - 1 - k];
        digits[k] = atoi(&c);
    }
}

// ---------------------------------------------------------------------------------
BigInt::BigInt(long n)
{
    const int base = 10;
    while (n >= 1)
    {   
        int thisDigit = n % base;
        digits.push_back(thisDigit);
        n /= 10;
        //std::cout << "thisDigit: " << thisDigit << ", n: " << n << std::endl;
    }
};



// ---------------------------------------------------------------------------------
void BigInt::print()
{
    for (int k = 0; k < digits.size(); ++k)
    {
        printf("digits[%d]: %d\n", k, digits[k]);
    }
}

// ---------------------------------------------------------------------------------
void BigInt::printStr()
{
    std::cout << toString() << std::endl;
}

// ---------------------------------------------------------------------------------
bool BigInt::operator==(BigInt& other) 
{
    redistribute();
    other.redistribute();
    if (digits.size() != other.digits.size())
    {
        return false;
    }
    for (int idx = 0; idx < digits.size(); ++idx)
    {
        if (digits[idx] != other.digits[idx])
        {
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------------
BigInt& BigInt::operator=(const BigInt& other)
{
    digits.resize(other.digits.size());
    for (int i = 0; i < digits.size(); ++i)
    {
        digits[i] = other.digits[i];
    }
    return *this;
}

// ---------------------------------------------------------------------------------
long BigInt::toLong() 
{
    int ret = 0;
    for (int p = 0; p < digits.size(); ++p)
    {
        ret += digits[p] * pow(10, p);
    }
    return ret;
}

// ---------------------------------------------------------------------------------
BigInt& BigInt::fromLong(long n)
{
    *this = BigInt(n);
    return *this;
}

// SLOW: 
// ---------------------------------------------------------------------------------
BigInt& BigInt::exponential(long p, bool print)
{
    long tot = p;
    float pct = 0;
    long cnt = 0;
    BigInt m(*this);
    while(--p > 0)
    {
        ++cnt;
        m *= *this;
        
        if (print)
        {
            pct = 100 * float(tot-p)/tot; 
            printf("cnt: %ld, len: %zu, %0.2f%%\n", cnt, m.size(), pct);
            //std::cout << "\r" << p;
        }
    }
    *this = m;
    return *this;
}

// does same as above, but converts to int first for speed
// ---------------------------------------------------------------------------------
BigInt& BigInt::exponentialInt(long p, bool print)
{
    long tot = p;
    float pct = 0;
    long cnt = 0;
    long N = toLong();
    while(--p > 0)
    {
        ++cnt;
        *this *= N;

        if (print)
        {
            pct = 100 * float(tot-p)/tot;
            printf("cnt: %ld, len: %zu, %0.2f%%\n", cnt, size(), pct);
            //std::cout << "\r" << p;
        }
    }
    return *this;
}

// does same as above, but converts to long int first for speed, and tries to minimize
// the number of multiplications that it does
// ---------------------------------------------------------------------------------
BigInt& BigInt::exponentialIntFast(long p, bool print)
{
    long N = toLong();

    //use the fact that if p = m*n N^p = (N^m)^n,
    //you can do m times fewer multiplications
    
    long m = 1;
    long NtoTheM = N;
    //make sure that N*digits[k] does not overflow!
    std::vector<long> factors = primeFactors(p);
    if (!factors.empty())
    {
        //naive:
        int idx = 0;
        const int LIMIT = INT_MAX/10;

        //guarantees that pow(NtoTheM, factors[idx]) < LIMIT
        while ( factors[idx] < log(LIMIT)/log(NtoTheM) && idx < factors.size())
        {
            m *= factors[idx];
            NtoTheM = pow(NtoTheM, factors[idx]);
            printf("factor: %ld; m: %ld; NtoTheM: %ld; log(%d)/log(NtoTheM): %0.3f\n", factors[idx], m, NtoTheM, LIMIT, log(LIMIT)/log(NtoTheM));
            ++idx;
        }
        
        printf("omitting %ld: pow(%ld, %ld) = %0.2f\n", factors[idx], NtoTheM, factors[idx], pow(NtoTheM, factors[idx]));
        printf("N: %ld; p: %ld --> m: %ld; NtoTheM: %ld\n",
                N, p, m, NtoTheM);
    }

    //recall N^p = (N^m)^n
    long n = p / m;
    *this = fromLong(NtoTheM);
    
    long tot = n;
    float pct = 0;
    long cnt = 0;
    while(--n > 0)
    {
        ++cnt;
        *this *= NtoTheM; 
        
        if (print)
        {
            pct = 100 * float(tot-n)/tot; 
            printf("cnt: %ld, len: %zu, %0.2f%%\n", cnt, size(), pct);
            //std::cout << "\r" << p;
        }
    }
    return *this;
}

// ---------------------------------------------------------------------------------
std::string BigInt::toString()
{
    std::string s(size(), 'x');
    for (int k = 0; k < size(); ++k)
    {
        s[size() - 1 - k] = char(digits[k] + '0');
    }

    return s;
} 

// ---------------------------------------------------------------------------------
BigInt& BigInt::operator+=(const BigInt &other)
{
    for (int k = 0; k < digits.size(); ++k)
    {
        if (k < digits.size() && k < other.digits.size())
        {
            digits[k] += other.digits[k];
        }
    }
    redistribute();
    return *this;
}

// ---------------------------------------------------------------------------------
BigInt BigInt::operator+(const BigInt& n)
{
    BigInt ret = *this;
    ret += n;
    return ret;
}

// ---------------------------------------------------------------------------------
BigInt& BigInt::operator*=(const BigInt &other)
{
    std::vector<int> c( size() + other.size(), 0 );
    
    for (int k = 0; k < c.size(); ++k)
    {
        for (int j = 0; j <= k; ++j)
        {
            if ( (j < digits.size()) && ((k-j) < other.digits.size() ) )
            {
                c[k] += digits[j] * other.digits[k-j];
            }
        }

    }
    
    digits = c;
    redistribute();

    return *this;
}



// ---------------------------------------------------------------------------------
size_t BigInt::size() const { 
    return digits.size(); 
}

// ---------------------------------------------------------------------------------
long BigInt::sum() const
{
    long s = 0;
    for (const int& x : digits)
    {
        s += x;
    }
    return s;
}

// ---------------------------------------------------------------------------------
//carries over any dangling digits
//and removes and leading zeros
void BigInt::redistribute()
{
    for (int idx = 0; idx < digits.size(); ++idx)
    {
        int num = digits[idx];
        if (num >= 10)
        {
            if (idx == digits.size() - 1)
            {
                digits.push_back(0); //add anothr digit
            }

            int carry = num / 10;
            digits[idx] = num % 10;
            digits[idx + 1] += carry;
        } 
    }

    while (!digits.empty() && digits.back() == 0)
    {
        digits.pop_back();   
    }
}