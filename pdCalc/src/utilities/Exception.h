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

#ifndef EXCEPTION_H
#define EXCEPTION_H

// Simple exception class to be used throughout PdCalc. It can be instantiated with a message,
// and that message can be returned.

#include <string>

namespace pdCalc {

class Exception
{
public:
    explicit Exception(const std::string& msg) : msg_(msg) { }
    const std::string& what() const { return msg_; }
  
private:
    std::string msg_;
};

}

#endif 
