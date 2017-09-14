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

#include "CommandRepository.h"
#include "Command.h"
#include <unordered_map>
#include "../utilities/Exception.h"
#include <sstream>

using std::string;
using std::unordered_map;
using std::set;

namespace pdCalc {

class CommandRepository::CommandRepositoryImpl
{
public:
    CommandRepositoryImpl();
    void registerCommand(const string& name, CommandPtr c);
    CommandPtr deregisterCommand(const string& name);

    size_t getNumberCommands() const { return repository_.size(); }
    CommandPtr allocateCommand(const string& name) const;

    bool hasKey(const string& s) const;
    set<string> getAllCommandNames() const;

    void printHelp(const std::string& command, std::ostream& os);

    void clearAllCommands();

private:
    using Repository = unordered_map<string, CommandPtr>;
    Repository repository_;
};

CommandRepository::CommandRepositoryImpl::CommandRepositoryImpl()
{
}

bool CommandRepository::CommandRepositoryImpl::hasKey(const string& s) const
{
    return repository_.find(s) != repository_.end();
}

set<string> CommandRepository::CommandRepositoryImpl::getAllCommandNames() const
{
    set<string> tmp;

    for(auto i = repository_.begin(); i != repository_.end(); ++i)
        tmp.insert(i->first);

    return tmp;
}

void CommandRepository::CommandRepositoryImpl::printHelp(const std::string& command, std::ostream& os)
{
    auto it = repository_.find(command);
    if(it != repository_.end())
        os << command << ": " << it->second->helpMessage();
    else
        os << command << ": no help entry found";

    return;
}

void CommandRepository::CommandRepositoryImpl::clearAllCommands()
{
    repository_.clear();
    return;
}

void CommandRepository::CommandRepositoryImpl::registerCommand(const string& name, CommandPtr c)
{
    if( hasKey(name) )
    {
        std::ostringstream oss;
        oss << "Command " << name << " already registered";
        throw Exception{ oss.str() };
    }
    else
        repository_.emplace( name, std::move(c) );

    return;
}

CommandPtr CommandRepository::CommandRepositoryImpl::deregisterCommand(const string& name)
{
    if( hasKey(name) )
    {
        auto i = repository_.find(name);
        auto tmp = MakeCommandPtr( i->second.release() );
        repository_.erase(i);
        return tmp;
    }
    else return MakeCommandPtr(nullptr);
}

CommandPtr CommandRepository::CommandRepositoryImpl::allocateCommand(const string &name) const
{
    if( hasKey(name) )
    {
        const auto& command = repository_.find(name)->second;
        return MakeCommandPtr( command->clone() );
    }
    else return MakeCommandPtr(nullptr);
}

CommandRepository::CommandRepository()
: pimpl_{ new CommandRepositoryImpl }
{
}

CommandRepository::~CommandRepository()
{ }

CommandRepository& CommandRepository::Instance()
{
    static CommandRepository instance;
    return instance;
}

void CommandRepository::registerCommand(const string& name, CommandPtr c)
{
    pimpl_->registerCommand( name, std::move(c) );

    return;
}
CommandPtr CommandRepository::deregisterCommand(const string& name)
{
    return pimpl_->deregisterCommand(name);
}

size_t CommandRepository::getNumberCommands() const
{
    return pimpl_->getNumberCommands();
}

CommandPtr CommandRepository::allocateCommand(const string& name) const
{
    return pimpl_->allocateCommand(name);
}

bool CommandRepository::hasKey(const string& s) const
{
    return pimpl_->hasKey(s);
}

set<string> CommandRepository::getAllCommandNames() const
{
    return pimpl_->getAllCommandNames();
}

void CommandRepository::printHelp(const std::string& command, std::ostream& os) const
{
    pimpl_->printHelp(command, os);
    return;
}

void CommandRepository::clearAllCommands()
{
    pimpl_->clearAllCommands();
    return;
}

}
