// Bruce, Donaven        CS130 Section 27623  2025-09-15
// First Laboratory Assignment - Bitwise Operation Calculator

#include <iostream>
#include <limits>
#include <iomanip>
#include <string>
#include <cstdint>
#include <climits>

// CS130 bitwise calculator per spec

struct App {
    using UWord = std::uint64_t;   // Unsigned for binary and shifts
    using SWord = std::int64_t;    // Signed for not and negate

    static constexpr int WORD_BITS = sizeof(UWord) * CHAR_BIT; // Bit width banner and limits

    void run();

private:
    bool aborted = false;  // Stop after any input error

    void printMenu() const;     // Exact wording required
    void printHeader() const;
    bool readMenuToken(std::string& token);
    void dispatch(char choice); // Map menu choice to handler

    bool getTwoOperands(UWord& a, UWord& b); // Unsigned inputs
    bool getOneOperandSigned(SWord& x);      // Signed input

    void printResultU(UWord decVal, UWord hexVal) const; // Zero padded hex
    void printResultS(SWord decVal, UWord hexVal) const;
    void printShiftError(UWord amount) const; // Spec wording

    template<class F> void doBinary(F op);      // Generic binary op
    template<class F> void doUnarySigned(F op); // Generic unary op
    template<class F> void doShift(F shifter);  // Generic shift op

    void doAND();
    void doOR();
    void doXOR();
    void doNOT();
    void doNEGATE();
    void doLSHIFT();
    void doRSHIFT();
};

using std::cin;
using std::cout;
using std::string;

static constexpr int HEX_WIDTH = sizeof(App::UWord) * 2; // Two hex chars per byte

void App::printMenu() const {
    cout << "Enter one of the following choices:\n";
    cout << "& - AND\n";
    cout << "| - OR\n";
    cout << "^ - XOR\n";
    cout << "~ - NOT\n";
    cout << "- - Negate\n";
    cout << "< - Left shift (logical shift only)\n";
    cout << "> - Right shift (logical shift only)\n";
    cout << "# - stop execution\n";
}

void App::printHeader() const {
    cout << "Bitwise Operation Calculator - CS130 - " << WORD_BITS
         << " bits - Donaven Bruce\n\n";
}

bool App::readMenuToken(string& token) {
    if (!(cin >> token)) {
        cout << "\nInput error, enter choice and values again.\n";
        aborted = true;
        return false;
    }
    return true;
}

void App::dispatch(char choice) {
    switch (choice) {
        case '&': doAND();    break;
        case '|': doOR();     break;
        case '^': doXOR();    break;
        case '~': doNOT();    break;
        case '-': doNEGATE(); break;
        case '<': doLSHIFT(); break;
        case '>': doRSHIFT(); break;
        default:
            cout << "\nInvalid choice " << string(1, choice) << ", please try again.\n";
            break;
    }
}

void App::run() {
    printHeader();

    for (;;) {
        if (aborted) break;
        printMenu();

        string token;
        if (!readMenuToken(token)) break;

        if (token == "#") {
            cout << "\nCalculation ended.\n";
            break;
        }

        if (token.size() == 1) {
            dispatch(token[0]);
        } else {
            cout << "\nInvalid choice " << token << ", please try again.\n";
        }
    }
}

bool App::getTwoOperands(UWord& a, UWord& b) {
    cout << "\nEnter values for first and second operand:\n";
    if (!(cin >> a >> b)) {
        cout << "\nInput error, enter choice and values again.\n";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        aborted = true;
        return false;
    }
    cout << "\n";
    return true;
}

bool App::getOneOperandSigned(SWord& x) {
    cout << "\nEnter value for operand:\n";
    if (!(cin >> x)) {
        cout << "\nInput error, enter choice and values again.\n";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        aborted = true;
        return false;
    }
    cout << "\n";
    return true;
}

void App::printResultU(UWord decVal, UWord hexVal) const {
    cout << "Result: " << std::dec << decVal
         << " [Hexadecimal:" << std::hex
         << std::setw(HEX_WIDTH) << std::setfill('0') << hexVal
         << std::dec << std::setfill(' ') << "] " << '\n';
}

void App::printResultS(SWord decVal, UWord hexVal) const {
    cout << "Result: " << std::dec << decVal
         << " [Hexadecimal:" << std::hex
         << std::setw(HEX_WIDTH) << std::setfill('0') << hexVal
         << std::dec << std::setfill(' ') << "] " << '\n';
}

void App::printShiftError(UWord amount) const {
    cout << "Shift value " << amount << " exceeds bit size " << WORD_BITS << "\n";
}

template<class F>
void App::doBinary(F op) {
    UWord a, b;
    if (!getTwoOperands(a, b) || aborted) return;
    UWord r = op(a, b);
    printResultU(r, r);
}

template<class F>
void App::doUnarySigned(F op) {
    SWord x;
    if (!getOneOperandSigned(x) || aborted) return;
    SWord sres = op(x);
    printResultS(sres, static_cast<UWord>(sres));
}

template<class F>
void App::doShift(F shifter) {
    UWord v, s;
    if (!getTwoOperands(v, s) || aborted) return;
    UWord r = 0;
    if (s >= WORD_BITS) {           // Zero result on excessive shift
        printShiftError(s);
    } else {
        r = shifter(v, s);          // Logical shifts due to unsigned value
    }
    printResultU(r, r);
}

void App::doAND()    { doBinary([](UWord a, UWord b){ return a & b; }); }
void App::doOR()     { doBinary([](UWord a, UWord b){ return a | b; }); }
void App::doXOR()    { doBinary([](UWord a, UWord b){ return a ^ b; }); }

void App::doNOT()    { doUnarySigned([](SWord x){ return static_cast<SWord>(~x); }); }
void App::doNEGATE() { doUnarySigned([](SWord x){ return static_cast<SWord>(-x); }); }

void App::doLSHIFT() { doShift([](UWord v, UWord s){ return v << s; }); }
void App::doRSHIFT() { doShift([](UWord v, UWord s){ return v >> s; }); }

int main() {
    App{}.run();
    return 0;
}
