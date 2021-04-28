#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <chrono>

/***************************************************************************
 * This is a class that represents a positive integer as a vector of ints
 * to calculate values beyond the capabilities of primitive types
 ***************************************************************************/
class Number
{
    public:
        Number();
        // ---------------------------------------------------------------------------------
        Number(const Number& other)
        {
            digits.resize(other.size());
            for (int i = 0; i < other.size(); ++i)
            {
                digits[i] = other.digits[i];
            }
        }


        // ---------------------------------------------------------------------------------
        Number (const std::string &str)
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
        Number(long n)
        {
            const long N = n;
            int mod = 10;
            while (n >= 1)
            {   
                int thisDigit = n % 10;
                digits.push_back(thisDigit);
                n /= 10;
                //std::cout << "thisDigit: " << thisDigit << ", n: " << n << std::endl;
            }
        };

        // ---------------------------------------------------------------------------------
        static std::vector<long> primeFactors(long n)
        {
            std::vector<long> factorVec;
            
            long z = 2;
            while (z * z <= n)
            {
                printf("seeing if %ld divides %ld\n", z, n);
                if (n % z == 0)
                {
                    factorVec.push_back(z);
                    printf("%ld divides %ld\n", z, n);
                    n /= z;
                }
                else
                {
                   ++z;
                } 
            }

            if (n > 1)
            {
                printf("finally %ld\n", n);
                factorVec.push_back(n);
            }

            for (auto k : factorVec)
            {
                std::cout << k << ", ";
            }

            std::cout << std::endl;

            return factorVec;

        }

        // ---------------------------------------------------------------------------------
        void print()
        {
            for (int k = 0; k < digits.size(); ++k)
            {
                printf("digits[%d]: %d\n", k, digits[k]);
            }
        }

        // ---------------------------------------------------------------------------------
        void printStr()
        {
            std::cout << toString() << std::endl;
        }

        // ---------------------------------------------------------------------------------
        bool operator==(Number& other) 
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
        Number& operator=(const Number& other)
        {
            digits.resize(other.digits.size());
            for (int i = 0; i < digits.size(); ++i)
            {
                digits[i] = other.digits[i];
            }
            return *this;
        }

        // ---------------------------------------------------------------------------------
        long toLong() 
        {
            int ret = 0;
            for (int p = 0; p < digits.size(); ++p)
            {
                ret += digits[p] * pow(10, p);
            }
            return ret;
        }

        // ---------------------------------------------------------------------------------
        Number& fromLong(long n)
        {
            *this = Number(n);
            return *this;
        }
     
        // SLOW: 
        // ---------------------------------------------------------------------------------
        Number& exponential(long p, bool print = false)
        {
            long tot = p;
            float pct = 0;
            long cnt = 0;
            Number m(*this);
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
        Number& exponentialInt(long p, bool print = false)
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
        Number& exponentialIntFast(long p, bool print = false)
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
        std::string toString()
        {
            std::string s(size(), 'x');
            for (int k = 0; k < size(); ++k)
            {
                s[size() - 1 - k] = char(digits[k] + '0');
            }

            return s;
        } 
         
        // ---------------------------------------------------------------------------------
        bool operator<(const Number& other);

        // ---------------------------------------------------------------------------------
        Number& operator+=(const Number &other)
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
        Number operator+(const Number& n)
        {
            Number ret = *this;
            ret += n;
            return ret;
        }

        // ---------------------------------------------------------------------------------
        Number& operator*=(const Number &other)
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
        template <typename T>
        Number& operator*=(T other)
        {
            for (int k = 0; k < size(); ++k)
            {
                digits[k] *= other;
            }
            redistribute();
            return *this;
        }

        // ---------------------------------------------------------------------------------
        size_t size() const { return digits.size(); }

        // ---------------------------------------------------------------------------------
        long sum() const
        {
            long s = 0;
            for (const int& x : digits)
            {
                s += x;
            }
            return s;
        }

    private:
        //carries over any dangling digits
        //and removes and leading zeros
        void redistribute()
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

    private:
        std::vector<int> digits;
};


// ---------------------------------------------------------------------------------
int main()
{

    std::cout << "welcome to big number generator\n";
    std::cout << "here, LONG_MAX is: " << LONG_MAX << std::endl;
    char buf[128];
    sprintf(buf, "%ld", long(sqrt(LONG_MAX)));
    std::cout << "-> sqrt(LONG_MAX): " << buf << std::endl;
    std::cout << "enter base smaller than that:";
    long base = 2;
    std::cin >> base;
    std::cout << "enter exponent K to calculate " << base << " to the power of K: ";
    long power = 0;
    std::cin >> power;
    
    Number l(base);
    auto t1 = std::chrono::high_resolution_clock::now();
    l.exponentialIntFast(power, true);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    long sum = l.sum();
    std::cout << "len: " << l.size() << "; sum: " << sum  << std::endl;
    std::cout << "time : " << duration/1000.0 << "ms = " << (duration/1000000.0)/60 << " minutes" << std::endl; 
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    
     
    std::ofstream file;
    char filename[512];
    sprintf(filename, "pow_%ld_to_the_%ld.txt", base, power);
    file.open(filename);

    std::cout << "writing to " << filename << "...";
    file << "len: " << l.size() << "; sum: " << sum  << "\n";
    file << "time : " << duration/1000.0 << "ms = " << (duration/1000000.0)/60 << " minutes" << "\n";
    file << "~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n";
    file << l.toString() << "\n";
    std::cout << "done" << std::endl;

    return 0;
}
