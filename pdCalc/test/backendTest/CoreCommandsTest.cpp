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

#include "CoreCommandsTest.h"
#include "src/backend/Stack.h"
#include "src/backend/CoreCommands.h"
#include "src/utilities/Exception.h"
#include "src/utilities/Observer.h"
#include <vector>
#include <cmath>
#include <string>
#include <memory>

using std::vector;
using std::string;

namespace {

double stackTop()
{
    vector<double> v;
    pdCalc::Stack::Instance().getElements(1, v);
    return v.front();
}

class StackChangedObserver : public pdCalc::Observer
{
public:
    StackChangedObserver(string name);
    unsigned int changeCount() const { return changeCount_; }
    void notifyImpl(std::shared_ptr<pdCalc::EventData>);

private:
    unsigned int changeCount_;
};

StackChangedObserver::StackChangedObserver(string name)
: pdCalc::Observer{name}
, changeCount_{0}
{
}

void StackChangedObserver::notifyImpl(std::shared_ptr<pdCalc::EventData>)
{
    ++changeCount_;
}

StackChangedObserver* raw;

}

void CoreCommandsTest::init()
{
    pdCalc::Stack::Instance().clear();
    raw = new StackChangedObserver{"StackChangedObserver"};
    pdCalc::Stack::Instance().attach( pdCalc::Stack::StackChanged, std::unique_ptr<pdCalc::Observer>{raw} );
    return;
}

void CoreCommandsTest::cleanup()
{
    pdCalc::Stack::Instance().detach( pdCalc::Stack::StackChanged, "StackChangedObserver");

    return;
}

namespace {

void checkStack()
{
    QVERIFY( pdCalc::Stack::Instance().size() == 0 );
}

}

pdCalc::Stack& CoreCommandsTest::getCheckedStack()
{
    checkStack();
    return pdCalc::Stack::Instance();
}

void CoreCommandsTest::testEnterNumber()
{
    pdCalc::Stack& stack = getCheckedStack();

    double number = 7.3;

    pdCalc::EnterNumber en{number};

    pdCalc::Command* command = &en;

    command->execute();

    QVERIFY( stack.size() == 1 );
    QCOMPARE( stackTop(), number );
    QVERIFY( raw->changeCount() == 1 );

    command->undo();

    QVERIFY( stack.size() == 0 );
    QCOMPARE( raw->changeCount(), 2u );

    return;
}

// just test that it cloned to the correct type, not that the commands
// are equal
template<typename T>
void CoreCommandsTest::testClone(pdCalc::Command& c)
{
    pdCalc::Command* clone = c.clone();

    QVERIFY( dynamic_cast<T*>(clone) != nullptr );

    delete clone;

    return;
}

void CoreCommandsTest::testEnterNumberClone()
{
    pdCalc::EnterNumber en{1};
    testClone<pdCalc::EnterNumber>(en);

    return;
}

