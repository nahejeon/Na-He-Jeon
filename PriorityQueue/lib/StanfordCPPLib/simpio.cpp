/*
 * File: simpio.cpp
 * ----------------
 * This file implements the simpio.h interface.
 * 
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#include "simpio.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace autograder {
extern bool getConsoleEchoUserInput();
}

static void appendSpace(std::string& prompt);

/*
 * Implementation notes: getInteger, getReal
 * -----------------------------------------
 * Each of these functions reads a complete input line and then uses the
 * <sstream> library to parse that line into a value of the desired type.
 * If that fails, the implementation asks the user for a new value.
 */

int getInteger(std::string prompt, std::string reprompt) {
    if (reprompt == "") {
        reprompt = "Illegal integer format. Try again.";
    }
    appendSpace(prompt);
    int value;
    std::string line;
    while (true) {
        std::cout << prompt;
        getline(std::cin, line);
        if (autograder::getConsoleEchoUserInput()) {
            std::cout << line << std::endl;
        }
        std::istringstream stream(line);
        stream >> value >> std::ws;
        if (!stream.fail() && stream.eof()) break;
        std::cout << reprompt << std::endl;
        if (prompt == "") prompt = "Enter an integer: ";
    }
    return value;
}

double getReal(std::string prompt, std::string reprompt) {
    if (reprompt == "") {
        reprompt = "Illegal numeric format. Try again.";
    }
    appendSpace(prompt);
    double value;
    std::string line;
    while (true) {
        std::cout << prompt;
        getline(std::cin, line);
        if (autograder::getConsoleEchoUserInput()) {
            std::cout << line << std::endl;
        }
        std::istringstream stream(line);
        stream >> value >> std::ws;
        if (!stream.fail() && stream.eof()) break;
        std::cout << reprompt << std::endl;
        if (prompt == "") prompt = "Enter a number: ";
    }
    return value;
}

bool getYesOrNo(std::string prompt, std::string reprompt) {
    if (reprompt == "") {
        reprompt = "Please type a word that starts with 'Y' or 'N'.";
    }
    appendSpace(prompt);
    bool value;
    std::string line;
    while (true) {
        std::cout << prompt;
        getline(std::cin, line);
        if (autograder::getConsoleEchoUserInput()) {
            std::cout << line << std::endl;
        }
        if ((int) line.length() > 0) {
            char first = tolower(line[0]);
            if (first == 'y') {
                value = true;
                break;
            } else if (first == 'n') {
                value = false;
                break;
            }
        }
        std::cout << reprompt << std::endl;
        if (prompt == "") prompt = "Try again: ";
    }
    return value;
}

/*
 * Implementation notes: getLine
 * -----------------------------
 * The getLine function simply combines the process of displaying a
 * prompt and reading an input line into a single call.  The primary
 * reason for including this function in the library is to ensure
 * that the process of reading integers, floating-point numbers, and
 * strings remains as consistent as possible.
 */

std::string getLine(std::string prompt) {
    appendSpace(prompt);
    std::string line;
    std::cout << prompt;
    getline(std::cin, line);
    if (autograder::getConsoleEchoUserInput()) {
        std::cout << line << std::endl;
    }
    return line;
}

void getLine(std::string prompt, std::string& out) {
    appendSpace(prompt);
    std::string line;
    std::cout << prompt;
    getline(std::cin, out);
    if (autograder::getConsoleEchoUserInput()) {
        std::cout << line << std::endl;
    }
}

void getLine(std::istream& input, std::string& out) {
    getline(input, out);
    if (autograder::getConsoleEchoUserInput() && &input == &std::cin) {
        std::cout << out << std::endl;
    }
}

static void appendSpace(std::string& prompt) {
    if (!prompt.empty() && !isspace(prompt[prompt.length() - 1])) {
        prompt += ' ';
    }
}
