#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <stdlib.h>

/***************************************************************************
 * This is a class that represents a positive integer as a vector of ints
 * to calculate values beyond the capabilities of primitive types
 ***************************************************************************/
class BigInt
{
    public:
        BigInt(){};
        BigInt(const BigInt& other);
        BigInt (const std::string &str);
        BigInt(long n);

        
        void print();
        void printStr();
        bool operator==(BigInt& other);
        BigInt& operator=(const BigInt& other);
        long toLong();
        BigInt& fromLong(long n);
        // SLOW: 
        BigInt& exponential(long p, bool print = false);
        // does same as above, but converts to int first for speed
        BigInt& exponentialInt(long p, bool print = false);
        BigInt& exponentialIntFast(long p, bool print = false);
        std::string toString();
        bool operator<(const BigInt& other);
        BigInt& operator+=(const BigInt &other);
        BigInt operator+(const BigInt& n);
        BigInt& operator*=(const BigInt &other);
        size_t size() const;
        long sum() const;

        // ---------------------------------------------------------------------------------
        template <typename T>
        BigInt& operator*=(T other)
        {
            for (int k = 0; k < size(); ++k)
            {
                digits[k] *= other;
            }
            redistribute();
            return *this;
        }

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
    private:
        //carries over any dangling digits
        //and removes and leading zeros
        void redistribute();

        std::vector<int> digits;
};