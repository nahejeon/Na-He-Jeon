/*
 * File: strlib.cpp
 * ----------------
 * This file implements the strlib.h interface.
 * 
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#include "strlib.h"
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "error.h"

/* Function prototypes */

/*
 * Implementation notes: numeric conversion
 * ----------------------------------------
 * These functions use the <sstream> library to perform the conversion.
 */

std::string integerToString(int n) {
    std::ostringstream stream;
    stream << n;
    return stream.str();
}

int stringToInteger(const std::string& str) {
    std::istringstream stream(str);
    int value;
    stream >> value >> std::ws;
    if (stream.fail() || !stream.eof()) {
        error("stringToInteger: Illegal integer format (" + str + ")");
    }
    return value;
}

std::string realToString(double d) {
    std::ostringstream stream;
    stream << std::uppercase << d;
    return stream.str();
}

double stringToReal(const std::string& str) {
    std::istringstream stream(str);
    double value;
    stream >> value >> std::ws;
    if (stream.fail() || !stream.eof()) {
        error("stringToReal: Illegal floating-point format (" + str + ")");
    }
    return value;
}

bool stringIsInteger(const std::string& str) {
    std::istringstream stream(str);
    int value;
    stream >> value >> std::ws;
    return !(stream.fail() || !stream.eof());
}

bool stringIsReal(const std::string& str) {
    std::istringstream stream(str);
    double value;
    stream >> value >> std::ws;
    return !(stream.fail() || !stream.eof());
}

bool stringToBool(const std::string& str) {
    std::istringstream stream(str);
    bool value;
    stream >> std::boolalpha >> value >> std::ws;
    if (stream.fail() || !stream.eof()) {
        error("stringToBool: Illegal bool format (" + str + ")");
    }
    return value;
}

std::string boolToString(bool b) {
    return (b ? "true" : "false");
}

std::string boolToString(int b) {
    return (b ? "true" : "false");
}

char stringToChar(const std::string& str) {
    std::string str2 = trim(str);
    if ((int) str2.length() != 1) {
        error("stringToChar: string must contain exactly 1 non-whitespace character");
    }
    return str2[0];
}

std::string charToString(char c) {
    std::string s;
    s += c;
    return s;
}

/*
 * Implementation notes: case conversion
 * -------------------------------------
 * The functions toUpperCase and toLowerCase return a new string whose
 * characters appear in the desired case. These implementations rely on
 * the fact that the characters in the string are copied when the
 * argument is passed to the function, which makes it possible to change
 * the case of the copy without affecting the original.
 */

std::string toUpperCase(const std::string& str) {
    std::string str2 = str;
    int nChars = str2.length();
    for (int i = 0; i < nChars; i++) {
        str2[i] = toupper(str2[i]);
    }
    return str2;
}

std::string toLowerCase(const std::string& str) {
    std::string str2 = str;
    int nChars = str2.length();
    for (int i = 0; i < nChars; i++) {
        str2[i] = tolower(str2[i]);
    }
    return str2;
}

/*
 * Implementation notes: equalsIgnoreCase
 * --------------------------------------
 * This implementation uses a for loop to cycle through the characters in
 * each string.  Converting each string to uppercase and then comparing
 * the results makes for a shorter but less efficient implementation.
 */

bool equalsIgnoreCase(const std::string& s1, const std::string& s2) {
    if (s1.length() != s2.length()) return false;
    int nChars = s1.length();
    for (int i = 0; i < nChars; i++) {
        if (tolower(s1[i]) != tolower(s2[i])) return false;
    }
    return true;
}

/*
 * Implementation notes: startsWith, endsWith
 * ------------------------------------------
 * These implementations are overloaded to allow the second argument to
 * be either a string or a character.
 */

bool startsWith(const std::string& str, const std::string& prefix) {
    if (str.length() < prefix.length()) return false;
    int nChars = prefix.length();
    for (int i = 0; i < nChars; i++) {
        if (str[i] != prefix[i]) return false;
    }
    return true;
}

bool startsWith(const std::string& str, char prefix) {
    return str.length() > 0 && str[0] == prefix;
}

bool endsWith(const std::string& str, const std::string& suffix) {
    int nChars = suffix.length();
    int start = str.length() - nChars;
    if (start < 0) return false;
    for (int i = 0; i < nChars; i++) {
        if (str[start + i] != suffix[i]) return false;
    }
    return true;
}

bool endsWith(const std::string& str, char suffix) {
    return str.length() > 0 && str[str.length() - 1] == suffix;
}

std::string trim(const std::string& str) {
    int finish = (int) str.length() - 1;
    while (finish >= 0 && isspace(str[finish])) {
        finish--;
    }
    int start = 0;
    while (start <= finish && isspace(str[start])) {
        start++;
    }
    return str.substr(start, finish - start + 1);
}

std::string trimEnd(const std::string& str) {
    int finish = (int) str.length() - 1;
    while (finish >= 0 && isspace(str[finish])) {
        finish--;
    }
    return str.substr(0, finish + 1);
}

std::string trimStart(const std::string& str) {
    int start = 0;
    int finish = (int) str.length() - 1;
    while (start <= finish && isspace(str[start])) {
        start++;
    }
    return str.substr(start, finish - start + 1);
}

bool stringContains(const std::string& s, const std::string& substring) {
    return s.find(substring) != std::string::npos;
}

int stringIndexOf(const std::string& s, const std::string& substring) {
    size_t index = s.find(substring);
    if (index == std::string::npos) {
        return -1;
    } else {
        return index;
    }
}

int stringLastIndexOf(const std::string& s, const std::string& substring) {
    size_t index = s.rfind(substring);
    if (index == std::string::npos) {
        return -1;
    } else {
        return index;
    }
}

