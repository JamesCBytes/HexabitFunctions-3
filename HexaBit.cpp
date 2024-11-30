#include "HexaBit.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

HexaBit::HexaBit() {
    for (int i = 0; i < 8; ++i) registers[i] = 0;
    for (int i = 0; i < 4; ++i) flags[i] = false;
}

int HexaBit::getOperand(string operand) {
    if (operand == "R0") return 0;
    if (operand == "R1") return 1;
    if (operand == "R2") return 2;
    if (operand == "R3") return 3;
    if (operand == "R4") return 4;
    if (operand == "R5") return 5;
    if (operand == "R6") return 6;
    if (operand == "R7") return 7;
    cerr << "Error: Invalid operand '" << operand << "'." << endl;
    return -1;
}

int HexaBit::operatorType(string operation) {
    if (operation == "ADD" || operation == "ADDS") return 1;
    if (operation == "AND" || operation == "ANDS") return 2;
    if (operation == "LSR" || operation == "LSRS") return 3;
    if (operation == "LSL" || operation == "LSLS") return 4;
    if (operation == "ORR" || operation == "ORRS") return 5;
    if (operation == "SUB" || operation == "SUBS") return 6;
    if (operation == "XOR" || operation == "XORS") return 7;
    if (operation == "MOV" || operation == "MOVS") return 8;
    return 0;
}

void HexaBit::movHex(int a, int b) {
    registers[a] = b;
}

void HexaBit::addHex(int a, int b, int c, string flagSet) {
    uint32_t sum = registers[b] + registers[c];
    registers[a] = sum;
    if (flagSet == "ADDS") {
        int32_t signedSum = static_cast<int32_t>(sum);
        flags[0] = (signedSum < 0); 
        flags[1] = (sum == 0);    
        flags[2] = (sum < registers[b]);
        flags[3] = ((registers[b] > 0 && registers[c] > 0 && signedSum < 0) || (registers[b] < 0 && registers[c] < 0 && signedSum > 0));
                    
    }
}

void HexaBit::andHex(int a, int b, int c, string flagSet) {
    uint32_t result = registers[b] & registers[c];
    registers[a] = result;
    if (flagSet == "ANDS") {
        flags[0] = false;          
        flags[1] = (result == 0);
    }
}


void HexaBit::subHex(int a, int b, int c, string flagSet) {
    int32_t signedResult = static_cast<int32_t>(registers[b]) - static_cast<int32_t>(registers[c]);
    registers[a] = static_cast<uint32_t>(signedResult);
    if (flagSet == "SUBS") {
        flags[0] = (signedResult < 0); 
        flags[1] = (signedResult == 0);
        flags[2] = (registers[b] < registers[c]); 
    }
}

void HexaBit::lsrHex(int a, int b, int c, string flagSet) {
    uint32_t result = registers[b] >> c;
    registers[a] = result;
    if (flagSet == "LSRS") {
        flags[0] = false;         
        flags[1] = (result == 0);
    }
}

void HexaBit::lslHex(int a, int b, int c, string flagSet) {
    uint32_t result = registers[b] << c;
    registers[a] = result;
    if (flagSet == "LSLS") {
        flags[0] = false;          
        flags[1] = (result == 0);
    }
}

void HexaBit::xorHex(int a, int b, int c, string flagSet) {
    uint32_t result = registers[b] ^ registers[c];
    registers[a] = result;
    if (flagSet == "XORS") {
        flags[0] = false;         
        flags[1] = (result == 0);
    }
}

void HexaBit::orrHex(int a, int b, int c, string flagSet) {
    uint32_t result = registers[b] | registers[c];
    registers[a] = result;
    if (flagSet == "ORRS") {
        flags[0] = false;        
        flags[1] = (result == 0);
    }
}



void HexaBit::printRegisters(const string& operation) const {
    cout << operation << endl;
    cout << "R0:0x" << hex << uppercase << registers[0] << "  "
         << "R1:0x" << registers[1] << "  "
         << "R2:0x" << registers[2] << "  "
         << "R3:0x" << registers[3] << endl;
    cout << "R4:0x" << registers[4] << "  "
         << "R5:0x" << registers[5] << "  "
         << "R6:0x" << registers[6] << "  "
         << "R7:0x" << registers[7] << endl;
    cout << "N = " << flags[0] << " "
         << "Z = " << flags[1] << " "
         << "C = " << flags[2] << " "
         << "V = " << flags[3] << endl;
}

