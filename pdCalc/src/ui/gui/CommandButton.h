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

#ifndef COMMANDBUTTON_H
#define COMMANDBUTTON_H

#include <QWidget>
#include <string>

class QPushButton;

namespace pdCalc {

// Class capable of emitting a signal with the primary and shift commands when the
// button is clicked. Note that the receiver is responsible for figuring out if the
// calculator is in shifted state.
class CommandButton : public QWidget
{
    Q_OBJECT
public:
    // Initialize the button with the primary command to be displayed, the string commanad
    // this triggers, the shift command to be displayed, and the string command the
    // shift triggers
    CommandButton(const std::string& dispPrimaryCmd, const std::string& primaryCmd,
        const std::string& dispShftCmd, const std::string& shftCmd, QWidget* parent = nullptr);

    CommandButton(const std::string& dispPrimaryCmd, const std::string& primaryCmd,
        QWidget* parent = nullptr);

    // registers a keyboard shortcut
    void registerShortcut(const QKeySequence &sequence);

    // enter a tooltip so that hovering over the button will cause text to be displayed,
    // presumably the text will be the keyboard shortuct
    void registerToolTip(const std::string& s);

    // change button color (used, for example, for a shift button)
    void setButtonTextColor(const std::string& color);

private slots:
    void onClicked();

signals:
    void clicked(std::string primCmd, std::string shftCmd);

private:
    void setup(const std::string& dispPrimaryCmd, const std::string& dispShftCommand);

    std::string primaryCmd_;
    std::string shftCmd_;

    QPushButton* btn_;
};

}

#endif // COMMANDBUTTON_H
