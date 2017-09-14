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

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>
#include "Publisher.h"

namespace pdCalc {

class CommandData : public EventData
{
public:
    CommandData(const std::string& s) : command_(s) { }
    const std::string& command() const { return command_; }

private:
    std::string command_;
};

class UserInterface : protected Publisher
{
public:
    UserInterface() { registerEvent(CommandEntered); }
    virtual ~UserInterface() { }

    // post a message to the user
    virtual void postMessage(const std::string& m) = 0;

    // notifies the interface that the stack has changed
    virtual void stackChanged() = 0;

    using Publisher::attach;
    using Publisher::detach;

    // defines the event this publisher can raise
    // note that the string is defined in main.cpp of the application since
    // class UserInterface has no implementation file (in test driver for same
    // reason)
    static const std::string CommandEntered;
};

}
#endif
