// Copyright 2016 Adam B. Singer
// Contact: PracticalDesignBook@gmail.com
//
// This file is part of pdCalc.
//
// pdCalc is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// pdCalc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with pdCalc; if not, see <http://www.gnu.org/licenses/>.

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "utilities/UserInterface.h"
#include <string>

namespace pdCalc {

class CommandButton;

class MainWindow : public QMainWindow, public UserInterface
{
    class MainWindowImpl;
public:
    MainWindow(int argc, char* argv[], QWidget* parent = nullptr);

    void postMessage(const std::string& m) override;
    void stackChanged() override;

    // Add a command button, for example, from a plugin
    // Buttons are added in order from left to right just below the
    // line undo, redo, proc from the bottom up
    // Buttons are reparented to MainWindow
    void addCommandButton(const std::string& dispPrimaryCmd, const std::string& primaryCmd, const std::string& dispShftCmd, const std::string& shftCmd);

    // setup the undo, redo, proc buttons after inserting plugin buttons
    void setupFinalButtons();

    // force the window to be fixed size...should be called as a final step once the GUI has fixed its own size
    void fixSize();

private:
    MainWindowImpl* pimpl_;
};

}

#endif
