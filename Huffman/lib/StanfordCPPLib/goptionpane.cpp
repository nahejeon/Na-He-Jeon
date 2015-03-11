/*
 * File: goptionpane.cpp
 * ---------------------
 * This file implements the operations declared in the goptionpane.h file.
 * See goptionpane.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2014/10/09
 * - finished implementation of showOptionDialog; added dialog types and other fixes
 * - enum rearrangement for clearer client code and better error messages
 * - removed 'using namespace' statement
 * @since 2014/07/09
 */

#include "goptionpane.h"
#include "platform.h"

static Platform* pp = getPlatform();

GOptionPane::GOptionPane() {
    // empty
}

GOptionPane::ConfirmResult GOptionPane::showConfirmDialog(std::string message, std::string title,
                                                          ConfirmType type) {
    if (type != GOptionPane::ConfirmType::YES_NO
            && type != GOptionPane::ConfirmType::YES_NO_CANCEL
            && type != GOptionPane::ConfirmType::OK_CANCEL) {
        error("GOptionPane::showConfirmDialog: Illegal dialog type");
    }
    int result = pp->goptionpane_showConfirmDialog(message, title, type);
    if (result == OK_OPTION) {
        return GOptionPane::ConfirmResult::OK;
    } else if (result == YES_OPTION) {
        return GOptionPane::ConfirmResult::YES;
    } else if (result == NO_OPTION) {
        return GOptionPane::ConfirmResult::NO;
    } else {
        return GOptionPane::ConfirmResult::CANCEL;
    }
}

std::string GOptionPane::showInputDialog(std::string message, std::string title) {
    return pp->goptionpane_showInputDialog(message, title);
}

void GOptionPane::showMessageDialog(std::string message, std::string title, MessageType type) {
    if (type != GOptionPane::MessageType::PLAIN
            && type != GOptionPane::MessageType::INFORMATION
            && type != GOptionPane::MessageType::ERROR
            && type != GOptionPane::MessageType::WARNING
            && type != GOptionPane::MessageType::QUESTION) {
        error("GOptionPane::showMessageDialog: Illegal dialog type");
    }
    pp->goptionpane_showMessageDialog(message, title, type);
}

std::string GOptionPane::showOptionDialog(std::string message, const Vector<std::string>& options,
                                          std::string title, std::string initiallySelected) {
    int index = pp->goptionpane_showOptionDialog(message, title, options.toStlVector(), initiallySelected);
    if (index == GOptionPane::InternalResult::CLOSED_OPTION
            || index < 0 || index >= options.size()) {
        return "";
    } else {
        return options[index];
    }
}
