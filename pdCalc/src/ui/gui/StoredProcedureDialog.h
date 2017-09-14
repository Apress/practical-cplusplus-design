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

#ifndef STORED_PROCEDURE_DIALOG_H
#define STORED_PROCEDURE_DIALOG_H

#include <QDialog>
#include <string>

class QComboBox;

namespace pdCalc {

class StoredProcedureDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StoredProcedureDialog(QWidget* parent = nullptr);
    std::string getProcedure() const;

private:
    void populateComboBox();

    QComboBox* comboBox_;
};

}
#endif
