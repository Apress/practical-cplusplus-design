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

#ifndef CORE_COMMANDS_H
#define CORE_COMMANDS_H

#include "Command.h"
#include <string>
#include <stack>

namespace pdCalc {

// accepts a number from input and adds it to the stack
// no preconditions are necessary for this command
class EnterNumber : public Command
{
public:
    explicit EnterNumber(double d);
    explicit EnterNumber(const EnterNumber&);
    ~EnterNumber();

private:
    EnterNumber(EnterNumber&&) = delete;
    EnterNumber& operator=(const EnterNumber&) = delete;
    EnterNumber& operator=(EnterNumber&&) = delete;

    // adds the number to the stack
    void executeImpl() noexcept override;

    // removes the number from the stack
    void undoImpl() noexcept override;

    EnterNumber* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;

    double number_;
};

// swaps two numbers on the stack
// precondition: must have two elements on the stack
class SwapTopOfStack : public Command
{
public:
    SwapTopOfStack() { }
    explicit SwapTopOfStack(const SwapTopOfStack&);
    ~SwapTopOfStack();

private:
    SwapTopOfStack(SwapTopOfStack&&) = delete;
    SwapTopOfStack& operator=(const SwapTopOfStack&) = delete;
    SwapTopOfStack& operator=(SwapTopOfStack&&) = delete;

    // throws an exception if the stack size is less than two
    void checkPreconditionsImpl() const override;

    // swaps the top two stack positions
    void executeImpl() noexcept override;

    // reverses the swap operation
    void undoImpl() noexcept override;

    SwapTopOfStack* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// drop the top of the stack
// precondition: must have at least one element on the stack
class DropTopOfStack : public Command
{
public:
    DropTopOfStack() { }
    explicit DropTopOfStack(const DropTopOfStack&);
    ~DropTopOfStack();

private:
    DropTopOfStack(DropTopOfStack&&) = delete;
    DropTopOfStack& operator=(const DropTopOfStack&) = delete;
    DropTopOfStack& operator=(DropTopOfStack&&) = delete;

    // throws an execption if the stack size is less than one
    void checkPreconditionsImpl() const override;

    // drops the top element on the stack
    void executeImpl() noexcept override;

    // returns the dropped element back to the top of the stack
    void undoImpl() noexcept override;

    DropTopOfStack* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;

    double droppedNumber_;
};

// clear the stack
// precondition: must have at least one element on the stack
class ClearStack : public Command
{
public:
    ClearStack(){ }
    explicit ClearStack(const ClearStack&);
    ~ClearStack();

private:
    ClearStack(ClearStack&&) = delete;
    ClearStack& operator=(const ClearStack&) = delete;
    ClearStack& operator=(ClearStack&&) = delete;

    // clears the stack
    void executeImpl() noexcept override;

    // restores the stack
    void undoImpl() noexcept override;

    ClearStack* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;

    std::stack<double> stack_;
};

// adds two elements on the stack
class Add : public BinaryCommand
{
public:
    Add() { }
    explicit Add(const Add&);
    ~Add();

private:
    Add(Add&&) = delete;
    Add& operator=(const Add&) = delete;
    Add& operator=(Add&&) = delete;

    double binaryOperation(double next, double top) const noexcept override;

    Add* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// subtracts two elements on the stack
class Subtract : public BinaryCommand
{
public:
    Subtract() { }
    explicit Subtract(const Subtract&);
    ~Subtract();

private:
    Subtract(Subtract&&) = delete;
    Subtract& operator=(const Subtract&) = delete;
    Subtract& operator=(Subtract&&) = delete;

    double binaryOperation(double next, double top) const noexcept override;

    Subtract* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// class Multiply is missing. It is achieved through BinaryCommandAlterative.
// See its creation in RegisterCoreCommands()

// divides two elements on the stack
// precondition: divisor cannot be zero
class Divide : public BinaryCommand
{
public:
    Divide() { }
    explicit Divide(const Divide&);
    ~Divide();

private:
    Divide(Divide&&) = delete;
    Divide& operator=(const Divide&) = delete;
    Divide& operator=(Divide&&) = delete;

    // checks binary preconditions
    // and throws if division by 0
    // checks for true 0, not epsilon close
    void checkPreconditionsImpl() const override;

    double binaryOperation(double next, double top) const noexcept override;

    Divide* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes two numbers from the stack (y == next, x == top) and implements y^x
// preconditions: 1) Two elements on the stack
//                2) If y == 0, x must be >= 0
//                3) If y < 0, x must be integral
class Power : public BinaryCommand
{
public:
    Power() { }
    explicit Power(const Power&);
    ~Power();

private:
    Power(Power&&) = delete;
    Power& operator=(const Power&) = delete;
    Power& operator=(Power&&) = delete;

    // throws if preconditions are not met
    void checkPreconditionsImpl() const override;

    double binaryOperation(double next, double top) const noexcept override;

