#include <cstdlib>
#include <cxxabi.h>
#include <execinfo.h>
#include <iostream>
#include <limits>

int input() {
    int value;
    do {
        std::cout << "Input the size of backtraced buffer: ";
        std::string inputStr;
        std::cin >> inputStr;

        bool isValid = true;
        for (char c : inputStr) {
            if (!isdigit(c)) { 
                isValid = false;
                break;
            }
        }

        if (!isValid) {
            std::cerr << "Invalid input. Please enter a positive integer" << std::endl;
            value = -1;
        } else {
            value = std::stoi(inputStr);
            if (value <= 0) {
                std::cerr << "Error: The size must be greater than 0" << std::endl;
            }
        }
    } while (value <= 0);

    return value;
}

void stack_dump(int depth, std::ostream& os) {
    if (depth <= 0) depth = 10;

    void* buffer[depth + 1];
    int nptrs = backtrace(buffer, depth + 1);

    if (nptrs <= 2) {
        os << "No useful stack frames to print\n";
        return;
    }

    char** strings = backtrace_symbols(buffer, nptrs);
    if (!strings) {
        os << "Error: backtrace_symbols failed\n";
        return;
    }

    for (int i = 2; i < nptrs; ++i) {
        std::string symbol(strings[i]);
        size_t begin = symbol.find('(');
        size_t end = symbol.find('+');
        size_t offset_end = symbol.find(')');

        if (begin != std::string::npos && end != std::string::npos && offset_end != std::string::npos) {
            std::string mangledName = symbol.substr(begin + 1, end - begin - 1);
            std::string offset = symbol.substr(end + 1, offset_end - end - 1);

            int status;
            char* demangledName = abi::__cxa_demangle(mangledName.c_str(), nullptr, nullptr, &status);

            if (status == 0 && demangledName) {
                os << symbol.substr(0, begin) << " :  " << demangledName << "+" << offset << std::endl;
                free(demangledName);
            } else {
                os << "Demangling failed for: " << mangledName << " with status: " << status << std::endl;
            }
        } else {
            os << symbol << std::endl;
        }
    }

    free(strings);
}

double sum(double a, double b) {
    stack_dump(input(), std::cout);
    return a + b;
}

void stringFoo(const std::string& str) {
    sum(1.5, 4.1);
}

void start() {
    stringFoo("testing");
}

int main() {
    start();
    return 0;
}
