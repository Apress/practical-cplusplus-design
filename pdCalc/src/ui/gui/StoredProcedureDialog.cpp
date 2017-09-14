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

#include "StoredProcedureDialog.h"
#include <QLabel>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QDir>
#include <QStringList>

namespace pdCalc {

StoredProcedureDialog::StoredProcedureDialog(QWidget *parent)
: QDialog{parent}
{
    auto layout = new QVBoxLayout{this};

    auto label = new QLabel{"Enter procedure name:", this};
    layout->addWidget(label);

    comboBox_ = new QComboBox{this};
    comboBox_->setEditable(true);
    comboBox_->setMinimumWidth(200);
    layout->addWidget(comboBox_);

    populateComboBox();

    auto buttonBox = new QDialogButtonBox{QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this};
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    layout->addWidget(buttonBox);

    setWindowTitle("Procedure");
}

std::string StoredProcedureDialog::getProcedure() const
{
    return comboBox_->currentText().toStdString();
}

void StoredProcedureDialog::populateComboBox()
{
    // display all names in current directory with extension of .psp (pdCalc stored procedure)
    QDir dir{"."};
    dir.setFilter(QDir::Files);
    QStringList lst;
    lst << "*.psp";
    dir.setNameFilters(lst);

    comboBox_->addItems( dir.entryList() );

    return;
}

}
