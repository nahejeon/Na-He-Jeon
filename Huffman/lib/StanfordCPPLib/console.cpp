/*
 * File: console.cpp
 * -----------------
 * This file implements the console.h interface.
 *
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * 2014/07/08
 * - modified clearConsole to print '=== cleared ===' message on normal text console
 *   for better matching against expected output log files
 */

#include "console.h"
#include <cstdio>
#include <string>
#include "error.h"
#include "platform.h"

static void sclTerminateHandler();

static Platform* pp = getPlatform();
static bool consoleEcho = false;
static bool consoleEventOnClose = false;
static bool consoleExitProgramOnClose = false;
static bool consoleLocationSaved = false;
static bool consolePrintExceptions = false;
static void (*old_terminate)() = NULL;

void clearConsole() {
    std::string msg = "==================== (console cleared) ====================";
    if (autograder::isConsoleAutograderMode()) {
        // don't actually clear the window, just display 'cleared' message on it
        std::cout << msg << std::endl;
    } else {
        // print to standard console (not Stanford graphical console)
        printf("%s\n", msg.c_str());

        // clear the graphical console window
        pp->jbeconsole_clear();
    }
}

bool getConsoleEcho() {
    return consoleEcho;
}

bool getConsolePrintExceptions() {
    return consolePrintExceptions;
}

void setConsoleEcho(bool echo) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    consoleEcho = echo;
}

void setConsoleExitProgramOnClose(bool exitOnClose) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    consoleExitProgramOnClose = exitOnClose;
    pp->jbeconsole_setExitProgramOnClose(exitOnClose);
}

bool getConsoleExitProgramOnClose() {
    return consoleExitProgramOnClose;
}

void setConsoleEventOnClose(bool eventOnClose) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    consoleEventOnClose = eventOnClose;
}

bool getConsoleEventOnClose() {
    return consoleEventOnClose;
}

void setConsoleLocationSaved(bool value) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    consoleLocationSaved = value;
    pp->jbeconsole_setLocationSaved(value);
}

bool getConsoleLocationSaved() {
    return consoleLocationSaved;
}

void setConsoleFont(const std::string& font) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    pp->jbeconsole_setFont(font);
}

void setConsolePrintExceptions(bool printExceptions) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    if (printExceptions && !consolePrintExceptions) {
        old_terminate = std::set_terminate(sclTerminateHandler);
    } else if (!printExceptions && consolePrintExceptions) {
        std::set_terminate(old_terminate);
    }
    consolePrintExceptions = printExceptions;
}

void setConsoleSize(double width, double height) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    pp->jbeconsole_setSize(width, height);
}

void setConsoleLocation(int x, int y) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    pp->jbeconsole_setLocation(x, y);
}

namespace autograder {
static bool consoleEchoUserInput = false;
static bool consoleAutograderMode = false;
static bool consoleLocked = false;
static std::string consoleLogFile = "";

bool getConsoleEchoUserInput() {
    return consoleEchoUserInput;
}

std::string getConsoleLogFile() {
    return consoleLogFile;
}

bool isConsoleAutograderMode() {
    return consoleAutograderMode;
}

void setConsoleAutograderMode(bool value) {
    consoleAutograderMode = value;
}

void setConsoleEchoUserInput(bool echo) {
    consoleEchoUserInput = echo;
}

void setConsoleLogFile(const std::string& filename) {
    if (autograder::isConsoleSettingsLocked()) { return; }
    consoleLogFile = filename;
}

void setConsoleSettingsLocked(bool value) {
    consoleLocked = value;
}

bool isConsoleSettingsLocked() {
    return consoleLocked;
}

void addInputButton(const std::string& text, const std::string& input) {
    pp->autograderinput_addButton(text, input);
}

void addInputCategory(const std::string& name) {
    pp->autograderinput_addCategory(name);
}

void removeInputCategory(const std::string& name) {
    pp->autograderinput_removeCategory(name);
}

void removeInputButton(const std::string& text) {
    pp->autograderinput_removeButton(text);
}
}

static void sclTerminateHandler() {
    std::ostream& out = std::cerr;
    try {
        throw;   // re-throws the exception that already occurred
    } catch (const ErrorException& ex) {
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** An ErrorException occurred during program execution: \n";
        msg += " *** ";
        msg += ex.what();
        msg += "\n ***\n\n";
        std::cout.flush();
        out << msg;
        throw ex;
    } catch (const InterruptedIOException& /* iex */) {
        // blocked console I/O was interrupted; just exit program immediately
        // (doesn't close any other JBE-generated GUI windows, but oh well)
        std::cout.flush();
        exit(0);
    } catch (const std::exception& ex) {
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** An exception occurred during program execution: \n";
        msg += " *** ";
        msg += ex.what();
        msg += "\n ***\n\n";
        std::cout.flush();
        out << msg;
        throw ex;
    } catch (std::string str) {
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A string exception occurred during program execution: \n";
        msg += " *** \"";
        msg += str;
        msg += "\"\n ***\n";
        std::cout.flush();
        out << msg;
        throw str;
    } catch (char const* str) {
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A string exception occurred during program execution: \n";
        msg += " *** \"";
        msg += str;
        msg += "\"\n ***\n";
        std::cout.flush();
        out << msg;
        throw str;
    } catch (int n) {
        char buf[128];
        snprintf(buf, 128, "%d", n);
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** An int exception occurred during program execution: \n";
        msg += " *** ";
        msg += buf;
        msg += "\n ***\n\n";
        std::cout.flush();
        out << msg;
        throw n;
    } catch (long l) {
        char buf[128];
        snprintf(buf, 128, "%ld", l);
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A long exception occurred during program execution: \n";
        msg += " *** ";
        msg += buf;
        msg += "\n ***\n\n";
        std::cout.flush();
        out << msg;
        throw l;
    } catch (char c) {
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A char exception occurred during program execution: \n";
        msg += " *** '";
        msg += c;
        msg += "'\n ***\n";
        std::cout.flush();
        out << msg;
        throw c;
    } catch (bool b) {
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A bool exception occurred during program execution: \n";
        msg += " *** ";
        msg += (b ? "true" : "false");
        msg += "\n ***\n\n";
        std::cout.flush();
        out << msg;
        throw b;
    } catch (double d) {
        char buf[128];
        snprintf(buf, 128, "%lf", d);
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A double exception occurred during program execution: \n";
        msg += " *** ";
        msg += buf;
        msg += "\n ***\n\n";
        std::cout.flush();
        out << msg;
        throw d;
    }
}
