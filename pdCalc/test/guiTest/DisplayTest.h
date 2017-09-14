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

#ifndef DISPLAY_TEST_H
#define DISPLAY_TEST_H

#include <QtTest/QtTest>
#include <string>
#include <vector>

namespace pdCalc {
    class Display;
    class GuiModel;
}

class DisplayTest : public QObject
{
    Q_OBJECT
public:
    DisplayTest();

private slots:
    void init();
    void cleanup();

    void testUpdateStack();
    void testInput();
    void testState();
    void testPlusMinus();

private:
    std::string createLine(unsigned int stackLabel, const std::string& txt) const;
    bool displayMatches(const std::string& input, const std::vector<double>&) const;
    void testInput(const std::vector<char> &input, const std::vector<double>&);

    pdCalc::Display* display_;
    pdCalc::GuiModel* guiModel_;
    class QApplication* app_;

    int nCharWide_;
    int nLinesStack_;
};

#endif
