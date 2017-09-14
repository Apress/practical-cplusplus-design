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

#include "StackTest.h"
#include "src/backend/Stack.h"
#include "src/utilities/Observer.h"
#include "src/utilities/Exception.h"
#include <vector>

using std::vector;
using std::vector;
using std::string;
using std::shared_ptr;
using std::unique_ptr;

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

class StackErrorObserver : public pdCalc::Observer
{
public:
    StackErrorObserver(string name);
    const vector<string>& errorMessages() const { return messages_; }
    const vector<pdCalc::StackEventData::ErrorConditions>& errors() const { return errors_; }
    void notifyImpl(std::shared_ptr<pdCalc::EventData>);

private:
    vector<string> messages_;
    vector<pdCalc::StackEventData::ErrorConditions> errors_;
};

StackErrorObserver::StackErrorObserver(string name)
: pdCalc::Observer{name}
{
}

void StackErrorObserver::notifyImpl(std::shared_ptr<pdCalc::EventData> data)
{
    std::shared_ptr<pdCalc::StackEventData> p = std::dynamic_pointer_cast<pdCalc::StackEventData>(data);
    if(p)
    {
        messages_.push_back(p->message());
        errors_.push_back(p->error());
    }

    return;
}

void StackTest::testPushPop()
{
    pdCalc::Stack& stack = pdCalc::Stack::Instance();
    stack.clear();
    StackChangedObserver* raw = new StackChangedObserver{"StackChangedObserver"};
    stack.attach( pdCalc::Stack::StackChanged, unique_ptr<pdCalc::Observer>{raw} );

    QVERIFY( stack.size() == 0 );

    vector<double> cur{ stack.getElements(100) };

    QVERIFY( cur.size() == 0 );

    stack.push(3.14159);
    stack.push(17.3);

    QVERIFY( stack.size() == 2 );

    stack.push(2.0);

    cur = stack.getElements(2);

    QCOMPARE( cur[0], 2.0 );
    QCOMPARE( cur[1], 17.3 );

    cur = stack.getElements(3);

    QCOMPARE( cur[0], 2.0 );
    QCOMPARE( cur[1], 17.3 );
    QCOMPARE( cur[2], 3.14159 );

    double val = stack.pop();
    QCOMPARE(val, 2.0);
    QVERIFY( stack.size() == 2 );

    cur = stack.getElements(2);
    QCOMPARE( cur[0], 17.3 );
    QCOMPARE( cur[1], 3.14159 );

    stack.push(3.0);

    cur = stack.getElements(3);

    QCOMPARE( cur[0], 3.0 );
    QCOMPARE( cur[1], 17.3 );
    QCOMPARE( cur[2], 3.14159 );

    QCOMPARE(raw->changeCount(), 5u);

    stack.push(5.0, true);
    stack.pop(true);

    QCOMPARE(raw->changeCount(), 5u);

    stack.clear();
    stack.detach(pdCalc::Stack::StackChanged, "StackChangedObserver");

    return;
}

void StackTest::testSwapTop()
{
    pdCalc::Stack& stack = pdCalc::Stack::Instance();
    stack.clear();
    StackChangedObserver* raw = new StackChangedObserver{"StackChangedObserver"};
    stack.attach( pdCalc::Stack::StackChanged, unique_ptr<pdCalc::Observer>{raw} );

    QVERIFY( stack.size() == 0 );

    stack.push(3.14159);
    stack.push(17.3);

    vector<double> cur{ stack.getElements(2) };
    QCOMPARE( cur[0], 17.3 );
    QCOMPARE( cur[1], 3.14159 );

    stack.swapTop();

    cur = stack.getElements(2);
    QCOMPARE( cur[0], 3.14159 );
    QCOMPARE( cur[1], 17.3 );

    stack.push(2.0);

    cur = stack.getElements(3);
    QCOMPARE( cur[0], 2.0 );
    QCOMPARE( cur[1], 3.14159 );
    QCOMPARE( cur[2], 17.3 );

    stack.swapTop();

    cur = stack.getElements(3);
    QCOMPARE( cur[0], 3.14159 );
    QCOMPARE( cur[1], 2.0 );
    QCOMPARE( cur[2], 17.3 );

    QCOMPARE(raw->changeCount(), 5u);

    stack.clear();
    stack.detach(pdCalc::Stack::StackChanged, "StackChangedObserver");

    return;
}

void StackTest::testErrors()
{
    pdCalc::Stack& stack = pdCalc::Stack::Instance();
    stack.clear();
    StackErrorObserver* raw = new StackErrorObserver{"StackErrorObserver"};
    stack.attach( pdCalc::Stack::StackError, unique_ptr<pdCalc::Observer>{raw} );

    const string emptyMsg = pdCalc::StackEventData::Message(pdCalc::StackEventData::ErrorConditions::Empty);
    const string swapMsg = pdCalc::StackEventData::Message(pdCalc::StackEventData::ErrorConditions::TooFewArguments);

    try
    {
        stack.pop();
        QVERIFY(false);
    }
    catch(pdCalc::Exception& e)
    {
        QCOMPARE(e.what(), emptyMsg);
    }

    try
    {
        stack.swapTop();
        QVERIFY(false);
    }
    catch(pdCalc::Exception& e)
    {
        QCOMPARE(e.what(), swapMsg);
    }

    const vector<string>& msgs { raw->errorMessages() };
    QVERIFY(msgs.size() == 2);
    QCOMPARE(msgs[0], emptyMsg);
    QCOMPARE(msgs[1], swapMsg);

    const vector<pdCalc::StackEventData::ErrorConditions> errors { raw->errors() };
    QVERIFY(errors.size() == 2);
    QCOMPARE(errors[0], pdCalc::StackEventData::ErrorConditions::Empty);
    QCOMPARE(errors[1], pdCalc::StackEventData::ErrorConditions::TooFewArguments);

    stack.clear();
    stack.detach(pdCalc::Stack::StackError, "StackErrorObserver");
}

