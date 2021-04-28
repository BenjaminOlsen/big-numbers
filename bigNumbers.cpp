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
        Number& operator*=(int other)
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
    l.exponentialInt(power, true);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    l.printStr();

    long sum = l.sum();
    std::cout << "len: " << l.size() << "; sum: " << sum  << std::endl;
    std::cout << "time : " << duration/1000.0 << "ms = " << (duration/1000000.0)/60 << " minutes" << std::endl; 
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    
    
    /*
    std::ofstream file;
    file.open("powersOfTwo.txt");

    for (int p = 1; p < 30000; ++p)
    {
        std::cout << "calculating 2^" << p << std::endl;
        Number n(2);
        n.exponentialInt(p);
        file << "~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        file << "2^" << p << ":\n";
        file << n.toString() << "\n";
    }
    */

    return 0;
}
