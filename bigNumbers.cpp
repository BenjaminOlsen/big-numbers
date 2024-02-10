#include <iostream>
#include <fstream>
#include <chrono>

#include "bigInt.hpp"


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
    
    BigInt l(base);
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
