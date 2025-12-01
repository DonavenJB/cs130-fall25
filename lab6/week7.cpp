/*
 * Student Name: Donaven bruce
 * Student ID: 000000000000
*/
#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Program reads a money string, validates, normalizes, then masks it
// Uses string operations only, no numeric format helpers

// Function prototype using reference parameters to validate and trim input
bool trimAndValidate(const string& rawInput,
    string& trimmedOutput,
    bool& isNegative);

// Function prototype with a reference parameter to build a normalized amount
string normalizeCanonical(const string& trimmedInput,
    bool isNegative);

// Function prototype with a reference parameter to build a masked amount
string createMasked(const string& normalizedCurrency);

int main() {
    cout << "Student Name: Donaven Bruce\n";
    cout << "Student ID: 000000000000\n\n";

    string rawInput;
    // Reads one full line as the money input
    cout << "Enter a money amount: ";
    getline(cin, rawInput);

    string trimmed;
    bool isNegative = false;

    // If validation fails, an error is printed and the program exits
    if (!trimAndValidate(rawInput, trimmed, isNegative)) { // Program exits here on any validation error
        return 1;
    }

    // On success, build normalized and masked strings from trimmed input
    string normalized = normalizeCanonical(trimmed, isNegative);
    string masked = createMasked(normalized);

    // On success, print original trimmed line, normalized string, and masked string
    cout << "Original(trimmed) : " << trimmed << "\n";
    cout << "Normalized:         " << normalized << "\n";
    cout << "Masked:             " << masked << "\n";

    return 0; // Program exits after printing original/normalized/masked
}

bool trimAndValidate(const string& rawInput,
    string& trimmedOutput,
    bool& isNegative)
{
    string s = rawInput;

    // Empty line is a special case
    if (s.empty()) {
        cout << "Error: Empty input.\n";
        return false; // fail on empty
    }

    int start = 0;
    int len = static_cast<int>(s.length());

    // Leading and trailing spaces are trimmed away
    while (start < len && s[start] == ' ')
        start++;

    int end = len;
    while (end > start && s[end - 1] == ' ')
        end--;

    if (start >= end) {
        // Only spaces after trimming: at least one digit required
        cout << "Error: At least one digit required.\n";
        return false; // fail if only spaces
    }

    s = s.substr(start, end - start);
    len = static_cast<int>(s.length());

    // Original trimmed line is echoed exactly as input after trimming
    string visible = s;

    bool negative = false;
    bool usingParens = false;

    // Parentheses must wrap the entire trimmed amount and mark a negative
    if (len > 0 && s[0] == '(') {
        if (len < 2 || s[len - 1] != ')') {
            cout << "Error: Parentheses must surround the entire amount.\n";
            return false; // bad parens
        }

        usingParens = true;
        negative = true;

        string inner = s.substr(1, len - 2);
        if (inner.length() == 0) {
            // () means no digits
            cout << "Error: No digits.\n";
            return false; // empty ()
        }

        // If inner text contains a minus, cannot mix minus and parentheses
        bool hasMinus = false;
        int innerLen = static_cast<int>(inner.length());
        for (int i = 0; i < innerLen; i++) {
            char c = inner[i];
            if (c == '-') {
                hasMinus = true;
                break; // stop scan
            }
        }
        if (hasMinus) {
            cout << "Error: Cannot mix '-' and parentheses.\n";
            return false; // fail on mixed sign
        }

        // Spaces inside parentheses are ignored for numeric value
        string noSpaces;
        for (int i = 0; i < innerLen; i++) {
            char c = inner[i];
            if (c != ' ')
                noSpaces += c;
        }
        s = noSpaces;
        len = static_cast<int>(s.length());
        if (len == 0) {
            // (   ) means no digits.
            cout << "Error: No digits.\n";
            return false; // all spaces inside ()
        }
    }

    int digitCount = 0;
    int decimalCount = 0;
    int minusCount = 0;
    int dollarCount = 0;
    bool invalid = false;
    bool tooManyDigits = false;

    // After trimming, only digits, dollar, minus, comma, dot are allowed
    // At most one decimal point
    // Only digits count toward the twelve digit limit
    for (int i = 0; i < len; i++) {
        char c = s[i];

        // Spaces inside nonparen forms are treated as invalid
        if (c == ' ' || c == '(' || c == ')') {
            invalid = true;
            break; // stop on invalid space/parens
        }

        if (c == '-') {
            // Valid negatives include minus or dollar minus, not when using parentheses
            if (usingParens) {
                invalid = true;
                break; // stop on minus with parens
            }

            minusCount++;
            if (minusCount > 1) {
                invalid = true;
                break; // stop on multiple minus
            }

            // Minus may appear only first nonspace, after optional dollar sign
            if (!(i == 0 || (i == 1 && s[0] == '$'))) {
                invalid = true;
                break; // stop on bad minus position
            }

            continue; // go to next char
        }

        if (c == '$') {
            // Allows optional leading dollar sign
            dollarCount++;
            if (dollarCount > 1 || i != 0) {
                invalid = true;
                break; // stop on bad dollar
            }
            continue; // skip dollar
        }

        if (c == '.') {
            decimalCount++;
            if (decimalCount > 1) {
                // Reject inputs with more than one decimal point
                cout << "Error: More than one decimal point.\n";
                return false; // fail on extra dot
            }
            continue; // skip dot for digit counting
        }

        // Allows optional commas in the integer part
        if (c == ',')
            continue; // ignore comma

        if (isdigit(c)) {
            // Only digits count toward the twelve digit limit
            digitCount++;
            if (digitCount > 12) {
                tooManyDigits = true;
                break; // stop on too many digits
            }
            continue; // digit handled
        }

        // Any other character is invalid
        invalid = true;
        break; // stop on invalid char
    }

    if (tooManyDigits) {
        // More than twelve digits prints this specific error
        cout << "Error: Maximum allowed digits: 12.\n";
        return false; // fail on digit limit
    }

    // Special handling for no-digit cases
    if (!invalid && digitCount == 0) {
        bool onlySigns = true;
        int sLen = static_cast<int>(s.length());
        for (int i = 0; i < sLen; i++) {
            char c = s[i];
            if (c != ' ' && c != '$' && c != '(' && c != ')') {
                onlySigns = false;
                break; // found non-sign
            }
        }
        if (onlySigns) {
            // $, () and similar have no digits.
            cout << "Error: No digits.\n";
        }
        else {
            // ',', '-.' or '( , )' still require a digit.
            cout << "Error: At least one digit required.\n";
        }
        return false; // fail on no digits
    }

    // Any other invalid case prints invalid character error
    if (invalid) {
        cout << "Error: Invalid character found.\n";
        return false; // fail on invalid flag
    }

    // Negative flag is true for minus or parentheses forms
    negative = negative || (minusCount == 1);

    trimmedOutput = visible;
    isNegative = negative;

    return true; // validation ok
}

