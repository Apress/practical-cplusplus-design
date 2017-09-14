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

#include "StoredProcedure.h"
#include "CommandDispatcher.h"
#include "utilities/Exception.h"
#include "utilities/Tokenizer.h"
#include <fstream>

using std::string;

namespace pdCalc {

StoredProcedure::StoredProcedure(UserInterface& ui, const string& filename)
: filename_{filename}
{
    ce_ = std::make_unique<CommandDispatcher>(ui);
}

StoredProcedure::~StoredProcedure()
{ }

void StoredProcedure::checkPreconditionsImpl() const
{
    if(first_)
    {
        try
        {
            std::ifstream ifs{ filename_.c_str() };
            if(!ifs)
                throw Exception{"Could not open procedure"};

            tokenizer_ = std::make_unique<Tokenizer>(ifs);
        }
        catch(...)
        {
            throw Exception{"Could not open procedure"};
        }
    }

    return;
}

void StoredProcedure::executeImpl() noexcept
{
    if(first_)
    {
        for(auto c : *tokenizer_)
        {
            ce_->commandEntered(c);
        }
        first_ = false;
    }
    else
    {
        for(unsigned int i = 0; i < tokenizer_->nTokens(); ++i)
            ce_->commandEntered("redo");
    }

    return;
}

void StoredProcedure::undoImpl() noexcept
{
    for(unsigned int i = 0; i < tokenizer_->nTokens(); ++i)
        ce_->commandEntered("undo");

    return;
}

Command*StoredProcedure::cloneImpl() const noexcept
{
    return 0;
}

const char* StoredProcedure::helpMessageImpl() const noexcept
{
    return "Executes a stored procedure from disk";
}

}