    Power* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes two numbers from the stack (y == next, x == top) and implements xth root of y
// preconditions: 1) Two elements on the stack
//                2) If y == 0, x must be >= 0
//                3) If y < 0, 1/x must be integral
//                4) x != 0
class Root : public BinaryCommand
{
public:
    Root() { }
    explicit Root(const Root&);
    ~Root();

private:
    Root(Root&&) = delete;
    Root& operator=(const Root&) = delete;
    Root& operator=(Root&&) = delete;

    // throws if preconditions are not met
    void checkPreconditionsImpl() const override;

    double binaryOperation(double next, double top) const noexcept override;

    Root* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes the sine of a number on the stack in radians
// precondition: at least one number on the stack
class Sine : public UnaryCommand
{
public:
    Sine() { }
    explicit Sine(const Sine&);
    ~Sine();

private:
    Sine(Sine&&) = delete;
    Sine& operator=(const Sine&) = delete;
    Sine& operator=(Sine&&) = delete;

    double unaryOperation(double top) const noexcept override;

    Sine* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes the cosine of a number on the stack in radians
// precondition: at least one number on the stack
class Cosine : public UnaryCommand
{
public:
    Cosine() { }
    explicit Cosine(const Cosine&);
    ~Cosine();

private:
    Cosine(Cosine&&) = delete;
    Cosine& operator=(const Cosine&) = delete;
    Cosine& operator=(Cosine&&) = delete;

    double unaryOperation(double top) const noexcept override;

    Cosine* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes the tangent of a number on the stack in radians
// preconditions: 1) at least one number on the stack
//                2) number cannot be a multiple of pi/2 +/- pi
class Tangent : public UnaryCommand
{
public:
    Tangent() { }
    explicit Tangent(const Tangent&);
    ~Tangent();

private:
    Tangent(Tangent&&) = delete;
    Tangent& operator=(const Tangent&) = delete;
    Tangent& operator=(Tangent&&) = delete;

    // checks unary condition
    // throws if a multiple of pi/2 +/- pi
    void checkPreconditionsImpl() const override;

    double unaryOperation(double top) const noexcept override;

    Tangent* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes the arcsine of a number on the stack and returns radians
// preconditions: 1) at least one number on the stack
//                2) number, x, must satisfy -1 <= x <= 1
class Arcsine : public UnaryCommand
{
public:
    Arcsine() { }
    explicit Arcsine(const Arcsine&);
    ~Arcsine();

private:
    Arcsine(Arcsine&&) = delete;
    Arcsine& operator=(const Arcsine&) = delete;
    Arcsine& operator=(Arcsine&&) = delete;

    // checks unary condition
    // throws if value outside [-1, 1]
    void checkPreconditionsImpl() const override;

    double unaryOperation(double top) const noexcept override;

    Arcsine* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes the arccosine of a number on the stack and returns radians
// preconditions: 1) at lesat one number on the stack
//                2) number, x, must satisfy -1 <= x <= 1
class Arccosine : public UnaryCommand
{
public:
    Arccosine() { }
    explicit Arccosine(const Arccosine&);
    ~Arccosine();

private:
    Arccosine(Arccosine&&) = delete;
    Arccosine& operator=(const Arccosine&) = delete;
    Arccosine& operator=(Arccosine&&) = delete;

    // check unary condition
    // throws if value outside [-1, 1]
    void checkPreconditionsImpl() const override;

    double unaryOperation(double top) const noexcept override;

    Arccosine* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes the arctangent of a number ron the stack and returns radians
// precondition: at least one number on the stack
class Arctangent : public UnaryCommand
{
public:
    Arctangent() { }
    explicit Arctangent(const Arctangent&);
    ~Arctangent();

private:
    Arctangent(Arctangent&&) = delete;
    Arctangent& operator=(const Arctangent&) = delete;
    Arctangent& operator=(Arctangent&&) = delete;

    double unaryOperation(double top) const noexcept override;

    Arctangent* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes the top of the stack and negates it
// precondition: at least one number on the stack
class Negate : public UnaryCommand
{
public:
    Negate() { }
    explicit Negate(const Negate&);
    ~Negate();

 private:
    Negate(Negate&&) = delete;
    Negate& operator=(const Negate&) = delete;
    Negate& operator=(Negate&&) = delete;

    double unaryOperation(double top) const noexcept override;

    Negate* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

// takes the top of the stack and duplicates it
// precondition: at least one number on the stack
class Duplicate : public Command
{
public:
    Duplicate() { }
    explicit Duplicate(const Duplicate&);
    ~Duplicate();

private:
    Duplicate(Duplicate&&) = delete;
    Duplicate& operator=(const Duplicate&) = delete;
    Duplicate& operator=(Duplicate&&) = delete;

    void checkPreconditionsImpl() const override;

    void executeImpl() noexcept override;

    void undoImpl() noexcept override;

    Duplicate* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;

};

class UserInterface;
void RegisterCoreCommands(UserInterface& ui);

}

#endif
