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

#include "CommandRepositoryTest.h"
#include "src/backend/Command.h"
#include "src/backend/CommandRepository.h"
#include "src/utilities/Exception.h"
#include <sstream>

using std::ostringstream;
using std::set;
using std::string;

namespace {

class TestCommand : public pdCalc::Command
{
public:
    TestCommand(const string& optionalName = "");
    TestCommand(const TestCommand&);

    const string& getOptionalName() const { return optionalName_; }

private:
    void executeImpl() noexcept override { }
    void undoImpl() noexcept override { }
    TestCommand* cloneImpl() const noexcept override;
    const char* helpMessageImpl() const noexcept override { return ""; }

private:
    string optionalName_;
};

TestCommand::TestCommand(const string& n)
: Command{}
, optionalName_{n}
{

}

TestCommand::TestCommand(const TestCommand& rhs)
: Command{rhs}
, optionalName_{rhs.optionalName_}
{

}

TestCommand* TestCommand::cloneImpl() const  noexcept
{
    return new TestCommand{*this};
}

}

void CommandRepositoryTest::testRegister()
{
    pdCalc::CommandRepository& cf = pdCalc::CommandRepository::Instance();
    cf.clearAllCommands();

    string name1 = "command 1";
    string name2 = "command 2";
    set<string> names = { name1, name2 };

    cf.registerCommand( name1, pdCalc::MakeCommandPtr<TestCommand>() );
    cf.registerCommand( name2, pdCalc::MakeCommandPtr<TestCommand>() );

    QVERIFY( cf.getNumberCommands() == 2 );

    set<string> registeredCommands = cf.getAllCommandNames();

    QCOMPARE( registeredCommands, names );

    QCOMPARE( cf.hasKey(name1), true );
    QCOMPARE( cf.hasKey(name2), true );
    QCOMPARE( cf.hasKey("not present"), false );

    return;
}

void CommandRepositoryTest::testDuplicateRegister()
{
    pdCalc::CommandRepository& cf = pdCalc::CommandRepository::Instance();
    cf.clearAllCommands();

    string name = "command";

    cf.registerCommand( name, pdCalc::MakeCommandPtr<TestCommand>() );

    QVERIFY( cf.getNumberCommands() == 1 );

    try
    {
        cf.registerCommand( name, pdCalc::MakeCommandPtr<TestCommand>() );
        QVERIFY( false );
    }
    catch(pdCalc::Exception& e)
    {
        ostringstream oss;
        oss << "Command " << name << " already registered";
        QCOMPARE( e.what(), oss.str() );
    }

    QVERIFY( cf.getNumberCommands() == 1 );

    return;
}

void CommandRepositoryTest::testDeregister()
{
    pdCalc::CommandRepository& cf = pdCalc::CommandRepository::Instance();
    cf.clearAllCommands();

    string name1 = "command 1";
    string name2 = "command 2";

    cf.registerCommand( name1, pdCalc::MakeCommandPtr<TestCommand>() );
    cf.registerCommand( name2, pdCalc::MakeCommandPtr<TestCommand>() );

    QVERIFY( cf.getNumberCommands() == 2 );

    pdCalc::CommandPtr command = cf.deregisterCommand( name1 );

    QVERIFY( command != nullptr );
    QVERIFY( cf.getNumberCommands() == 1 );
    QCOMPARE( cf.hasKey(name1), false );
    QCOMPARE( cf.hasKey(name2), true );

    command = cf.deregisterCommand( "not present" );

    QVERIFY( command == nullptr );
    QVERIFY( cf.getNumberCommands() == 1 );
    QCOMPARE( cf.hasKey(name1), false );
    QCOMPARE( cf.hasKey(name2), true );

    return;
}

void CommandRepositoryTest::testAllocateCommand()
{
    pdCalc::CommandRepository& cf = pdCalc::CommandRepository::Instance();
    cf.clearAllCommands();

    string name = "command";

    cf.registerCommand( name, pdCalc::MakeCommandPtr<TestCommand>(name) );

    QVERIFY( cf.getNumberCommands() == 1 );
    QCOMPARE( cf.hasKey(name), true );

    pdCalc::CommandPtr clone = cf.allocateCommand(name);

    QVERIFY( cf.getNumberCommands() == 1 );
    QCOMPARE( cf.hasKey(name), true );
    QVERIFY( clone != nullptr );
    TestCommand* testCommand = dynamic_cast<TestCommand*>( clone.get() );
    QVERIFY( testCommand != nullptr );
    QCOMPARE( testCommand->getOptionalName(), name );

    clone = cf.allocateCommand("not present");

    QVERIFY( clone == nullptr );

    return;
}
