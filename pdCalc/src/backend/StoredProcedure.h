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

#ifndef STORED_PROCEDURE_H
#define STORED_PROCEDURE_H

#include "Command.h"
#include "utilities/Tokenizer.h"
#include <string>
#include <memory>

namespace pdCalc {

class CommandDispatcher;
class UserInterface;

class StoredProcedure : public Command
{
public:
    StoredProcedure(UserInterface& ui, const std::string& filename);
    ~StoredProcedure();

private:
    StoredProcedure() = delete;
    StoredProcedure(StoredProcedure&&) = delete;
    StoredProcedure& operator=(const StoredProcedure&) = delete;
    StoredProcedure& operator=(StoredProcedure&&) = delete;
    StoredProcedure(const StoredProcedure&) = delete;

    void checkPreconditionsImpl() const override;
    void executeImpl() noexcept override;
    void undoImpl() noexcept override;
    Command* cloneImpl() const noexcept override;
    const char* helpMessageImpl() const noexcept override;

    mutable std::unique_ptr<Tokenizer> tokenizer_;
    std::unique_ptr<CommandDispatcher> ce_;
    std::string filename_;
    bool first_ = true;
};

}

#endif
