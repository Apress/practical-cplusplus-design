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

#include "PluginLoaderTest.h"
#include "backend/PluginLoader.h"
#include "utilities/UserInterface.h"
#include "backend/Plugin.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::vector;
using std::string;

class TestInterface : public pdCalc::UserInterface
{
public:
    TestInterface() { }
    void postMessage(const string&) override {  }
    void stackChanged() override { }
};

void PluginLoaderTest::testLoading()
{
    TestInterface ui;
    pdCalc::PluginLoader loader;

    std::ostringstream pluginFile;
    pluginFile << BACKEND_TEST_DIR << "/" << PLUGIN_TEST_FILE;
    loader.loadPlugins(ui, pluginFile.str());
    vector<const pdCalc::Plugin*> plugins { loader.getPlugins() };

    QVERIFY(plugins.size() == 1);

    return;
}

void PluginLoaderTest::testNoPluginFile()
{
    TestInterface ui;
    pdCalc::PluginLoader loader;

    string pluginFile = "fileDoesNotExist";
    loader.loadPlugins(ui, pluginFile);
    vector<const pdCalc::Plugin*> plugins { loader.getPlugins() };

    QVERIFY(plugins.size() == 0);

    return;
}
