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

#include "Publisher.h"
#include "Observer.h"
#include "Exception.h"

#include <sstream>
#include <unordered_map>
#include <set>

using std::string;
using std::vector;
using std::set;
using std::ostringstream;
using std::unique_ptr;
using std::shared_ptr;

namespace pdCalc {

class Publisher::PublisherImpl
{
    using ObserversList = std::unordered_map<string, unique_ptr<Observer>>;
    using Events = std::unordered_map<string, ObserversList>;
    
public:    
    PublisherImpl();
    ~PublisherImpl();    
    
    void attach(const string& eventName, unique_ptr<Observer> observer);
    unique_ptr<Observer> detach(const string& eventName, const string& observer);
    void notify(const string& eventName, shared_ptr<EventData> d) const;
    void registerEvent(const string& eventName);
    void registerEvents(const vector<string>& eventNames);
    set<string> listEvents() const;
    set<string> listEventObservers(const string& eventName) const;
    
    Events::const_iterator findCheckedEvent(const string& eventName) const;
    Events::iterator findCheckedEvent(const string& eventName);

private: 
    Events events_;
};

Publisher::PublisherImpl::PublisherImpl()
{ }

Publisher::PublisherImpl::~PublisherImpl()
{ 

}

Publisher::PublisherImpl::Events::const_iterator Publisher::PublisherImpl::findCheckedEvent(const string& eventName) const
{
    auto ev = events_.find(eventName);
    if( ev == events_.end() )
    {
        ostringstream oss;
        oss << "Publisher does not support event '" << eventName << "'";
        throw Exception{ oss.str() };
    }
    
    return ev;
}

Publisher::PublisherImpl::Events::iterator Publisher::PublisherImpl::findCheckedEvent(const string &eventName)
{
    auto ev = events_.find(eventName);
    if( ev == events_.end() )
    {
        ostringstream oss;
        oss << "Publisher does not support event '" << eventName << "'";
        throw Exception( oss.str() );
    }

    return ev;
}

void Publisher::PublisherImpl::attach(const string& eventName, unique_ptr<Observer> observer)
{ 
    auto ev = findCheckedEvent(eventName);   
    auto& obsList = ev->second;
    
    auto obs = obsList.find(observer->name());
    if( obs != obsList.end() )
        throw Exception("Observer already attached to publisher");   

    obsList.insert( std::make_pair(observer->name(), std::move(observer)) );
    
    return;
}
  
unique_ptr<Observer> Publisher::PublisherImpl::detach(const string& eventName, const string& observer)
{ 
    auto ev = findCheckedEvent(eventName);
    auto& obsList = ev->second;
    
    auto obs = obsList.find(observer);
    if( obs == obsList.end() )
        throw Exception("Cannot detach observer because observer not found");
    
    auto tmp = std::move(obs->second);
    obsList.erase(obs);
    
    return tmp;
}

void Publisher::PublisherImpl::notify(const string& eventName, shared_ptr<EventData> d) const
{
    auto ev = findCheckedEvent(eventName);
    const auto& obsList = ev->second;
    
    for(const auto& obs : obsList)
        obs.second->notify(d);
    
    return;
}

void Publisher::PublisherImpl::registerEvent(const string& eventName)
{ 
    auto i = events_.find(eventName);
    if( i != events_.end() )
        throw Exception{"Event already registered"};
    
    events_[eventName] = ObserversList{};
  
    return;
}

void Publisher::PublisherImpl::registerEvents(const vector<string>& eventNames)
{ 
    for(auto i : eventNames)  
        registerEvent(i);
  
    return;
}

set<string> Publisher::PublisherImpl::listEvents() const
{
    set<string> tmp;
    for(const auto& i : events_)
        tmp.insert(i.first);

    
    return tmp;
}
    
set<string> Publisher::PublisherImpl::listEventObservers(const string& eventName) const
{
    auto ev = findCheckedEvent(eventName);

    set<string> tmp;
    for(const auto& kvp : ev->second)
        tmp.insert(kvp.first);

    return tmp;
}

Publisher::Publisher()
{
    publisherImpl_ = std::make_unique<PublisherImpl>();
}

Publisher::~Publisher()
{
    // std::unique_ptr requires a definition of the destructor instead
    // of using the default because the destructor must appear in a scope
    // in which the complete definition of the template argument for
    // std::unique_ptr is known
}

 void Publisher::attach(const string& eventName, unique_ptr<Observer> observer)
 {
     publisherImpl_->attach(eventName, std::move(observer));
    
     return;
 }
 
unique_ptr<Observer> Publisher::detach(const string& eventName, const string& observer)
{
    return publisherImpl_->detach(eventName, observer);
}

void Publisher::raise(const string& eventName, std::shared_ptr<EventData> d) const
{
    publisherImpl_->notify(eventName, d);
    return;
}

void Publisher::registerEvent(const string& eventName)
{
    publisherImpl_->registerEvent(eventName);
    return;
}

void Publisher::registerEvents(const vector<string>& eventNames)
{
    publisherImpl_->registerEvents(eventNames);
    return;
}

set<string> Publisher::listEvents() const
{
    return publisherImpl_->listEvents();   
}
    
set<string> Publisher::listEventObservers(const string& eventName) const
{
    return publisherImpl_->listEventObservers(eventName);
}

EventData::~EventData()
{

}

}


