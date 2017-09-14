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

#ifndef CLI_H
#define CLI_H

#include "utilities/UserInterface.h"
#include <string>
#include <memory>
#include <iostream>

namespace pdCalc {

class Cli : public UserInterface
{
    class CliImpl;
public:
    Cli(std::istream&, std::ostream&);
    ~Cli();

    // start the cli run loop
    void execute(bool suppressStartupMessage = false, bool echo = false);

private:
    // posts a text message to the output
    void postMessage(const std::string& m) override;

    // updates the output when the stack is changed
    void stackChanged() override;

    Cli(const Cli&) = delete;
    Cli(Cli&&) = delete;
    Cli& operator=(const Cli&) = delete;
    Cli& operator=(Cli&&) = delete;

    std::unique_ptr<CliImpl> pimpl_;
};

}

#endif
