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

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <istream>

namespace pdCalc {

class  Tokenizer
{
public:
    using Token = std::string;
    using Tokens = std::vector<Token>;
    using const_iterator =  Tokens::const_iterator;

    explicit Tokenizer(const std::string&);
    explicit Tokenizer(std::istream&);
    ~Tokenizer();

    size_t nTokens() const { return tokens_.size(); }

    const_iterator begin() const { return tokens_.begin(); }
    const_iterator end() const { return tokens_.end(); }

    const Token& operator[](size_t i) const { return tokens_[i]; }

private:
    void tokenize(std::istream&);

    Tokenizer() = delete;
    Tokenizer(const Tokenizer&) = delete;
    Tokenizer(Tokenizer&&) = delete;
    Tokenizer& operator=(const Tokenizer&) = delete;
    Tokenizer& operator=(Tokenizer&&) = delete;

    Tokens tokens_;
};

}

#endif