void CoreCommandsTest::testSwapTopOfStackPreconditions()
{
    pdCalc::Stack& stack = getCheckedStack();

    pdCalc::SwapTopOfStack stos;
    pdCalc::Command* command = &stos;

    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    double number1 = 2.2;
    stack.push(number1);
    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    double number2 = 3.3;
    stack.push(number2);
    try
    {
        command->execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    return;
}

void CoreCommandsTest::testSwapTopOfStackClone()
{
    pdCalc::SwapTopOfStack c;
    testClone<pdCalc::SwapTopOfStack>(c);

    return;
}

void CoreCommandsTest::testSwapTopOfStack()
{
    pdCalc::Stack& stack = getCheckedStack();

    pdCalc::SwapTopOfStack stos;
    pdCalc::Command* command = &stos;

    double number1 = 2.2;
    stack.push(number1);

    double number2 = 3.3;
    stack.push(number2);

    vector<double> v = stack.getElements(2);
    QCOMPARE( v[0], number2 );
    QCOMPARE( v[1], number1 );

    command->execute();

    QCOMPARE( raw->changeCount(), 3u );

    v = stack.getElements(2);
    QCOMPARE( v[0], number1 );
    QCOMPARE( v[1], number2 );

    command->undo();

    QCOMPARE( raw->changeCount(), 4u );

    v = stack.getElements(2);
    QCOMPARE( v[0], number2 );
    QCOMPARE( v[1], number1 );

    return;
}

void CoreCommandsTest::testDropTopOfStackPreconditions()
{
    pdCalc::Stack& stack = getCheckedStack();

    pdCalc::DropTopOfStack dtos;
    pdCalc::Command* command = &dtos;

    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    double number1 = 2.2;
    stack.push(number1);
    try
    {
        command->execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    return;
}

void CoreCommandsTest::testDropTopOfStackClone()
{
    pdCalc::DropTopOfStack c;
    testClone<pdCalc::DropTopOfStack>(c);

    return;
}

void CoreCommandsTest::testDropTopOfStack()
{
    pdCalc::Stack& stack = getCheckedStack();

    pdCalc::DropTopOfStack dtos;
    pdCalc::Command* command = &dtos;

    double number = 14.3;
    stack.push(number);

    QVERIFY( stack.size() == 1 );

    command->execute();

    QVERIFY( stack.size() == 0 );

    command->undo();

    QVERIFY( stack.size() == 1 );
    QCOMPARE( stackTop(), number );

    return;
}

void CoreCommandsTest::testClearStackClone()
{
    pdCalc::ClearStack c;
    testClone<pdCalc::ClearStack>(c);
}

void CoreCommandsTest::testClearStack()
{
    pdCalc::Stack& stack = getCheckedStack();

    pdCalc::ClearStack cs;
    pdCalc::Command* command = &cs;

    command->execute();

    QVERIFY( stack.size() == 0 );

    command->undo();

    QVERIFY( stack.size() == 0 );

    stack.push(4);
    stack.push(5);
    stack.push(6);
    stack.push(7);

    QVERIFY( stack.size() == 4 );

    command->execute();

    QVERIFY( stack.size() == 0 );

    command->undo();

    QVERIFY( stack.size() == 4 );

    auto v = stack.getElements(4);
    QCOMPARE(v[0], 7.0);
    QCOMPARE(v[1], 6.0);
    QCOMPARE(v[2], 5.0);
    QCOMPARE(v[3], 4.0);

    return;
}

void CoreCommandsTest::testBinaryCommandPreconditions(pdCalc::Command& command)
{
    pdCalc::Stack& stack = getCheckedStack();

    try
    {
        command.execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    double number = 1.0;

    stack.push(number);

    try
    {
        command.execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    stack.push(number);

    try
    {
        command.execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    return;
}

void CoreCommandsTest::testBinaryCommand(pdCalc::Command& command, double top, double next, double result)
{
    pdCalc::Stack& stack = getCheckedStack();

    stack.push(next);
    stack.push(top);

    QVERIFY( stack.size() == 2 );

    command.execute();

    QCOMPARE( raw->changeCount(), 3u );
    QVERIFY( stack.size() == 1 );
    QCOMPARE( stackTop(), result );

    command.undo();

    QCOMPARE( raw->changeCount(), 4u );
    QVERIFY( stack.size() == 2 );
    vector<double> vals;
    stack.getElements(2, vals);
    QCOMPARE( vals[0], top );
    QCOMPARE( vals[1], next );

    return;
}

void CoreCommandsTest::testUnaryCommandPreconditions(pdCalc::Command& command)
{
    pdCalc::Stack& stack = getCheckedStack();

    try
    {
        command.execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    double number = 1.0;

    stack.push(number);

    try
    {
        command.execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    return;
}

void CoreCommandsTest::testUnaryCommand(pdCalc::Command& command, double top, double result)
{
    pdCalc::Stack& stack = getCheckedStack();

    stack.push(top);

    QVERIFY( stack.size() == 1 );

    command.execute();

    QCOMPARE( raw->changeCount(), 2u );
    QVERIFY( stack.size() == 1 );
    QCOMPARE( stackTop(), result );

    command.undo();

    QCOMPARE( raw->changeCount(), 3u );
    QVERIFY( stack.size() == 1 );
    QCOMPARE( stackTop(), top);

    return;
}

void CoreCommandsTest::testAddPreconditions()
{
    pdCalc::Add add;
    testBinaryCommandPreconditions(add);

    return;
}

void CoreCommandsTest::testAddClone()
{
    pdCalc::Add c;
    testClone<pdCalc::Add>(c);

    return;
}

void CoreCommandsTest::testAdd()
{
    pdCalc::Add add;

    double top = 2.0;
    double next = 5.0;
    double result = next + top;

    testBinaryCommand(add, top, next, result);

    return;
}

void CoreCommandsTest::testSubtractPreconditions()
{
    pdCalc::Subtract sub;
    testBinaryCommandPreconditions(sub);

    return;
}

void CoreCommandsTest::testSubtractClone()
{
    pdCalc::Subtract c;
    testClone<pdCalc::Subtract>(c);

    return;
}

void CoreCommandsTest::testSubtract()
{
    pdCalc::Subtract sub;
    double top = 2.0;
    double next = 0.5;
    double result = next - top;

    testBinaryCommand(sub, top, next, result);

    return;
}

void CoreCommandsTest::testDividePreconditions()
{
    pdCalc::Divide div;
    testBinaryCommandPreconditions(div);

    double zero = 0.;
    pdCalc::Stack::Instance().push(zero);

    pdCalc::Command* command = &div;
    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    zero = -0.;
    pdCalc::Stack::Instance().push(zero);

    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    return;
}

void CoreCommandsTest::testDivideClone()
{
    pdCalc::Divide c;
    testClone<pdCalc::Divide>(c);

    return;
}

void CoreCommandsTest::testDivide()
{
    pdCalc::Divide div;
    double top = 0.34;
    double next = 1.2e3;
    double result = next / top;

    testBinaryCommand(div, top, next, result);

    return;
}

void CoreCommandsTest::testSinePreconditions()
{
    pdCalc::Sine sine;
    testUnaryCommandPreconditions(sine);

    return;
}

void CoreCommandsTest::testSineClone()
{
    pdCalc::Sine c;
    testClone<pdCalc::Sine>(c);

    return;
}

void CoreCommandsTest::testSine()
{
    pdCalc::Sine sine;
    double top = 1.34;
    double result = std::sin(top);

    testUnaryCommand(sine, top, result);

    return;
}

void CoreCommandsTest::testCosinePreconditions()
{
    pdCalc::Cosine cosine;
    testUnaryCommandPreconditions(cosine);

    return;
}

void CoreCommandsTest::testCosineClone()
{
    pdCalc::Cosine c;
    testClone<pdCalc::Cosine>(c);

    return;
}

void CoreCommandsTest::testCosine()
{
    pdCalc::Cosine cosine;
    double top = 2.4;
    double result = std::cos(top);

    testUnaryCommand(cosine, top, result);

    return;
}

void CoreCommandsTest::testTangentPreconditions()
{
    pdCalc::Tangent tangent;
    testUnaryCommandPreconditions(tangent);

    pdCalc::Stack::Instance().push(M_PI);
    pdCalc::Command* command = &tangent;
    try
    {
        command->execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    pdCalc::Stack::Instance().push(M_PI / 2. + 1e-8);
    try
    {
        command->execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    pdCalc::Stack::Instance().push(M_PI / 2.);
    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    pdCalc::Stack::Instance().push(-3. * M_PI / 2.);
    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    return;
}

void CoreCommandsTest::testTangentClone()
{
    pdCalc::Tangent c;
    testClone<pdCalc::Tangent>(c);

    return;
}

void CoreCommandsTest::testTangent()
{
    pdCalc::Tangent tangent;
    double top = -2.34;
    double result = std::tan(top);

    testUnaryCommand(tangent, top, result);

    return;
}

void CoreCommandsTest::testArcsinArccosPreconditions(pdCalc::Command* command)
{
    pdCalc::Stack::Instance().push(1);
    try
    {
        command->execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    pdCalc::Stack::Instance().push(2);
    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    pdCalc::Stack::Instance().push(-1);
    try
    {
        command->execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    pdCalc::Stack::Instance().push(-2);
    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    pdCalc::Stack::Instance().push(0);
    try
    {
        command->execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    return;
}

void CoreCommandsTest::testArcsinePreconditions()
{
    pdCalc::Arcsine arcsin;
    testUnaryCommandPreconditions(arcsin);

    testArcsinArccosPreconditions(&arcsin);

    return;
}

void CoreCommandsTest::testArcsineClone()
{
    pdCalc::Arcsine c;
    testClone<pdCalc::Arcsine>(c);

    return;
}

void CoreCommandsTest::testArcsine()
{
    pdCalc::Arcsine arcsin;
    double top = 0.24;
    double result = std::asin(top);

    testUnaryCommand(arcsin, top, result);

    return;
}

void CoreCommandsTest::testArccosinePreconditions()
{
    pdCalc::Arccosine arccosine;
    testUnaryCommandPreconditions(arccosine);

    testArcsinArccosPreconditions(&arccosine);

    return;
}

void CoreCommandsTest::testArccosineClone()
{
    pdCalc::Arccosine c;
    testClone<pdCalc::Arccosine>(c);

    return;
}

void CoreCommandsTest::testArccosine()
{
    pdCalc::Arccosine arccos;
    double top = -0.82;
    double result = std::acos(top);

    testUnaryCommand(arccos, top, result);
}

void CoreCommandsTest::testArctangentPreconditions()
{
    pdCalc::Arctangent arctan;
    testUnaryCommandPreconditions(arctan);

    return;
}

void CoreCommandsTest::testArctangentClone()
{
    pdCalc::Arctangent c;
    testClone<pdCalc::Arctangent>(c);

    return;
}

void CoreCommandsTest::testArctangent()
{
    pdCalc::Arctangent arctan;
    double top = 35;
    double result = std::atan(top);

    testUnaryCommand(arctan, top, result);

    return;
}

void CoreCommandsTest::testPowerPreconditions()
{
    pdCalc::Power power;
    testBinaryCommandPreconditions(power);

    pdCalc::Command* command = &power;

    pdCalc::Stack::Instance().push(5.0);
    pdCalc::Stack::Instance().push(6.0);
    try
    {
        command->execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    pdCalc::Stack::Instance().push(-5.0);
    pdCalc::Stack::Instance().push(-6.0);
    try
    {
        command->execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    pdCalc::Stack::Instance().push(-5.0);
    pdCalc::Stack::Instance().push(6.0);
    try
    {
        command->execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    pdCalc::Stack::Instance().push(-5.0);
    pdCalc::Stack::Instance().push(6.1);
    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    pdCalc::Stack::Instance().push(0);
    pdCalc::Stack::Instance().push(-1);
    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    pdCalc::Stack::Instance().push(0);
    pdCalc::Stack::Instance().push(1);
    try
    {
        command->execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    return;
}

void CoreCommandsTest::testPowerClone()
{
    pdCalc::Power c;
    testClone<pdCalc::Power>(c);

    return;
}

void CoreCommandsTest::testPower()
{
    pdCalc::Power power;
    double top = 3.4;
    double next = 2.7;
    double result = std::pow(next, top);

    testBinaryCommand(power, top, next, result);
}

void CoreCommandsTest::testRootPreconditions()
{
    pdCalc::Root root;
    testBinaryCommandPreconditions(root);

    pdCalc::Command* command = &root;

    pdCalc::Stack::Instance().push(5.0);
    pdCalc::Stack::Instance().push(6.0);
    try
    {
        command->execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    pdCalc::Stack::Instance().push(-5.0);
    pdCalc::Stack::Instance().push(-0.5);
    try
    {
        command->execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    pdCalc::Stack::Instance().push(-5.0);
    pdCalc::Stack::Instance().push(6.0);
    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    pdCalc::Stack::Instance().push(-5.0);
    pdCalc::Stack::Instance().push(6.1);
    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    pdCalc::Stack::Instance().push(0);
    pdCalc::Stack::Instance().push(-1);
    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    pdCalc::Stack::Instance().push(0);
    pdCalc::Stack::Instance().push(1);
    try
    {
        command->execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    pdCalc::Stack::Instance().push(4.3);
    pdCalc::Stack::Instance().push(0);
    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    return;
}

void CoreCommandsTest::testRootClone()
{
    pdCalc::Root c;
    testClone<pdCalc::Root>(c);

    return;
}

void CoreCommandsTest::testRoot()
{
    pdCalc::Root root;
    double top = 2.8;
    double next = 35.76;
    double result = std::pow(next, 1. / top);

    testBinaryCommand(root, top, next, result);

    return;
}

void CoreCommandsTest::testNegatePreconditions()
{
    pdCalc::Negate neg;
    testUnaryCommandPreconditions(neg);

    return;
}

void CoreCommandsTest::testNegateClone()
{
    pdCalc::Negate c;
    testClone<pdCalc::Negate>(c);

    return;
}

void CoreCommandsTest::testNegate()
{
    pdCalc::Negate neg;
    double top = 5.34;
    double result = -top;

    testUnaryCommand(neg, top, result);

    return;
}

void CoreCommandsTest::testDuplicatePreconditions()
{
    pdCalc::Stack& stack = getCheckedStack();
    pdCalc::Duplicate dup;
    pdCalc::Command& c = dup;

    try
    {
        c.execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    stack.push(15.);
    try
    {
        c.execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }
}

void CoreCommandsTest::testDuplicateClone()
{
    pdCalc::Duplicate c;
    testClone<pdCalc::Duplicate>(c);

    return;
}

void CoreCommandsTest::testDuplicate()
{
    pdCalc::Stack& stack = getCheckedStack();
    double num = 14.0;
    stack.push(num);

    QVERIFY( stack.size() == 1 );
    pdCalc::Duplicate dup;
    pdCalc::Command& c = dup;
    c.execute();

    QVERIFY( stack.size() == 2 );

    vector<double> v = stack.getElements(2);

    QCOMPARE(v[0], num);
    QCOMPARE(v[1], num);
}
