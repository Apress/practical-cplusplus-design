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

#include "LookAndFeel.h"
#include <QFont>
#include <QSize>
#include <QFontMetrics>
#include <QPalette>
#include <QColor>
#include <QMargins>
#include <string>

using std::string;

namespace pdCalc {

class LookAndFeel::LookAndFeelImpl
{
public:
    LookAndFeelImpl();
    const QFont& getDisplayFont() const { return displayFont_; }
    const QFont& getButtonFont() const { return buttonFont_; }
    const QFont& getShiftLabelFont() const { return shiftLabelFont_; }
    const QFont& getStatusBarFont() const { return statusBarFont_; }

    const QSize& getMinimumButtonSize() const { return minButtonSize_; }
    const QSize& getMinimumShiftLabelSize() const { return minShiftLabelSize_; }
    const QPalette& getStatusBarPalette() const { return statusBarPalette_; }
    const QMargins& getContentsMargins() const { return contentsMargins_; }
    const string& getShiftColor() const { return shiftColor_; }

private:
    void setupDisplayFont();
    void setupButtonFont();
    void setupShiftLabelFont();
    void setupStatusBarMetrics();
    void setupContentsMargins();

    QFont displayFont_;
    QFont buttonFont_;
    QFont shiftLabelFont_;
    QFont statusBarFont_;
    QPalette statusBarPalette_;

    QSize minButtonSize_;
    QSize minShiftLabelSize_;

    QMargins contentsMargins_;

    unsigned int displayFontPtSize_;
    string shiftColor_;
    QString fixedWidthFont_;
};

LookAndFeel::LookAndFeelImpl::LookAndFeelImpl()
: displayFontPtSize_{14}
, shiftColor_{"blue"}
, fixedWidthFont_{"courier"}
{
    setupDisplayFont();
    setupButtonFont();
    setupShiftLabelFont();
    setupStatusBarMetrics();
    setupContentsMargins();
}

void LookAndFeel::LookAndFeelImpl::setupDisplayFont()
{
    displayFont_.setFamily(fixedWidthFont_);
    displayFont_.setPointSize(displayFontPtSize_);

    return;
}

void LookAndFeel::LookAndFeelImpl::setupButtonFont()
{
    buttonFont_.setFamily(fixedWidthFont_);
    buttonFont_.setPointSize(displayFontPtSize_ - 2);

    QFontMetrics fm{buttonFont_};
    minButtonSize_.setHeight( 1.5 * fm.height() );
    minButtonSize_.setWidth( 6 * fm.maxWidth() );

    return;
}

void LookAndFeel::LookAndFeelImpl::setupShiftLabelFont()
{
    shiftLabelFont_.setFamily(fixedWidthFont_);
    shiftLabelFont_.setPointSize(displayFontPtSize_ - 5);

    QFontMetrics fm{shiftLabelFont_};
    minShiftLabelSize_.setHeight( 1.5 * fm.height() );
    minShiftLabelSize_.setWidth( 6 * fm.maxWidth() );
    return;
}

void LookAndFeel::LookAndFeelImpl::setupStatusBarMetrics()
{
    statusBarFont_.setFamily(fixedWidthFont_);
    statusBarFont_.setPointSize(displayFontPtSize_ - 4);

    statusBarPalette_.setColor(QPalette::WindowText, QColor(QString::fromStdString(shiftColor_)));

    return;
}

void LookAndFeel::LookAndFeelImpl::setupContentsMargins()
{
    contentsMargins_.setTop(1);
    contentsMargins_.setBottom(1);
    contentsMargins_.setLeft(1);
    contentsMargins_.setRight(1);

    return;
}

const LookAndFeel& LookAndFeel::Instance()
{
    static LookAndFeel instance;
    return instance;
}

const QFont &LookAndFeel::getDisplayFont() const
{
    return pimpl_->getDisplayFont();
}

const QFont&LookAndFeel::getButtonFont() const
{
    return pimpl_->getButtonFont();
}

const QFont&LookAndFeel::getShiftLabelFont() const
{
    return pimpl_->getShiftLabelFont();
}

const QSize&LookAndFeel::getMinimumButtonSize() const
{
    return pimpl_->getMinimumButtonSize();
}

const QSize&LookAndFeel::getMinimumShiftLabelSize() const
{
    return pimpl_->getMinimumShiftLabelSize();
}

const QFont&LookAndFeel::getStatusBarFont() const
{
    return pimpl_->getStatusBarFont();
}

const QPalette&LookAndFeel::getStatusBarPalette() const
{
    return pimpl_->getStatusBarPalette();
}

const QMargins& LookAndFeel::getContentsMargins() const
{
    return pimpl_->getContentsMargins();
}

const std::string& LookAndFeel::getShiftColor() const
{
    return pimpl_->getShiftColor();
}

std::string LookAndFeel::getDisplayBackgroundColor() const
{
    return "white";
}

std::string LookAndFeel::getDisplayBorderStyle() const
{
    return "1px solid gray";
}

std::string LookAndFeel::getDisplayBorderRadius() const
{
    return "6px";
}

LookAndFeel::LookAndFeel()
: pimpl_{ new LookAndFeelImpl }
{ }

LookAndFeel::~LookAndFeel()
{ }

}
