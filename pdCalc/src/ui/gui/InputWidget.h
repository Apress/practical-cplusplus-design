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

#ifndef INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include <QWidget>
#include <string>
#include <memory>

class QGridLayout;

namespace pdCalc {

class InputWidget : public QWidget
{
    class InputWidgetImpl;
    Q_OBJECT
public:
    explicit InputWidget(QWidget *parent = nullptr);
    ~InputWidget();

    void addCommandButton(const std::string& dispPrimaryCmd, const std::string& primaryCmd,
        const std::string& dispShftCmd, const std::string& shftCmd);

    void setupFinalButtons();

signals:
    void characterEntered(char c);
    void enterPressed();
    void backspacePressed();
    void plusMinusPressed();
    void commandEntered(std::string, std::string);
    void shiftPressed();
    void procedurePressed();

private:
    std::unique_ptr<InputWidgetImpl> pimpl_;
};

}

#endif
