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

#include "CommandButton.h"
#include "LookAndFeel.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <iostream>
#include <QShortcut>
#include <QToolTip>
#include <sstream>

using std::ostringstream;
using std::cout;
using std::endl;

using std::string;

namespace pdCalc {

CommandButton::CommandButton(const string& dispPrimaryCmd, const string& primaryCmd,
const string& dispShftCmd, const string& shftCmd, QWidget* parent)
: QWidget{parent}
, primaryCmd_{primaryCmd}
, shftCmd_{shftCmd}
{
    setup(dispPrimaryCmd, dispShftCmd);
}

CommandButton::CommandButton(const std::string& dispPrimaryCmd, const std::string& primaryCmd, QWidget* parent)
: QWidget{parent}
, primaryCmd_{primaryCmd}
, shftCmd_{""}
{
    setup(dispPrimaryCmd, "");
}

void CommandButton::registerShortcut(const QKeySequence& sequence)
{
    // Use the shortcut mechanism rather than btn_->setShortcut() to allow
    // multiple different shortcuts for one button (e.g., plus/minus button)
    // shortcuts with both p and m. btn_->setShortcut() releases the current
    // shortcut when the next one is set.

    auto sc = new QShortcut(sequence, btn_);
    connect(sc, SIGNAL(activated()), this, SLOT(onClicked()));
}

void CommandButton::registerToolTip(const string& s)
{
    btn_->setToolTip( QString::fromStdString(s) );
    return;
}

void CommandButton::setButtonTextColor(const std::string& color)
{
    ostringstream oss;
    oss << "QPushButton { color : " << color << "; }";
    btn_->setStyleSheet( oss.str().c_str() );

    return;
}

void CommandButton::onClicked()
{
    emit clicked(primaryCmd_, shftCmd_);
}

void CommandButton::setup(const std::string& dispPrimaryCmd, const std::string& dispShftCmd)
{
    btn_ = new QPushButton{ QString::fromStdString(dispPrimaryCmd), this};
    btn_->setFont( LookAndFeel::Instance().getButtonFont() );
    btn_->setFixedSize( LookAndFeel::Instance().getMinimumButtonSize() );

    auto label = new QLabel{ QString::fromStdString(dispShftCmd), this };
    label->setFont( LookAndFeel::Instance().getShiftLabelFont() );
    label->setMinimumSize( ( dispShftCmd.empty() ? QSize(0, 0) : LookAndFeel::Instance().getMinimumShiftLabelSize()) );

    ostringstream oss;
    oss << "QLabel { color : " << LookAndFeel::Instance().getShiftColor() <<"; }";
    label->setStyleSheet( oss.str().c_str() );

    connect(btn_, SIGNAL(clicked()), this, SLOT(onClicked()));

    auto layout = new QVBoxLayout{this};
    layout->setSpacing(0);
    layout->addWidget(label);
    layout->addWidget(btn_);
    layout->setContentsMargins( LookAndFeel::Instance().getContentsMargins() );
}

}
