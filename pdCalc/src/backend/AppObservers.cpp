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

#include "AppObservers.h"
#include "utilities/Exception.h"
#include "ui/cli/Cli.h"
#include "Stack.h"
#include "utilities/UserInterface.h"
#include <vector>
#include <sstream>

using std::ostringstream;
using std::shared_ptr;
using std::dynamic_pointer_cast;
using std::vector;

namespace pdCalc {

CommandIssuedObserver::CommandIssuedObserver(CommandDispatcher& ce)
: Observer("CommandIssued")
, ce_(ce)
{ }

void CommandIssuedObserver::notifyImpl(std::shared_ptr<EventData> eventData)
{
    auto data = dynamic_pointer_cast<CommandData>(eventData);
    if(!data)
    {
        throw Exception("Could not convert CommandData to a command");
    }
    else
    {
        ce_.commandEntered( data->command() );
    }

    return;
}

StackUpdatedObserver::StackUpdatedObserver(UserInterface& ui)
: Observer("StackUpdated")
, ui_(ui)
{ }

void StackUpdatedObserver::notifyImpl(std::shared_ptr<EventData>)
{
    ui_.stackChanged();

    return;
}

}
