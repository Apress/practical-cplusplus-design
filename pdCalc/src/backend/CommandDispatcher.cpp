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

#include "CommandDispatcher.h"
#include "CommandRepository.h"
#include "CommandManager.h"
#include "CoreCommands.h"
#include "utilities/Exception.h"
#include <sstream>
#include <regex>
#include <cassert>
#include <algorithm>
#include "utilities/UserInterface.h"
#include <fstream>
#include "utilities/Tokenizer.h"
#include "StoredProcedure.h"

using std::string;
using std::ostringstream;
using std::unique_ptr;
using std::set;
using std::istringstream;

namespace pdCalc {

class CommandDispatcher::CommandDispatcherImpl
{
public:
    explicit CommandDispatcherImpl(UserInterface& ui);

    void executeCommand(const string& command);


private:
    bool isNum(const string&, double& d);
    void handleCommand(CommandPtr command);
    void printHelp() const;

    CommandManager manager_;
    UserInterface& ui_;
};

CommandDispatcher::CommandDispatcherImpl::CommandDispatcherImpl(UserInterface& ui)
: ui_(ui)
{ }

void CommandDispatcher::CommandDispatcherImpl::executeCommand(const string& command)
{
    // entry of a number simply goes onto the the stack
    double d;
    if( isNum(command, d) )
        manager_.executeCommand(MakeCommandPtr<EnterNumber>(d));
    else if(command == "undo")
        manager_.undo();
    else if(command == "redo")
        manager_.redo();
    else if(command == "help")
        printHelp();
    else if(command.size() > 6 && command.substr(0, 5) == "proc:")
    {
        auto filename = command.substr(5, command.size() - 5);
        handleCommand( MakeCommandPtr<StoredProcedure>(ui_, filename) );
    }
    else
    {
        auto c = CommandRepository::Instance().allocateCommand(command);
        if(!c)
        {
            ostringstream oss;
            oss << "Command " << command << " is not a known command";
            ui_.postMessage( oss.str() );
        }
        else handleCommand( std::move(c) );
    }

    return;
}

void CommandDispatcher::CommandDispatcherImpl::handleCommand(CommandPtr c)
{
    try
    {
        manager_.executeCommand( std::move(c) );
    }
    catch(Exception& e)
    {
        ui_.postMessage( e.what() );
    }

    return;
}

void CommandDispatcher::CommandDispatcherImpl::printHelp() const
{
    ostringstream oss;
    set<string> allCommands = CommandRepository::Instance().getAllCommandNames();
    oss << "\n";
    oss << "undo: undo last operation\n"
        << "redo: redo last operation\n";

    for(auto i : allCommands)
    {
        CommandRepository::Instance().printHelp(i, oss);
        oss << "\n";
    }

    ui_.postMessage( oss.str() );

}

// uses a C++11 regular expression to check if this is a valid double number
// if so, converts it into one and returns it
bool CommandDispatcher::CommandDispatcherImpl::isNum(const string& s, double& d)
{
     if(s == "+" || s == "-") return false;

     std::regex dpRegex("((\\+|-)?[[:digit:]]*)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?");
     bool isNumber{ std::regex_match(s, dpRegex) };

     if(isNumber)
     {
         d = std::stod(s);
     }

     return isNumber;
}

void CommandDispatcher::commandEntered(const std::string& command)
{
    pimpl_->executeCommand(command);

    return;
}

CommandDispatcher::CommandDispatcher(UserInterface& ui)
{
    pimpl_ = std::make_unique<CommandDispatcherImpl>(ui);
}

CommandDispatcher::~CommandDispatcher()
{ }

}