void HexaBit::processFile(const string& fileName) {
    ifstream hexNumberStream(fileName);
    if (!hexNumberStream.is_open()) {
        cerr << "Error: Unable to open file " << fileName << endl;
        return;
    }

    string hexOperation, operandName, operandName2, operandName3;
    uint32_t operandOne = 0, operandTwo = 0, operandThree = 0;

    while (hexNumberStream >> hexOperation) {

        transform(hexOperation.begin(), hexOperation.end(), hexOperation.begin(), ::toupper);

        string operationDetails;
        stringstream ss;

        if (hexOperation == "MOV") {
            hexNumberStream >> operandName >> hex >> operandTwo;

            transform(operandName.begin(), operandName.end(), operandName.begin(), ::toupper);
            if (!operandName.empty() && operandName.back() == ',') {
                operandName.pop_back();
            }

            operandOne = getOperand(operandName);
            if (operandOne < 0) {
                cerr << "Error: Invalid operand '" << operandName << "'." << endl;
                continue;
            }

            ss << hexOperation << " " << operandName << ", #0x" << hex << uppercase << operandTwo;
            operationDetails = ss.str();
            movHex(operandOne, operandTwo);

        } else if (hexOperation == "ADDS" || hexOperation == "SUBS" || 
                   hexOperation == "ANDS" || hexOperation == "ORR" || 
                   hexOperation == "XOR") {
            hexNumberStream >> operandName >> operandName2 >> operandName3;


            transform(operandName.begin(), operandName.end(), operandName.begin(), ::toupper);
            transform(operandName2.begin(), operandName2.end(), operandName2.begin(), ::toupper);
            transform(operandName3.begin(), operandName3.end(), operandName3.begin(), ::toupper);
            if (!operandName.empty() && operandName.back() == ',') operandName.pop_back();
            if (!operandName2.empty() && operandName2.back() == ',') operandName2.pop_back();
            if (!operandName3.empty() && operandName3.back() == ',') operandName3.pop_back();

            operandOne = getOperand(operandName);
            operandTwo = getOperand(operandName2);
            operandThree = getOperand(operandName3);

            ss << hexOperation << " " << operandName << ", " << operandName2 << ", " << operandName3;
            operationDetails = ss.str();

            switch (operatorType(hexOperation)) {
                case 1: addHex(operandOne, operandTwo, operandThree, hexOperation); break;
                case 2: andHex(operandOne, operandTwo, operandThree, hexOperation); break;
                case 5: orrHex(operandOne, operandTwo, operandThree, hexOperation); break;
                case 6: subHex(operandOne, operandTwo, operandThree, hexOperation); break;
                case 7: xorHex(operandOne, operandTwo, operandThree, hexOperation); break;
                default: cerr << "Error: Unknown operation type '" << hexOperation << "'." << endl; break;
            }

        } else if (hexOperation == "LSRS" || hexOperation == "LSLS") {
            hexNumberStream >> operandName >> operandName2 >> hex >> operandThree;

            transform(operandName.begin(), operandName.end(), operandName.begin(), ::toupper);
            transform(operandName2.begin(), operandName2.end(), operandName2.begin(), ::toupper);
            if (!operandName.empty() && operandName.back() == ',') operandName.pop_back();
            if (!operandName2.empty() && operandName2.back() == ',') operandName2.pop_back();

            operandOne = getOperand(operandName);
            operandTwo = getOperand(operandName2);

            ss << hexOperation << " " << operandName << ", " << operandName2 << ", #" << hex << uppercase << operandThree;
            operationDetails = ss.str();

            if (hexOperation == "LSRS") lsrHex(operandOne, operandTwo, operandThree, hexOperation);
            if (hexOperation == "LSLS") lslHex(operandOne, operandTwo, operandThree, hexOperation);

        } else if (hexOperation == "CMP" || hexOperation == "TST") {
            hexNumberStream >> operandName >> operandName2;

            transform(operandName.begin(), operandName.end(), operandName.begin(), ::toupper);
            transform(operandName2.begin(), operandName2.end(), operandName2.begin(), ::toupper);
            if (!operandName.empty() && operandName.back() == ',') operandName.pop_back();

            operandOne = getOperand(operandName);
            operandTwo = getOperand(operandName2);

            ss << hexOperation << " " << operandName << ", " << operandName2;
            operationDetails = ss.str();

            if (hexOperation == "CMP") {
                uint32_t result = registers[operandOne] - registers[operandTwo];
                flags[0] = (static_cast<int32_t>(result) < 0); 
                flags[1] = (result == 0);
                flags[2] = (registers[operandOne] < registers[operandTwo]); 
            } else if (hexOperation == "TST") {
                uint32_t result = registers[operandOne] & registers[operandTwo];
                flags[0] = false; 
                flags[1] = (result == 0);
            }

        }

        printRegisters(operationDetails);
    }

    hexNumberStream.close();
}

