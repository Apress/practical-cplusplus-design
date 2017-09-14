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

#include <QtTest/QtTest>
#include "../utilitiesTest/PublisherObserverTest.h"
#include "../utilitiesTest/TokenizerTest.h"
#include "../pluginsTest/HyperbolicLnPluginTest.h"
#include "../guiTest/DisplayTest.h"
#include "../cliTest/CliTest.h"
#include "../backendTest/CommandDispatcherTest.h"
#include "../backendTest/CommandManagerTest.h"
#include "../backendTest/CommandRepositoryTest.h"
#include "../backendTest/CoreCommandsTest.h"
#include "../backendTest/PluginLoaderTest.h"
#include "../backendTest/StackTest.h"
#include "../backendTest/StoredProcedureTest.h"

#include <iostream>
#include <QStringList>
#include <unordered_map>

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    // I use QStringList instead of argv directly because the QStringList
    // argument to qExec is constant. API doc indicates that for the argv
    // version, it may alter argv making repeated calls to qExec unstable
    // with respect to the arguments.
    QStringList args;
    for(int i = 0; i < argc; ++i)
        args.append( QString{argv[i]} );

    std::unordered_map<std::string, int> passFail;

    PublisherObserverTest pot;
    passFail["PublisherObserverTest"] = QTest::qExec(&pot, args);

    TokenizerTest tt;
    passFail["TokenizerTest"] = QTest::qExec(&tt, args);

    HyperbolicLnPluginTest hpt;
    passFail["HyperbolicPluginTest"] = QTest::qExec(&hpt, args);

    DisplayTest dt;
    passFail["DisplayTest"] = QTest::qExec(&dt, args);

    CliTest ct;
    passFail["CliTest"] = QTest::qExec(&ct, args);

    CommandDispatcherTest cet;
    passFail["CommandDispatcherTest"] = QTest::qExec(&cet, args);

    CommandManagerTest cmt;
    passFail["CommandManagerTest"] = QTest::qExec(&cmt, args);

    CommandRepositoryTest crt;
    passFail["CommandRepositoryTest"] = QTest::qExec(&crt, args);

    CoreCommandsTest cct;
    passFail["CoreCommandsTest"] = QTest::qExec(&cct, args);

    PluginLoaderTest plt;
    passFail["PluginLoaderTest"] = QTest::qExec(&plt, args);

    StackTest st;
    passFail["StackTest"] = QTest::qExec(&st, args);

    StoredProcedureTest spt;
    passFail["StoredProcedureTest"] = QTest::qExec(&spt, args);

    cout << endl;
    int errors = 0;
    for(const auto& i : passFail)
    {
        errors += i.second;
        if(i.second != 0)
            cout << "Failed test in " << i.first << endl;
    }

    if(errors == 0)
        cout << "All tests passed" << endl;

    return 0;
}
