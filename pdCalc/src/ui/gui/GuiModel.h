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

#ifndef GUI_MODEL_H
#define GUI_MODEL_H

#include <QObject>
#include <memory>
#include <vector>
#include <string>
#include <QDoubleValidator>

namespace pdCalc {

class GuiModel : public QObject
{
    class GuiModelImpl;
    Q_OBJECT
public:
    enum class ShiftState { Unshifted, Shifted };

    struct State
    {
        State();

        std::vector<double> curStack;
        std::string curInput;
        ShiftState shiftState;
        QValidator::State curInputValidity;
    };

    explicit GuiModel(QObject* parent = nullptr);
    ~GuiModel();

    void stackChanged(const std::vector<double>& v);

    const State& getState() const;

    // exposed externally for testing only
    bool inputEmpty() const;
    void clearInput();

public slots:
    // called to toggle the calculator's shift state
    void onShift();

    // call this slot when a new character of input is available
    void onCharacterEntered(char c);

    // call this slot when enter is pressed. Does nothing if input is invalid else enters number on stack
    // and clears input
    void onEnter();

    // call this slot to backspace during input entry
    void onBackspace();

    // call this slot when plus/minus is entered
    void onPlusMinus();

    // called when commands are entered
    void onCommandEntered(std::string primaryCmd, std::string secondaryCmd);

signals:
    void modelChanged();
    void commandEntered(std::string s);
    void errorDetected(std::string s);

private:
    std::unique_ptr<GuiModelImpl> pimpl_;
};

}
#endif
