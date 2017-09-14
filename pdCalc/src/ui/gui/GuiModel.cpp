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

#include "GuiModel.h"
#include <string>
#include <utility>
#include <iostream>

using std::string;
using std::vector;
using std::pair;

namespace pdCalc {

class GuiModel::GuiModelImpl
{
public:
    explicit GuiModelImpl(GuiModel& p);
    void onShift();
    void stackChanged(const std::vector<double>& v);
    const GuiModel::State& getState() const { return state_; }

    void onCharacterEntered(char c);
    void onEnter();
    void onBackspace();
    void handlePlusMinus();
    void onCommandEntered(string primaryCmd, string shiftCmd);

    bool inputEmpty() const { return state_.curInput.empty(); }

    void clearInput();

private:
    QValidator::State validate(const string& s);
    pair<string, string> split(const string&, string::size_type pos);

    GuiModel& parent_;
    GuiModel::State state_;
    QDoubleValidator validator_;
};

GuiModel::GuiModelImpl::GuiModelImpl(GuiModel& p)
: parent_{p}
, state_{}
{
    validator_.setNotation(QDoubleValidator::ScientificNotation);
}

void GuiModel::GuiModelImpl::onShift()
{
    if(state_.shiftState == ShiftState::Shifted) state_.shiftState = ShiftState::Unshifted;
    else state_.shiftState = ShiftState::Shifted;

    emit parent_.modelChanged();
}

void GuiModel::GuiModelImpl::stackChanged(const vector<double>& v)
{
    state_.curStack = v;

    emit parent_.modelChanged();
}

void GuiModel::GuiModelImpl::onCharacterEntered(char c)
{
    string::size_type pos{ state_.curInput.find('e') };

    // bug in QDoubleValidator that lets '.' in an exponent, so self validate
    if(pos != string::npos && c == '.') return;

    // bug in QDoubleValidator that lets e be the first letter, so self validate
    if(state_.curInput.empty() && c == 'e') return;

    auto state = validate(state_.curInput + c);

    if(state != QValidator::Invalid)
    {
        state_.curInputValidity = state;
        state_.curInput += c;
        emit parent_.modelChanged();
    }

    return;
}

QValidator::State GuiModel::GuiModelImpl::validate(const string& s)
{
    int dummy;
    QString c{ QString::fromStdString(s) };
    return validator_.validate(c, dummy);
}

void GuiModel::GuiModelImpl::onEnter()
{
    // enter duplicates the stack if the input is empty
    if(state_.curInput.empty())
    {
        onCommandEntered("dup", "swap");
    }
    else if(state_.curInputValidity == QValidator::Acceptable)
    {
        string tmp{ state_.curInput };
        clearInput();
        onCommandEntered(tmp, "");
    }
    return;
}

void GuiModel::GuiModelImpl::clearInput()
{
    // just clear the current input...someone else will call stackChanged()
    state_.curInput.clear();
    emit parent_.modelChanged();
    state_.curInputValidity = QValidator::Intermediate;

    return;
}

void GuiModel::GuiModelImpl::onBackspace()
{
    if(state_.curInput.size() > 0)
    {
        state_.curInput = state_.curInput.substr(0, state_.curInput.size() - 1);
        state_.curInputValidity = validate(state_.curInput);
        emit parent_.modelChanged();
    }
    else
    {
        onCommandEntered("drop", "clear");
    }

    return;
}

void GuiModel::GuiModelImpl::handlePlusMinus()
{
     // if no input, treat as negate command
    if(state_.curInput.empty())
    {
        onCommandEntered("neg", "");
        return;
    }

    auto pos = state_.curInput.find('e');

    // +/- changes sign for whole number if before exponent
    // or exponent if after e
    if(pos == string::npos)
    {
        if(!state_.curInput.empty() && state_.curInput[0] == '-')
            state_.curInput = state_.curInput.substr(1, state_.curInput.length() - 1);
        else
           state_.curInput = '-' + state_.curInput;
    }
    else
    {
        auto halves = split(state_.curInput, pos);
        if(state_.curInput.size() > pos && state_.curInput[pos + 1] == '-')
        {
            state_.curInput = halves.first + halves.second.substr(1, halves.second.length() - 1);
        }
        else
        {
            state_.curInput = halves.first + '-' + halves.second;
        }
    }

    auto state = validate(state_.curInput);

    if(state != QValidator::Invalid)
    {
        state_.curInputValidity = state;
        emit parent_.modelChanged();
    }

    return;
}

pair<string, string> GuiModel::GuiModelImpl::split(const string& s, string::size_type pos)
{
    auto left = s.substr(0, pos + 1);
    auto right = s.substr(pos + 1, s.size() - pos);
    return std::make_pair(left, right);
}

void GuiModel::GuiModelImpl::onCommandEntered(string primaryCmd, string shiftCmd)
{
    // need to choose either primary or shift command
    string cmd;
    if(shiftCmd.empty())
    {
        cmd = primaryCmd;
    }
    else
    {
        if(state_.shiftState == ShiftState::Shifted) cmd = shiftCmd;
        else cmd = primaryCmd;
    }

    // a command should pull the number off the entry line and enter it on the stack before
    // executing a command, but only if it is valid
    if( !state_.curInput.empty() )
    {
        if(state_.curInputValidity == QValidator::Acceptable)
        {
            onEnter();
        }
        else
        {
            emit parent_.errorDetected("Invalid input");
            return;
        }
    }

    state_.shiftState = ShiftState::Unshifted;
    emit parent_.modelChanged();

    emit parent_.commandEntered(cmd);

    return;
}

GuiModel::GuiModel(QObject* parent)
: QObject{parent}
{
    pimpl_ = std::make_unique<GuiModelImpl>(*this);
}

GuiModel::~GuiModel()
{

}

void GuiModel::onShift()
{
    pimpl_->onShift();
    return;
}

void GuiModel::onCharacterEntered(char c)
{
    pimpl_->onCharacterEntered(c);

    return;
}

void GuiModel::onEnter()
{
    pimpl_->onEnter();

    return;
}

void GuiModel::onBackspace()
{
    pimpl_->onBackspace();

    return;
}

void GuiModel::stackChanged(const vector<double>& v)
{
    pimpl_->stackChanged(v);

    return;
}

const GuiModel::State& GuiModel::getState() const
{
    return pimpl_->getState();
}

bool GuiModel::inputEmpty() const
{
    return pimpl_->inputEmpty();
}

void GuiModel::clearInput()
{
    pimpl_->clearInput();

    return;
}

void GuiModel::onPlusMinus()
{
    pimpl_->handlePlusMinus();

    return;
}

void GuiModel::onCommandEntered(string primaryCmd, string secondaryCmd)
{
    pimpl_->onCommandEntered(primaryCmd, secondaryCmd);

    return;
}

GuiModel::State::State()
: curStack{}
, curInput{""}
, shiftState{ShiftState::Unshifted}
, curInputValidity{QValidator::Intermediate}
{ }

}
