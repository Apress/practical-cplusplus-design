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

#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include <vector>
#include <QValidator>
#include <string>

namespace pdCalc {

class GuiModel;

// This class is the calculator's display. It shows a number of lines of the stack
// as determined by the constructor and shows the input as it is entered

class Display : public QWidget
{
    class DisplayImpl;
    Q_OBJECT
public:
    // nLinesStack is the number of vertical lines of the stack to be displayed
    // minCharWide is the minimum number of characters wide for the display (defaults to min size)
    explicit Display(const GuiModel& g, QWidget* parent = nullptr, int nLinesStack = 6, int minCharWide = 25);

    // this is for testing purposes to be able to get the current display
    std::string getText() const;

    // show message in the status bar
    void showMessage(const std::string& m);
    
public slots:
    // call this slot when the underlying model changes
    void onModelChanged();

private:
    DisplayImpl* pimpl_;
    
};

}

#endif
