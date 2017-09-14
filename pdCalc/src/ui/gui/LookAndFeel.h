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

#ifndef LOOK_AND_FEEL_H
#define LOOK_AND_FEEL_H

#include <memory>

class QFont;
class QSize;
class QPalette;
class QMargins;

namespace pdCalc {

// This is a singleton class that defines the metrics to be used by all
// of the display. An example of such a metric would be a font family or
// button size.

class LookAndFeel
{
    class LookAndFeelImpl;
public:
    static const LookAndFeel& Instance();

    // get the font that should be used by the display
    const QFont& getDisplayFont() const;

    // get the font that should be used by the buttons
    const QFont& getButtonFont() const;

    // get the font that should be used by the shift labels for the buttons
    const QFont& getShiftLabelFont() const;

    // get the minimum size of the buttons
    const QSize& getMinimumButtonSize() const;

    // get the minimum size of the shift label
    const QSize& getMinimumShiftLabelSize() const;

    // get the font that should be used for the status bar
    const QFont& getStatusBarFont() const;

    // get the palette for the status bar to enable color change of status bar text
    const QPalette& getStatusBarPalette() const;

    // get contents margins for layouts affecting button spacing
    const QMargins& getContentsMargins() const;

    // get the color of the shift label
    const std::string& getShiftColor() const;

    // get display background color
    std::string getDisplayBackgroundColor() const;

    // get display border style
    std::string getDisplayBorderStyle() const;

    // get display border radius
    std::string getDisplayBorderRadius() const;

private:
    LookAndFeel();
    ~LookAndFeel();
    LookAndFeel(const LookAndFeel&) = delete;
    LookAndFeel& operator=(const LookAndFeel&) = delete;
    LookAndFeel(LookAndFeel&&) = delete;
    LookAndFeel& operator=(LookAndFeel&&) = delete;

    std::unique_ptr<LookAndFeelImpl> pimpl_;
};

}

#endif
