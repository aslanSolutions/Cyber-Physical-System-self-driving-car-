// Comment for assignment 6 by Ali
#include "PrimeChecker.hpp"

bool PrimeChecker::isPrime(uint16_t n) {
    if (n < 2) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    if (n % 2 == 0) {
        return false;
    }
    
    for (uint16_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    
    return true;
}
