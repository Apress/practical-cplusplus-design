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

#include "HyperbolicLnPluginTest.h"
#include "backend/PluginLoader.h"
#include "utilities/UserInterface.h"
#include "plugins/hyperbolicLnPlugin/HyperbolicLnPlugin.h"
#include "backend/Stack.h"
#include "utilities/Exception.h"
#include "backend/Command.h"
#include <cmath>
#include <string>
#include <map>
#include <vector>

using std::map;
using std::string;
using std::vector;
using pdCalc::Plugin;

class TestInterface : public pdCalc::UserInterface
{
public:
    TestInterface() { }
    void postMessage(const string&) override {  }
    void stackChanged() override { }
};

namespace {

// this function needed because of how QCOMPARE works
void testStack()
{
    QVERIFY( pdCalc::Stack::Instance().size() == 0 );
}

}

pdCalc::Stack& HyperbolicLnPluginTest::getCheckedStack()
{
    pdCalc::Stack::Instance().clear();
    testStack();
    return pdCalc::Stack::Instance();
}

void HyperbolicLnPluginTest::testPreconditions(pdCalc::Command* command, double number)
{
    pdCalc::Stack& stack = getCheckedStack();

    try
    {
        command->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    stack.push(number);

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

void HyperbolicLnPluginTest::testCommand(pdCalc::Command* command, double top, double result)
{
    pdCalc::Stack& stack = getCheckedStack();

    stack.push(top);

    QVERIFY( stack.size() == 1 );

    command->execute();

    QVERIFY( stack.size() == 1 );
    vector<double> stackTop{ stack.getElements(1) };
    QVERIFY( std::abs(stackTop[0] - result) < 1e-10 );

    command->undo();

    QVERIFY( stack.size() == 1 );
    stackTop = stack.getElements(1);
    QCOMPARE( stackTop[0], top);

    return;
}

void HyperbolicLnPluginTest::testHyperbolicLnPlugin()
{
    TestInterface ui;
    pdCalc::PluginLoader loader;

    string pluginFile{PLUGIN_TEST_DIR};
    pluginFile += "/../backendTest/";
    pluginFile += PLUGIN_TEST_FILE;
    loader.loadPlugins(ui, pluginFile);

    vector<const Plugin*> plugins{ loader.getPlugins() };

    QVERIFY(plugins.size() == 1);

    const Plugin* p = plugins[0];
    Plugin::PluginDescriptor descriptor = p->getPluginDescriptor();

    QCOMPARE(descriptor.nCommands, 8);

    map<string, pdCalc::Command*> commands;
    for(int i = 0; i < descriptor.nCommands; ++i)
        commands[descriptor.commandNames[i]] = descriptor.commands[i];

    testPreconditions( commands.find("sinh")->second, 1.0 );
    double top = 3.45;
    testCommand( commands.find("sinh")->second, top, std::sinh(top));

    testPreconditions( commands.find("cosh")->second, 1.0 );
    top = -12.3;
    testCommand( commands.find("cosh")->second, top, std::cosh(top) );

    testPreconditions( commands.find("tanh")->second, 1.0 );
    top = -1.34;
    testCommand( commands.find("tanh")->second, top, std::tanh(top));

    testPreconditions( commands.find("arccosh")->second, 1.3 );
    try
    {
        pdCalc::Stack::Instance().push(0.2);
        commands.find("arccosh")->second->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    top = 1.34;
    testCommand( commands.find("arccosh")->second, top, std::log(top + std::sqrt(top*top - 1.)));

    testPreconditions( commands.find("arctanh")->second, 0.5 );
    try
    {
        pdCalc::Stack::Instance().push(1.2);
        commands.find("arctanh")->second->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    try
    {
        pdCalc::Stack::Instance().push(-1.2);
        commands.find("arctanh")->second->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    top = 0.3;
    testCommand( commands.find("arctanh")->second, top, 0.5 * std::log( (1.0 + top) / (1.0 - top) ));

    testPreconditions( commands.find("exp")->second, 1.0 );
    top = -1.34;
    testCommand( commands.find("exp")->second, top, std::exp(top));

    testPreconditions( commands.find("ln")->second, 1.3 );
    try
    {
        pdCalc::Stack::Instance().push(-0.2);
        commands.find("ln")->second->execute();
        QVERIFY(false);
    }
    catch(pdCalc::Exception&)
    {
        QVERIFY(true);
    }

    top = 1.34;
    testCommand( commands.find("ln")->second, top, std::log(top) );
    return;
}
