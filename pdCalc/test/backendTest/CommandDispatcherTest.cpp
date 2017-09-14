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

#include "CommandDispatcherTest.h"
#include "src/utilities/UserInterface.h"
#include "src/backend/CoreCommands.h"
#include "src/backend/CommandDispatcher.h"
#include "src/backend/CommandRepository.h"
#include "src/backend/Stack.h"

#include <cmath>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::vector;
using std::ostringstream;
using std::string;

namespace {

class TestInterface : public pdCalc::UserInterface
{
public:
    TestInterface() : lastMessage_{""} { }
    void postMessage(const string& m) override { lastMessage_ = m; }
    void stackChanged() override { }
    const string& getLastMessage() const { return lastMessage_; }

    double top() const;

private:
    string lastMessage_;
};

double TestInterface::top() const
{
    vector<double> v{ pdCalc::Stack::Instance().getElements(1) };
    return v.back();
}

}

void CommandDispatcherTest::testCommandDispatcher()
{
    pdCalc::CommandRepository::Instance().clearAllCommands();
    pdCalc::Stack::Instance().clear();
    TestInterface ui;
    pdCalc::CommandDispatcher ce{ui};

    ce.commandEntered("+");
    QCOMPARE(ui.getLastMessage(), string{"Command + is not a known command"});

    pdCalc::RegisterCoreCommands(ui);

    ce.commandEntered("+");

    QCOMPARE(ui.getLastMessage(), string{"Stack must have 2 elements"});

    ce.commandEntered("1.0");
    ce.commandEntered("2.0");

    ce.commandEntered("+");

    QCOMPARE( ui.top(), 3.0 );

    ce.commandEntered("-18.3");
    ce.commandEntered("swap");
    QCOMPARE( ui.top(), 3.0 );

    ce.commandEntered("drop");
    QCOMPARE( ui.top(), -18.3);

    ce.commandEntered("22");
    ce.commandEntered("undo");
    QCOMPARE( ui.top(), -18.3);

    ce.commandEntered("undo");
    QCOMPARE(ui.top(), 3.0);

    ce.commandEntered("redo");
    QCOMPARE(ui.top(), -18.3);

    ce.commandEntered("undo");
    ce.commandEntered("-");
    QCOMPARE(ui.top(), -21.3);

    ce.commandEntered("neg");
    QCOMPARE(ui.top(), 21.3);

    ce.commandEntered("7.1");
    ce.commandEntered("/");
    QCOMPARE(ui.top(), 21.3 / 7.1 );

    ce.commandEntered("6");
    ce.commandEntered("2");
    ce.commandEntered("pow");
    QCOMPARE(ui.top(), 36.0);

    ce.commandEntered("27");
    ce.commandEntered("3");
    ce.commandEntered("root");
    QCOMPARE(ui.top(), 3.0);

    ce.commandEntered("*");;
    QCOMPARE(ui.top(), 108.0);

    ce.commandEntered("0.8");
    ce.commandEntered("sin");
    QCOMPARE(ui.top(), std::sin(0.8));

    ce.commandEntered("dup");
    ce.commandEntered("arcsin");
    ce.commandEntered("swap");
    double t = ui.top();
    ce.commandEntered("drop");
    QVERIFY(std::abs(ui.top() - std::asin(t)) < 1e-10);

    ce.commandEntered("0.7");
    ce.commandEntered("cos");
    QCOMPARE(ui.top(), std::cos(0.7));

    ce.commandEntered("dup");
    ce.commandEntered("arccos");
    ce.commandEntered("swap");
    t = ui.top();
    ce.commandEntered("drop");
    QVERIFY(std::abs(ui.top() - std::acos(t)) < 1e-10);

    ce.commandEntered("0.3");
    ce.commandEntered("tan");
    QCOMPARE(ui.top(), std::tan(0.3));

    ce.commandEntered("dup");
    ce.commandEntered("arctan");
    ce.commandEntered("swap");
    t = ui.top();
    ce.commandEntered("drop");
    QVERIFY(std::abs(ui.top() - std::atan(t)) < 1e-10);

    return;
}