string normalizeCanonical(const string& trimmedInput,
    bool isNegative)
{
    // Internal value is sign, integer digits, and fractional digits
    string cleaned;
    bool seenDot = false;

    int len = static_cast<int>(trimmedInput.length());
    for (int i = 0; i < len; i++) {
        char c = trimmedInput[i];

        // Input commas and sign markers are ignored for numeric value
        if (c == '$' || c == ',' || c == '(' || c == ')' || c == '-' || c == ' ') {
            continue; // skip non-numeric
        }

        if (c == '.') {
            // At most one decimal point
            if (!seenDot) {
                cleaned += c;
                seenDot = true;
            }
            continue; // skip extra dots
        }

        cleaned += c;
    }

    string intPart;
    string fracRaw;

    // Split into integer part and fractional raw part
    string::size_type dotPos = cleaned.find('.');
    if (dotPos == string::npos) {
        intPart = cleaned;
        fracRaw = "";
    }
    else {
        intPart = cleaned.substr(0, dotPos);
        if (dotPos + 1 < cleaned.length()) {
            fracRaw = cleaned.substr(dotPos + 1);
        }
        else {
            fracRaw = "";
        }
    }

    // No integer digits becomes zero
    if (intPart.empty()) {
        intPart = "0";
    }

    // Integer part has no leading zeros except single zero
    int firstNonZero = 0;
    int intLen = static_cast<int>(intPart.length());
    while (firstNonZero < intLen && intPart[firstNonZero] == '0') {
        firstNonZero++;
    }
    if (firstNonZero == intLen) {
        intPart = "0";
    }
    else {
        intPart = intPart.substr(firstNonZero);
    }

    // Fractional part is always two digits
    string frac;
    int fracLen = static_cast<int>(fracRaw.length());
    if (fracLen == 0) {
        // No decimal means fractional part is 00
        frac = "00";
    }
    else if (fracLen == 1) {
        // One fractional digit is padded with one zero
        frac = fracRaw + "0";
    }
    else {
        // Extra fractional digits are truncated to two
        frac = fracRaw.substr(0, 2);
    }

    // Commas are reinserted as standard thousands separators
    string intWithCommas = intPart;
    int n = static_cast<int>(intWithCommas.length());
    int insertPos = n - 3;
    while (insertPos > 0) {
        intWithCommas.insert(static_cast<string::size_type>(insertPos), ",");
        insertPos -= 3;
    }

    // Final format is optional minus, dollar, integer with commas, dot, two digits
    string result = "$" + intWithCommas + "." + frac;
    if (isNegative) {
        result = "-" + result;
    }

    return result; // return normalized
}

// Masking hides all but the last three integer digits
string createMasked(const string& normalizedCurrency)
{
    string result = normalizedCurrency;

    // Masking runs on the normalized currency string
    string::size_type dotPos = result.find('.');
    string::size_type endInt = result.length();
    if (dotPos != string::npos) {
        endInt = dotPos;
    }

    // Only digits before the decimal point are considered for masking
    int intDigitCount = 0;
    for (string::size_type i = 0; i < endInt; i++) {
        char c = result[i];
        if (isdigit(c)) {
            intDigitCount++;
        }
    }

    // More than three integer digits mask all but the last three
    // Three or fewer integer digits are never masked
    int cutoff = intDigitCount - 3;
    if (cutoff < 0) {
        cutoff = 0;
    }

    int digitIndex = 0;
    for (string::size_type i = 0; i < endInt; i++) {
        char c = result[i];
        if (isdigit(c)) {
            digitIndex++;
            if (digitIndex <= cutoff) {
                // Dollar, minus, commas, dot, and fractional digits are never masked
                result[i] = 'X';
            }
        }
    }

    return result; // return masked
}


//Test cases 
// 1234567.898
// ($123,45.678)
// 12345678.98765
// (-1234.56)
// 0012345.678
// 123a45.67
