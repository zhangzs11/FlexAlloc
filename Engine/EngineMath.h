#pragma once
class LCG {
private:
    unsigned long long a = 1664525;
    unsigned long long c = 1013904223;
    unsigned long long m = 4294967296; // 2^32
    unsigned long long seed;
public:
    LCG(unsigned long long initialSeed) : seed(initialSeed) {}

    unsigned long long next() {
        seed = (a * seed + c) % m;
        return seed;
    }

    // return random number from 0 to max-1
    unsigned long next(unsigned long max) {
        return next() % max;
    }
};
