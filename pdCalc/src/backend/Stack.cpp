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

#include "Stack.h"
#include "utilities/Exception.h"
#include <deque>

using std::vector;
using std::string;

namespace pdCalc {

const string Stack::StackChanged = "stackChanged";
const string Stack::StackError = "error";

const char* StackEventData::Message(StackEventData::ErrorConditions ec)
{
    switch(ec)
    {
    case ErrorConditions::Empty: return "Attempting to pop empty stack";
    case ErrorConditions::TooFewArguments: return "Need at least two stack elements to swap top";
    default: return "Unknown error";
    };
}

const char* StackEventData::message() const
{
    return Message(err_);
}

class Stack::StackImpl
{
public:
    explicit StackImpl(const Stack&);
    void push(double d, bool suppressChangeEvent);
    double pop(bool suppressChangeEvent);
    void swapTop();
    vector<double> getElements(size_t n) const;
    void getElements(size_t n, vector<double>& v) const;
    size_t size() const { return stack_.size(); }
    void clear();
    double top() const;

private:
    const Stack& parent_; // for raising events
    std::deque<double> stack_;
};

Stack::StackImpl::StackImpl(const Stack& s)
: parent_(s)
{

}

void Stack::StackImpl::push(double d, bool suppressChangeEvent)
{
    stack_.push_back(d);
    if(!suppressChangeEvent) parent_.raise(Stack::StackChanged, nullptr);

    return;
}

double Stack::StackImpl::pop(bool suppressChangeEvent)
{
    if( stack_.empty() )
    {
        parent_.raise(Stack::StackError,
            std::make_shared<StackEventData>(StackEventData::ErrorConditions::Empty));

        throw Exception{StackEventData::Message(StackEventData::ErrorConditions::Empty)};
    }
    else
    {
        auto val = stack_.back();
        stack_.pop_back();
        if(!suppressChangeEvent) parent_.raise(Stack::StackChanged, nullptr);
        return val;
    }
}

void Stack::StackImpl::swapTop()
{
    if( stack_.size() < 2 )
    {
        parent_.raise(Stack::StackError,
            std::make_shared<StackEventData>(StackEventData::ErrorConditions::TooFewArguments));

        throw Exception{StackEventData::Message(StackEventData::ErrorConditions::TooFewArguments)};
    }
    else
    {
        auto first = stack_.back();
        stack_.pop_back();
        auto second = stack_.back();
        stack_.pop_back();
        stack_.push_back(first);
        stack_.push_back(second);

        parent_.raise(Stack::StackChanged, nullptr);
    }

    return;
}

vector<double> Stack::StackImpl::getElements(size_t n) const
{
    vector<double> v;
    getElements(n, v);
    return v;
}

void Stack::StackImpl::getElements(size_t n, vector<double>& v) const
{
    // if n is > stack's size, just return size of stack
    if(n > stack_.size()) n = stack_.size();

    v.insert(v.end(), stack_.rbegin(), stack_.rbegin() + n);

    return;
}

void Stack::StackImpl::clear()
{
    stack_.clear();

    parent_.raise(Stack::StackChanged, nullptr);

    return;
}

double Stack::StackImpl::top() const
{
    return stack_.back();
}

Stack& Stack::Instance()
{
    static Stack instance;
    return instance;
}

void Stack::push(double d, bool suppressChangeEvent)
{
    pimpl_->push(d, suppressChangeEvent);
    return;
}

double Stack::pop(bool suppressChangeEvent)
{
    return pimpl_->pop(suppressChangeEvent);
}

void Stack::swapTop()
{
    pimpl_->swapTop();
    return;
}

vector<double> Stack::getElements(size_t n) const
{
    return pimpl_->getElements(n);
}

void Stack::getElements(size_t n, vector<double>& v) const
{
    pimpl_->getElements(n, v);
    return;
}

size_t Stack::size() const
{
    return pimpl_->size();
}

void Stack::clear() const
{
    pimpl_->clear();
    return;
}

Stack::Stack()
{
    pimpl_ = std::make_unique<StackImpl>(*this);
    registerEvent(StackChanged);
    registerEvent(StackError);
}

Stack::~Stack()
{

}

}

