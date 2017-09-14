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

#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H

#include <string>
#include <memory>
#include "Command.h"
#include <set>

namespace pdCalc {

class UserInterface;

class CommandDispatcher
{
    class CommandDispatcherImpl;

public:
    explicit CommandDispatcher(UserInterface& ui);
    ~CommandDispatcher();

    void commandEntered(const std::string& command);

private:
    CommandDispatcher(const CommandDispatcher&) = delete;
    CommandDispatcher(CommandDispatcher&&) = delete;
    CommandDispatcher& operator=(const CommandDispatcher&) = delete;
    CommandDispatcher& operator=(CommandDispatcher&&) = delete;


    std::unique_ptr<CommandDispatcherImpl> pimpl_;
};

}

#endif
