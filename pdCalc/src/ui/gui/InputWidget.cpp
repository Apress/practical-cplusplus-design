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

#include "InputWidget.h"
#include <vector>
#include <QSize>
#include <iostream>
#include "CommandButton.h"
#include <QGridLayout>
#include "LookAndFeel.h"

using std::cout;
using std::endl;
using std::string;

using std::vector;

namespace pdCalc {

class InputWidget::InputWidgetImpl : public QWidget
{
    Q_OBJECT
public:
    InputWidgetImpl(pdCalc::InputWidget* parent);
    void addCommandButton(const string& dispPrimaryCmd, const string& primaryCmd, const string& dispShftCmd, const string& shftCmd);
    void setupFinalButtons();
    QGridLayout* getLayout() { return layout_; }

private slots:
    void onEex();
    void onDecimal();
    void onPlusMinus();
    void onZero();
    void onOne();
    void onTwo();
    void onThree();
    void onFour();
    void onFive();
    void onSix();
    void onSeven();
    void onEight();
    void onNine();

private:
    void allocateButtons();
    void setupShortcuts();
    void makeConnections();
    void layoutButtons();

    pdCalc::InputWidget* parent_;
    int nAddedButtons_;
    QGridLayout* layout_;

    CommandButton* eexButton_;
    CommandButton* decimalButton_;
    CommandButton* plusMinusButton_;
    CommandButton* enterButton_;
    CommandButton* backspaceButton_;
    CommandButton* zeroButton_;
    CommandButton* oneButton_;
    CommandButton* twoButton_;
    CommandButton* threeButton_;
    CommandButton* fourButton_;
    CommandButton* fiveButton_;
    CommandButton* sixButton_;
    CommandButton* sevenButton_;
    CommandButton* eightButton_;
    CommandButton* nineButton_;
    CommandButton* plusButton_;
    CommandButton* minusButton_;
    CommandButton* multiplyButton_;
    CommandButton* divideButton_;
    CommandButton* sinButton_;
    CommandButton* cosButton_;
    CommandButton* tanButton_;
    CommandButton* powButton_;
    CommandButton* shiftButton_;
};

InputWidget::InputWidgetImpl::InputWidgetImpl(pdCalc::InputWidget* parent)
: QWidget{parent}
, parent_(parent)
, nAddedButtons_{0}
{
    allocateButtons();
    setupShortcuts();
    makeConnections();
    layout_ = new QGridLayout;
    layout_->setOriginCorner(Qt::BottomLeftCorner);
    layoutButtons();

    return;
}

void InputWidget::InputWidgetImpl::allocateButtons()
{
    eexButton_ = new CommandButton{"eex", "e", this};
    decimalButton_ = new CommandButton{".", ".", this};
    plusMinusButton_ = new CommandButton{"+/-", "+", this};
    enterButton_ = new CommandButton{"Enter", "", "swap", "swap", this };
    backspaceButton_ = new CommandButton{"Bksp", "", "clear", "clear", this};
    zeroButton_ = new CommandButton{"0", "0", this};
    oneButton_ = new CommandButton{"1", "1", this};
    twoButton_ = new CommandButton{"2", "2", this};
    threeButton_ = new CommandButton{"3", "3", this};
    fourButton_ = new CommandButton{"4", "4", this};
    fiveButton_ = new CommandButton{"5", "5", this};
    sixButton_ = new CommandButton{"6", "6", this};
    sevenButton_ = new CommandButton{"7", "7", this};
    eightButton_ = new CommandButton{"8", "8", this};
    nineButton_ = new CommandButton{"9", "9", this};
    plusButton_ = new CommandButton{"+", "+", this};
    minusButton_ = new CommandButton{"-", "-", this};
    multiplyButton_ = new CommandButton{"*", "*", this};
    divideButton_ = new CommandButton{"/", "/", this};
    sinButton_ = new CommandButton{"sin", "sin", "asin", "arcsin", this};
    cosButton_ = new CommandButton{"cos", "cos", "acos", "arccos", this};
    tanButton_ = new CommandButton{"tan", "tan", "atan", "arctan", this};
    powButton_ = new CommandButton{"pow", "pow", "root", "root", this};
    shiftButton_ = new CommandButton{"Shift", "", this};

    return;
}

void InputWidget::InputWidgetImpl::setupShortcuts()
{
    // "Enter" key on number pad
    enterButton_->registerShortcut(Qt::Key_Enter);
    enterButton_->registerShortcut(Qt::Key_Return);
    enterButton_->setToolTip("Enter / Duplicate");

    eexButton_->registerShortcut(Qt::Key_E);
    eexButton_->setToolTip("10^x");

    backspaceButton_->registerShortcut(Qt::Key_Backspace);
    backspaceButton_->setToolTip("Backspace / Drop");

    decimalButton_->registerShortcut(Qt::Key_Period);

    plusMinusButton_->registerShortcut(Qt::Key_P);
    plusMinusButton_->registerShortcut(Qt::Key_M);
    plusMinusButton_->setToolTip("+/- / negate");

    zeroButton_->registerShortcut(Qt::Key_0);
    oneButton_->registerShortcut(Qt::Key_1);
    twoButton_->registerShortcut(Qt::Key_2);
    threeButton_->registerShortcut(Qt::Key_3);
    fourButton_->registerShortcut(Qt::Key_4);
    fiveButton_->registerShortcut(Qt::Key_5);
    sixButton_->registerShortcut(Qt::Key_6);
    sevenButton_->registerShortcut(Qt::Key_7);
    eightButton_->registerShortcut(Qt::Key_8);
    nineButton_->registerShortcut(Qt::Key_9);

    // oddly required for the + above the = on my US keyboard
    plusButton_->registerShortcut(QKeySequence{Qt::SHIFT | Qt::Key_Plus});
    plusButton_->registerShortcut(QKeySequence{Qt::Key_Plus});

    minusButton_->registerShortcut(Qt::Key_Minus);

    // oddly required for the * above the 8 on my US keyboard
    multiplyButton_->registerShortcut(QKeySequence{Qt::SHIFT | Qt::Key_Asterisk});
    multiplyButton_->registerShortcut(QKeySequence{Qt::Key_Asterisk});

    divideButton_->registerShortcut(Qt::Key_Slash);

    shiftButton_->registerShortcut(Qt::Key_S);
    shiftButton_->setButtonTextColor( LookAndFeel::Instance().getShiftColor() );

    return;
}

void InputWidget::InputWidgetImpl::makeConnections()
{
    connect(enterButton_, SIGNAL(clicked(std::string, std::string)), parent_, SIGNAL(enterPressed()));
    connect(backspaceButton_, SIGNAL(clicked(std::string, std::string)), parent_, SIGNAL(backspacePressed()));

    connect(eexButton_, SIGNAL(clicked(std::string, std::string)), this, SLOT(onEex()));
    connect(decimalButton_, SIGNAL(clicked(std::string, std::string)), this, SLOT(onDecimal()));
    connect(plusMinusButton_, SIGNAL(clicked(std::string, std::string)), this, SLOT(onPlusMinus()));

    connect(zeroButton_, SIGNAL(clicked(std::string, std::string)), this, SLOT(onZero()));
    connect(oneButton_, SIGNAL(clicked(std::string, std::string)), this, SLOT(onOne()));
    connect(twoButton_, SIGNAL(clicked(std::string, std::string)), this, SLOT(onTwo()));
    connect(threeButton_, SIGNAL(clicked(std::string, std::string)), this, SLOT(onThree()));
    connect(fourButton_, SIGNAL(clicked(std::string, std::string)), this, SLOT(onFour()));
    connect(fiveButton_, SIGNAL(clicked(std::string, std::string)), this, SLOT(onFive()));
    connect(sixButton_, SIGNAL(clicked(std::string, std::string)), this, SLOT(onSix()));
    connect(sevenButton_, SIGNAL(clicked(std::string, std::string)), this, SLOT(onSeven()));
    connect(eightButton_, SIGNAL(clicked(std::string, std::string)), this, SLOT(onEight()));
    connect(nineButton_, SIGNAL(clicked(std::string, std::string)), this, SLOT(onNine()));

    connect(plusButton_, SIGNAL(clicked(std::string, std::string)), parent_, SIGNAL(commandEntered(std::string, std::string)));
    connect(minusButton_, SIGNAL(clicked(std::string, std::string)), parent_, SIGNAL(commandEntered(std::string, std::string)));
    connect(multiplyButton_, SIGNAL(clicked(std::string, std::string)), parent_, SIGNAL(commandEntered(std::string, std::string)));
    connect(divideButton_, SIGNAL(clicked(std::string, std::string)), parent_, SIGNAL(commandEntered(std::string, std::string)));
    connect(sinButton_, SIGNAL(clicked(std::string, std::string)), parent_, SIGNAL(commandEntered(std::string, std::string)));
    connect(cosButton_, SIGNAL(clicked(std::string, std::string)), parent_, SIGNAL(commandEntered(std::string, std::string)));
    connect(tanButton_, SIGNAL(clicked(std::string, std::string)), parent_, SIGNAL(commandEntered(std::string, std::string)));
    connect(powButton_, SIGNAL(clicked(std::string, std::string)), parent_, SIGNAL(commandEntered(std::string, std::string)));

    connect(shiftButton_, SIGNAL(clicked(std::string,std::string)), parent_, SIGNAL(shiftPressed()));

}

void InputWidget::InputWidgetImpl::layoutButtons()
{
    layout_->addWidget(enterButton_, 4, 0);
    layout_->addWidget(eexButton_, 4, 1);
    layout_->addWidget(backspaceButton_, 4, 2);
    layout_->addWidget(sevenButton_, 3, 0);
    layout_->addWidget(eightButton_, 3, 1);
    layout_->addWidget(nineButton_, 3, 2);
    layout_->addWidget(fourButton_, 2, 0);
    layout_->addWidget(fiveButton_, 2, 1);
    layout_->addWidget(sixButton_, 2, 2);
    layout_->addWidget(oneButton_, 1, 0);
    layout_->addWidget(twoButton_, 1, 1);
    layout_->addWidget(threeButton_, 1, 2);
    layout_->addWidget(zeroButton_, 0, 0);
    layout_->addWidget(decimalButton_, 0, 1);
    layout_->addWidget(plusMinusButton_, 0, 2);
    layout_->addWidget(plusButton_, 0, 3);
    layout_->addWidget(minusButton_, 1, 3);
    layout_->addWidget(multiplyButton_, 2, 3);
    layout_->addWidget(divideButton_, 3, 3);
    layout_->addWidget(sinButton_, 5, 0);
    layout_->addWidget(cosButton_, 5, 1);
    layout_->addWidget(tanButton_, 5, 2);
    layout_->addWidget(powButton_, 5, 3);
    layout_->addWidget(shiftButton_, 4, 3);

    return;
}

void InputWidget::InputWidgetImpl::addCommandButton(const string& dispPrimaryCmd, const string& primaryCmd,
const string& dispShftCmd, const string& shftCmd)
{
    int nCols{ layout_->columnCount() };
    int nRows{ layout_->rowCount() };
    int atCol{ nAddedButtons_ % nCols };
    int atRow{(atCol == 0 ? nRows : nRows - 1) };

    auto cb = new CommandButton{dispPrimaryCmd, primaryCmd, dispShftCmd, shftCmd};
    connect(cb, SIGNAL(clicked(std::string, std::string)), parent_, SIGNAL(commandEntered(std::string, std::string)));
    layout_->addWidget(cb, atRow, atCol);

    nAddedButtons_++;

    return;
}

void InputWidget::InputWidgetImpl::setupFinalButtons()
{
    int atRow = layout_->rowCount();
    auto undo = new CommandButton{"undo", "undo", this};
    connect(undo, SIGNAL(clicked(std::string, std::string)), parent_, SIGNAL(commandEntered(std::string, std::string)));
    undo->registerShortcut( QKeySequence{QKeySequence::Undo} );
    layout_->addWidget(undo, atRow, 0);

    auto redo = new CommandButton{"redo", "redo", this};
    connect(redo, SIGNAL(clicked(std::string, std::string)), parent_, SIGNAL(commandEntered(std::string, std::string)));
    layout_->addWidget(redo, atRow, 1);
    redo->registerShortcut( QKeySequence{QKeySequence::Redo} );

    auto procedure = new CommandButton{"proc", "proc", this};
    connect(procedure, SIGNAL(clicked(std::string, std::string)), parent_, SIGNAL(procedurePressed()));
    layout_->addWidget(procedure, atRow, 2);
    procedure->registerToolTip("Open a dialog to enter a procedure");
}

void InputWidget::InputWidgetImpl::onEex()
{
    emit parent_->characterEntered('e');
}

void InputWidget::InputWidgetImpl::onDecimal()
{
    emit parent_->characterEntered('.');
}

void InputWidget::InputWidgetImpl::onPlusMinus()
{
    emit parent_->plusMinusPressed();
}

void InputWidget::InputWidgetImpl::onZero()
{
    emit parent_->characterEntered('0');
}

void InputWidget::InputWidgetImpl::onOne()
{
    emit parent_->characterEntered('1');
}

void InputWidget::InputWidgetImpl::onTwo()
{
    emit parent_->characterEntered('2');
}

void InputWidget::InputWidgetImpl::onThree()
{
    emit parent_->characterEntered('3');
}

void InputWidget::InputWidgetImpl::onFour()
{
    emit parent_->characterEntered('4');
}

void InputWidget::InputWidgetImpl::onFive()
{
    emit parent_->characterEntered('5');
}

void InputWidget::InputWidgetImpl::onSix()
{
    emit parent_->characterEntered('6');
}

void InputWidget::InputWidgetImpl::onSeven()
{
    emit parent_->characterEntered('7');
}

void InputWidget::InputWidgetImpl::onEight()
{
    emit parent_->characterEntered('8');
}

void InputWidget::InputWidgetImpl::onNine()
{
    emit parent_->characterEntered('9');
}

InputWidget::InputWidget(QWidget* parent)
: QWidget{parent}
, pimpl_( std::make_unique<InputWidgetImpl>(this) )
{
    setLayout( pimpl_->getLayout() );
}

InputWidget::~InputWidget()
{ }

void InputWidget::addCommandButton(const string& dispPrimaryCmd, const string& primaryCmd, const string& dispShftCmd, const string& shftCmd)
{
    pimpl_->addCommandButton(dispPrimaryCmd, primaryCmd, dispShftCmd, shftCmd);

    return;
}

void InputWidget::setupFinalButtons()
{
    pimpl_->setupFinalButtons();

    return;
}

}

#include "InputWidget.moc"
