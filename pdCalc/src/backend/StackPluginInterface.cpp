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

#include "Stack.h"
#include "StackPluginInterface.h"

using std::vector;

void StackPush(double d, bool suppressChangeEvent)
{
    pdCalc::Stack::Instance().push(d, suppressChangeEvent);

    return;
}

double StackPop(bool suppressChangeEvent)
{
    return pdCalc::Stack::Instance().pop(suppressChangeEvent);
}

size_t StackSize()
{
    return pdCalc::Stack::Instance().size();
}

double StackFirstElement()
{
    vector<double> v = pdCalc::Stack::Instance().getElements(1);
    return v[0];
}

double StackSecondElement()
{
    vector<double> v = pdCalc::Stack::Instance().getElements(2);
    return v[1];
}
