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

#ifndef CORE_COMMANDS_TEST_H
#define CORE_COMMANDS_TEST_H

#include <QtTest/QtTest>

namespace pdCalc {
    class Stack;
    class Command;
}

class CoreCommandsTest : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void testEnterNumber();
    void testEnterNumberClone();
    void testSwapTopOfStackPreconditions();
    void testSwapTopOfStackClone();
    void testSwapTopOfStack();
    void testDropTopOfStackPreconditions();
    void testDropTopOfStackClone();
    void testDropTopOfStack();
    void testClearStackClone();
    void testClearStack();
    void testAddPreconditions();
    void testAddClone();
    void testAdd();
    void testSubtractPreconditions();
    void testSubtractClone();
    void testSubtract();
    void testDividePreconditions();
    void testDivideClone();
    void testDivide();
    void testSinePreconditions();
    void testSineClone();
    void testSine();
    void testCosinePreconditions();
    void testCosineClone();
    void testCosine();
    void testTangentPreconditions();
    void testTangentClone();
    void testTangent();
    void testArcsinePreconditions();
    void testArcsineClone();
    void testArcsine();
    void testArccosinePreconditions();
    void testArccosineClone();
    void testArccosine();
    void testArctangentPreconditions();
    void testArctangentClone();
    void testArctangent();
    void testPowerPreconditions();
    void testPowerClone();
    void testPower();
    void testRootPreconditions();
    void testRootClone();
    void testRoot();
    void testNegatePreconditions();
    void testNegateClone();
    void testNegate();
    void testDuplicatePreconditions();
    void testDuplicateClone();
    void testDuplicate();

private:
    pdCalc::Stack& getCheckedStack();
    void testBinaryCommandPreconditions(pdCalc::Command&);
    void testBinaryCommand(pdCalc::Command&, double top, double next, double result);
    void testUnaryCommandPreconditions(pdCalc::Command&);
    void testUnaryCommand(pdCalc::Command&, double top, double result);
    template<typename T> void testClone(pdCalc::Command& c);
    void testArcsinArccosPreconditions(pdCalc::Command* command);
};

#endif
