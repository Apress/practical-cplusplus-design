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

#include <QApplication>
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include "ui/cli/Cli.h"
#include "backend/AppObservers.h"
#include "backend/CommandDispatcher.h"
#include <memory>
#include "backend/Stack.h"
#include "utilities/Exception.h"
#include "backend/CoreCommands.h"
#include "ui/gui/MainWindow.h"
#include "backend/PluginLoader.h"
#include <vector>
#include "backend/Plugin.h"
#include "backend/CommandRepository.h"
#include <set>
#include <sstream>

using std::set;
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::unique_ptr;
using std::make_unique;
using pdCalc::Cli;
using pdCalc::Stack;
using pdCalc::CommandDispatcher;
using pdCalc::Observer;
using std::ostream;
using std::istream;
using pdCalc::Plugin;
using std::ostringstream;

using namespace pdCalc;

namespace {

class BatchIo
{
public:
    BatchIo(const string& in, const string& out);
    ~BatchIo();

    istream& in() { return *in_; }
    ostream& out() { return *out_; }

private:
    istream* in_;
    ostream* out_;
    bool hasOutFile_;
};

BatchIo::BatchIo(const string& in, const string& out)
: in_(nullptr)
, out_(nullptr)
, hasOutFile_(false)
{
    in_ = new std::ifstream{in.c_str()};
    if(!(*in_))
    {
        cerr << "Could not open " << in << " for reading." << endl;
        exit(1);
    }

    if(out.empty())
    {
        out_ = &cout;
    }
    else
    {
        out_ = new std::ofstream{out.c_str()};
        if(!(*out_))
        {
            cerr << "Could not open " << out << " for writing. Reverting to cout." << endl;
            delete out_;
            out_ = &cout;
        }
        else
        {
            hasOutFile_ = true;
        }
    }
}

BatchIo::~BatchIo()
{
    delete in_;
    if(hasOutFile_) delete out_;
}

void usage()
{
    cout << "\n"
         << "pdCalc v. 1.0\n"
         << "by Adam B. Singer, 2011\n"
         << "\n"
         << "\tpdCalc [options]\n"
         << "\t--gui, -g: graphical user interface\n"
         << "\t--cli, -c: command line interface\n"
         << "\t--batch <in> [out], -b <in> [out]: batch interface (out optional)\n"
         << endl;
       
    exit(0);
}

void setupUi(UserInterface& ui, CommandDispatcher& ce)
{
    RegisterCoreCommands(ui);

    ui.attach(UserInterface::CommandEntered, make_unique<CommandIssuedObserver>( ce ) );

    Stack::Instance().attach(Stack::StackChanged, make_unique<StackUpdatedObserver>( ui ) );
}

void registerCommand(UserInterface& ui, const string& label, CommandPtr c)
{
    try
    {
        CommandRepository::Instance().registerCommand(label, std::move(c));
    }
    catch(Exception& e)
    {
        ui.postMessage( e.what() );
    }

    return;
}

set<string> setupPlugins(UserInterface& ui, PluginLoader& loader)
{
    // for now, I don't want to allow the plugin file to be a command
    // line option, so I simply code the name of the searched plugin file
    auto pluginFile = "plugins.pdp";
    loader.loadPlugins(ui, pluginFile);
    auto plugins = loader.getPlugins();

    set<string> injectedCommands;
    for(auto p : plugins)
    {
        auto apiVersion = p->apiVersion();
        if(apiVersion.major != 1 || apiVersion.minor != 0)
        {
            ui.postMessage("Plugin API version is incompatible. Need v. 1.0.");
            continue;
        }
        auto descriptor = p->getPluginDescriptor();
        for(int i = 0; i < descriptor.nCommands; ++i)
        {
            registerCommand(ui, descriptor.commandNames[i], MakeCommandPtr(descriptor.commands[i]->clone()) );
            injectedCommands.insert(descriptor.commandNames[i]);
        }

        // if gui, setup buttons
        auto mw = dynamic_cast<MainWindow*>(&ui);
        if(mw)
        {
            auto allCommands = CommandRepository::Instance().getAllCommandNames();
            auto buttonDescriptor = p->getPluginButtonDescriptor();
            if(buttonDescriptor)
            {
                for(int i = 0; i < buttonDescriptor->nButtons; ++i)
                {
                    auto b = *buttonDescriptor;
                    if( allCommands.find(b.primaryCmd[i]) == allCommands.end() )
                    {
                        ostringstream oss;
                        oss << "Error adding button " << b.dispPrimaryCmd[i];
                        mw->postMessage( oss.str() );
                    }
                    else if( allCommands.find(b.shftCmd[i]) == allCommands.end() )
                    {
                        ostringstream oss;
                        oss << "Error adding button " << b.dispShftCmd[i];
                        mw->postMessage( oss.str() );

                    }
                    else
                    {
                        mw->addCommandButton(b.dispPrimaryCmd[i], b.primaryCmd[i], b.dispShftCmd[i], b.shftCmd[i]);
                    }
                }
            }
        }
    }

    return injectedCommands;
}

void runGui(int argc, char* argv[])
try
{
    QApplication app{argc, argv};

    MainWindow gui{argc, argv};

    // PluginLoader must be before CommandDispatcher so that memory on Command stack
    // is released before plugins are freed
    PluginLoader loader;
    CommandDispatcher ce{gui};

    setupUi(gui, ce);
    set<string> injectedCommands{setupPlugins(gui, loader)};

    gui.setupFinalButtons();
    gui.show();
    gui.fixSize();

    app.exec();

    for(auto i : injectedCommands)
        CommandRepository::Instance().deregisterCommand(i);

    return;
}
catch(Exception& e)
{
    cerr << "GUI terminated with exception:\n"
         << e.what() << endl;
}

void runBatch(const string& in, const string& out)
{
    BatchIo io{in, out};

    Cli cli{ io.in(), io.out() };

    // PluginLoader must be before CommandDispatcher so that memory on Command stack
    // is released before plugins are freed
    PluginLoader loader;
    CommandDispatcher ce{cli};

    setupUi(cli, ce);
    set<string> injectedCommands{setupPlugins(cli, loader)};

    cli.execute(true, true);

    for(auto i : injectedCommands)
        CommandRepository::Instance().deregisterCommand(i);

    return;
}

void runCli()
try
{
    Cli cli{cin, cout};

    // PluginLoader must be before CommandDispatcher so that memory on Command stack
    // is released before plugins are freed
    PluginLoader loader;
    CommandDispatcher ce{cli};

    setupUi(cli, ce);
    set<string> injectedCommands{setupPlugins(cli, loader)};

    cli.execute();

    for(auto i : injectedCommands)
        CommandRepository::Instance().deregisterCommand(i);

    return;
}  
catch(Exception& e)
{
    cerr << "pdCalc terminated with the following message:\n"
         << e.what() << endl;
}
catch(...)
{
    cerr << "pdCalc terminated with an unknown error." << endl;
}
  
}

int main(int argc, char* argv[])
{
    if(argc == 1) runGui(argc, argv); // default
    else if(argc == 2)
    {
        string arg(argv[1]);
        if(arg == "--gui" || arg == "-g") runGui(argc, argv);
        else if(arg == "--cli" || arg == "-c") runCli();
        else usage();
    }
    else if(argc == 3 || argc == 4)
    {
        string cmd(argv[1]);
        string file(argv[2]);
        if(cmd == "--batch" || cmd == "-b") runBatch(file, (argc == 4 ? argv[3] : "") );
        else usage();
    }
    else usage();

    return 0;
}
