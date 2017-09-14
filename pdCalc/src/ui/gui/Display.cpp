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

#include "Display.h"
#include <QVBoxLayout>
#include <QLabel>
#include <sstream>
#include <QResizeEvent>
#include <vector>
#include <string>
#include "LookAndFeel.h"
#include <utility>
#include <QStatusBar>
#include <QPalette>
#include <QColor>
#include "GuiModel.h"

using std::vector;
using std::ostringstream;
using std::string;

namespace pdCalc {

class Display::DisplayImpl : public QWidget
{
public:
    explicit DisplayImpl(const GuiModel&, int nLinesStack, int nCharWide, QVBoxLayout* layout, Display* parent);
    string getText() const;
    void showMessage(const string& m);
    void onModelChanged();

private:
    void setupSize();
    void resizeEvent(QResizeEvent*) override;
    string createLine(int lineNumber, double value, int stackSize);
    QValidator::State validate(const string&);

    const GuiModel& guiModel_;
    QLabel* label_;
    int nLinesStack_;
    int nCharWide_;
    QStatusBar* statusBar_;
    unsigned int statusBarTimeout;
    QLabel* shiftIndicator_;
};

Display::DisplayImpl::DisplayImpl(const GuiModel& g, int nLinesStack, int nCharWide, QVBoxLayout* layout, Display* parent)
: QWidget{parent}
, guiModel_{g}
, nLinesStack_{nLinesStack}
, nCharWide_{nCharWide}
, statusBarTimeout{3000}
{
    statusBar_ = new QStatusBar{this};
    statusBar_->setFont( LookAndFeel::Instance().getStatusBarFont() );
    statusBar_->setSizeGripEnabled(false);
    statusBar_->setPalette( LookAndFeel::Instance().getStatusBarPalette() );
    shiftIndicator_ = new QLabel{"Shift", this};
    statusBar_->addPermanentWidget(shiftIndicator_);
    shiftIndicator_->hide();

    label_ = new QLabel{this};

    ostringstream oss;
    oss << "QLabel { background-color : " << LookAndFeel::Instance().getDisplayBackgroundColor() << "; "
        << "border : " << LookAndFeel::Instance().getDisplayBorderStyle() << "; "
        << "border-radius : " << LookAndFeel::Instance().getDisplayBorderRadius() << "}";
    label_->setStyleSheet( oss.str().c_str() );

    layout->addWidget(statusBar_);
    layout->addWidget(label_);

    label_->setFont( LookAndFeel::Instance().getDisplayFont() );

    setupSize();

    onModelChanged();
}

string Display::DisplayImpl::getText() const
{
    return label_->text().toStdString();
}

void Display::DisplayImpl::showMessage(const string& m)
{
    statusBar_->showMessage( QString::fromStdString(m), statusBarTimeout);
    return;
}

void Display::DisplayImpl::setupSize()
{
    QFontMetrics fm{LookAndFeel::Instance().getDisplayFont()};
    label_->setFixedHeight( nLinesStack_ * fm.height() + 2);
    label_->setMinimumWidth( (nCharWide_ + 2) * fm.maxWidth() );

    return;
}

void Display::DisplayImpl::resizeEvent(QResizeEvent* event)
{
    QFontMetrics fm{LookAndFeel::Instance().getDisplayFont()};
    int w{ event->size().width() };
    nCharWide_ = w / fm.maxWidth() - 2;

    onModelChanged();

    return;
}

string Display::DisplayImpl::createLine(int lineNumber, double sv, int stackSize)
{
    string value{""};
    if(lineNumber < stackSize)
    {
        ostringstream t;
        t.precision(12);
        t << sv;
        value = t.str();
    }

    ostringstream oss;
    oss << lineNumber + 1 << ":";
    int lineLabelSize{ static_cast<int>(oss.str().size()) };
    for(int i = 0; i < nCharWide_ - static_cast<int>( value.length() ) - lineLabelSize; ++i)
    {
        oss << ' ';
    }
    oss << value;
    return oss.str();
}

void Display::DisplayImpl::onModelChanged()
{
    const GuiModel::State& state = guiModel_.getState();
    if(state.shiftState == GuiModel::ShiftState::Shifted)
    {
        shiftIndicator_->show();
    }
    else shiftIndicator_->hide();

    ostringstream oss;
    auto hasInput = state.curInput.size() != 0;
    auto start =  nLinesStack_ - ( hasInput ? 1 : 0 );

    for(int i = start - 1; i > -1; --i)
    {
        bool valueExists = i < static_cast<int>( state.curStack.size() );
        oss << createLine( i, (valueExists ? state.curStack[i] : 0 /*dummy value*/), state.curStack.size() ) << (i != 0 ? "\n" : "");
    }

    if(hasInput)
    {
        oss << "\n"
            << state.curInput;
    }

    label_->setText( QString::fromStdString( oss.str() ) );
}

Display::Display(const GuiModel& g, QWidget *parent, int nLinesStack, int nCharWide)
: QWidget{parent}
{
    auto layout = new QVBoxLayout{this};
    pimpl_ = new DisplayImpl{g, nLinesStack, nCharWide, layout, this};
    layout->setContentsMargins( LookAndFeel::Instance().getContentsMargins() );
    layout->addWidget(pimpl_);
}

std::string Display::getText() const
{
    return pimpl_->getText();
}

void Display::showMessage(const std::string& m)
{
    pimpl_->showMessage(m);

    return;
}

void Display::onModelChanged()
{
    pimpl_->onModelChanged();
}


}
