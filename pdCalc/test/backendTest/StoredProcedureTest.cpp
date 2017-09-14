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

#include "StoredProcedureTest.h"
#include "src/backend/StoredProcedure.h"
#include "src/utilities/UserInterface.h"
#include "backend/Stack.h"
#include "utilities/Exception.h"
#include <vector>
#include "backend/CoreCommands.h"
#include "backend/CommandRepository.h"
#include <sstream>

using std::vector;
using std::string;

namespace {

class TestInterface : public pdCalc::UserInterface
{
public:
    TestInterface() { }
    void postMessage(const string&) override {  }
    void stackChanged() override { }
};

}

void StoredProcedureTest::testMissingProcedure()
{
    TestInterface ui;

    try
    {
        pdCalc::StoredProcedure sp{ui, "DoesNotExist"};
        sp.execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception& e)
    {
        QCOMPARE(e.what(), string{"Could not open procedure"});
    }

    return;
}

void StoredProcedureTest::testStoredProcedure()
{
    pdCalc::CommandRepository::Instance().clearAllCommands();
    TestInterface ui;
    pdCalc::RegisterCoreCommands(ui);
    std::ostringstream oss;
    oss << BACKEND_TEST_DIR << "/hypotenuse";
    pdCalc::StoredProcedure sp(ui, oss.str());

    pdCalc::Stack& stack = pdCalc::Stack::Instance();
    stack.push(3.0);
    stack.push(4.0);

    try
    {
        sp.execute();
        QVERIFY(true);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(false);
    }

    vector<double> vals = stack.getElements(1);

    QCOMPARE(vals[0], 5.0);

    sp.undo();

    vals = stack.getElements(2);

    QCOMPARE(vals[0], 4.0);
    QCOMPARE(vals[1], 3.0);

    sp.execute();

    vals = stack.getElements(1);

    QCOMPARE(vals[0], 5.0);

    QVERIFY(sp.clone() == nullptr);

    return;
}
