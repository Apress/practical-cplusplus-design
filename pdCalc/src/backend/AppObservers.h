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

#ifndef APP_OBSERVERS_H
#define APP_OBSERVERS_H

#include "utilities/Observer.h"
#include "CommandDispatcher.h"

namespace pdCalc {

class UserInterface;

class CommandIssuedObserver : public Observer
{
public:
    explicit CommandIssuedObserver(CommandDispatcher& ce);

private:
    void notifyImpl(std::shared_ptr<EventData>) override;

    CommandDispatcher& ce_;
};

class StackUpdatedObserver : public Observer
{
public:
    explicit StackUpdatedObserver(UserInterface& ui);

private:
    void notifyImpl(std::shared_ptr<EventData>) override;

    UserInterface& ui_;
};


}
#endif
