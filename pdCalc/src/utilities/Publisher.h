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

#ifndef PUBLISHER_H
#define PUBLISHER_H

// The Publisher class is a class capable of receiving observers. Note that it is assumed
// that a real publisher may publish multiple separate events. These are stored by string
// name in a table. Since each event may have multiple observers, the table stores
// a collection of observers.

// Important: Publishers own the memory for their observers (enforced by std::unique_ptr)

#include <string>
#include <vector>
#include <memory>
#include <set>

namespace pdCalc {

class Observer;

class EventData
{
public:
    virtual ~EventData();
};

class Publisher
{
    class PublisherImpl;
public:
    Publisher();

    void attach(const std::string& eventName, std::unique_ptr<Observer> observer);
    std::unique_ptr<Observer> detach(const std::string& eventName, const std::string& observerName);

    std::set<std::string> listEvents() const;
    std::set<std::string> listEventObservers(const std::string& eventName) const;

protected:
    ~Publisher();

    void raise(const std::string& eventName, std::shared_ptr<EventData>) const;

    void registerEvent(const std::string& eventName);
    void registerEvents(const std::vector<std::string>& eventNames);
    
private:    
    std::unique_ptr<PublisherImpl> publisherImpl_;
};

}

#endif