std::string stringReplace(const std::string& str, const std::string& old, const std::string& replacement, int limit) {
    std::string str2 = str;
    int count = 0;
    while (limit < 0 || count < limit) {
        size_t index = str2.find(old);
        if (index == std::string::npos) {
            break;
        }
        str2.replace(index, old.length(), replacement);
        count++;
    }
    return str2;
}

std::vector<std::string> stringSplit(const std::string& str, const std::string& delimiter, int limit) {
    std::string str2 = str;
    std::vector<std::string> result;
    int count = 0;
    size_t index = 0;
    while (limit < 0 || count < limit) {
        index = str2.find(delimiter);
        if (index == std::string::npos) {
            break;
        }
        result.push_back(str2.substr(0, index));
        str2.erase(str2.begin(), str2.begin() + index + delimiter.length());
        count++;
    }
    if ((int) str2.length() > 0) {
        result.push_back(str2);
    }

    return result;
}

std::string stringJoin(const std::vector<std::string>& v, const std::string& delimiter) {
    if (v.empty()) {
        return "";
    } else {
        std::ostringstream out;
        out << v[0];
        for (int i = 1; i < (int) v.size(); i++) {
            out << delimiter;
            out << v[i];
        }
        return out.str();
    }
}

std::string urlEncode(const std::string& value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex << std::uppercase;

    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        std::string::value_type c = (*i);
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '*') {
            escaped << c;
        } else if (c == ' ')  {
            escaped << '+';
        } else {
            escaped << '%' << std::setw(2) << ((int) c) << std::setw(0);
        }
    }

    return escaped.str();
}

std::string urlDecode(const std::string& value) {
    std::ostringstream unescaped;
    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        std::string::value_type c = (*i);
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '*') {
            unescaped << c;
        } else if (c == '+')  {
            unescaped << ' ';
        } else if (c == '%') {
            int decodedChar = 0;
            ++i;
            decodedChar += (*i) * 16;
            ++i;
            decodedChar += (*i);
            unescaped << (char) decodedChar;
        } else {
            std::ostringstream msg;
            msg << "Unexpected character in urlDecode string: "
                << (int) c << " (" << (char) c << ")";
            error(msg.str());
        }
    }

    return unescaped.str();
}


/*
 * Implementation notes: readQuotedString and writeQuotedString
 * ------------------------------------------------------------
 * Most of the work in these functions has to do with escape sequences.
 */

static const std::string STRING_DELIMITERS = ",:)}]\n";

bool stringNeedsQuoting(const std::string & str) {
    int n = str.length();
    for (int i = 0; i < n; i++) {
        char ch = str[i];
        if (isspace(ch)) return false;
        if (STRING_DELIMITERS.find(ch) != std::string::npos) return true;
    }
    return false;
}

void readQuotedString(std::istream & is, std::string & str) {
    str = "";
    char ch;
    while (is.get(ch) && isspace(ch)) {
        /* Empty */
    }
    if (is.fail()) return;
    if (ch == '\'' || ch == '"') {
        char delim = ch;
        while (is.get(ch) && ch != delim) {
            if (is.fail()) error("Unterminated string");
            if (ch == '\\') {
                if (!is.get(ch)) error("Unterminated string");
                if (isdigit(ch) || ch == 'x') {
                    int maxDigits = 3;
                    int base = 8;
                    if (ch == 'x') {
                        base = 16;
                        maxDigits = 2;
                    }
                    int result = 0;
                    int digit = 0;
                    for (int i = 0; i < maxDigits && ch != delim; i++) {
                        if (isdigit(ch)) {
                            digit = ch - '0';
                        } else if (base == 16 && isxdigit(ch)) {
                            digit = toupper(ch) - 'A' + 10;
                        } else {
                            break;
                        }
                        result = base * result + digit;
                        if (!is.get(ch)) error("Unterminated string");
                    }
                    ch = char(result);
                    is.unget();
                } else {
                    switch (ch) {
                    case 'a': ch = '\a'; break;
                    case 'b': ch = '\b'; break;
                    case 'f': ch = '\f'; break;
                    case 'n': ch = '\n'; break;
                    case 'r': ch = '\r'; break;
                    case 't': ch = '\t'; break;
                    case 'v': ch = '\v'; break;
                    case '"': ch = '"'; break;
                    case '\'': ch = '\''; break;
                    case '\\': ch = '\\'; break;
                    }
                }
            }
            str += ch;
        }
    } else {
        str += ch;
        int endTrim = 0;
        while (is.get(ch) && STRING_DELIMITERS.find(ch) == std::string::npos) {
            str += ch;
            if (!isspace(ch)) endTrim = str.length();
        }
        if (is) is.unget();
        str = str.substr(0, endTrim);
    }
}

void writeQuotedString(std::ostream & os, const std::string & str, bool forceQuotes) {
    if (!forceQuotes && stringNeedsQuoting(str)) forceQuotes = true;
    if (forceQuotes) os << '"';
    int len = str.length();
    for (int i = 0; i < len; i++) {
        char ch = str.at(i);
        switch (ch) {
        case '\a': os << "\\a"; break;
        case '\b': os << "\\b"; break;
        case '\f': os << "\\f"; break;
        case '\n': os << "\\n"; break;
        case '\r': os << "\\r"; break;
        case '\t': os << "\\t"; break;
        case '\v': os << "\\v"; break;
        case '\\': os << "\\\\"; break;
        default:
            if (isprint(ch) && ch != '"') {
                os << ch;
            } else {
                std::ostringstream oss;
                oss << std::oct << std::setw(3) << std::setfill('0') << (int(ch) & 0xFF);
                os << "\\" << oss.str();
            }
        }
    }
    if (forceQuotes) os << '"';
}
