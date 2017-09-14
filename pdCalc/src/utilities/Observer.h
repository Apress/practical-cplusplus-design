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

#ifndef OBSERVER_H
#define OBSERVER_H

// The Observer class serves as the base class for all observer objects. The Observer class 
// is a mediator between the Publisher and the actual class that performs the observation. 
// A concrete observer should inherit from observer and contain a pointer to the actual observing
// object. This decouples the observing object from knowing about publishers. Note that publishers
// call Observer's notify, which must be implemented in the concrete observer to respond to event
// notifications.

// Note that the semantics of Publisher to to own the Observer uniquely (enforced by std::unique_ptr)

#include <string>
#include <memory>

namespace pdCalc {

class EventData;

class Observer
{ 
public:
    explicit Observer(const std::string& name);
    virtual ~Observer();

    void notify(std::shared_ptr<EventData>);

    const std::string name() const { return observerName_; }

private:
    virtual void notifyImpl(std::shared_ptr<EventData>) = 0;

    std::string observerName_;
};

}

#endif
