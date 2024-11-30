#ifndef HEXABIT_H
#define HEXABIT_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

class HexaBit {
private:
    uint32_t registers[8];
    bool flags[4]; // N, Z, C, V 

    int getOperand(string operand);
    int operatorType(string operation);

    void movHex(int a, int b);
    void addHex(int a, int b, int c, string flagSet);
    void andHex(int a, int b, int c, string flagSet);
    void subHex(int a, int b, int c, string flagSet);
    void lsrHex(int a, int b, int c, string flagSet);
    void lslHex(int a, int b, int c, string flagSet);
    void xorHex(int a, int b, int c, string flagSet);
    void orrHex(int a, int b, int c, string flagSet);

public:
    HexaBit();
    void processFile(const string& fileName);
    void printRegisters(const string& operation) const;
};

#endif
