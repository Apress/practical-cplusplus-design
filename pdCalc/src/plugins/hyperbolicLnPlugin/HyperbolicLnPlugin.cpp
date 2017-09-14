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

#include "HyperbolicLnPlugin.h"
#include "backend/Command.h"
#include "backend/StackPluginInterface.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <memory>

using std::cout;
using std::endl;

using std::vector;
using std::string;
using std::unique_ptr;

class HyperbolicLnPluginCommand : public pdCalc::PluginCommand
{
public:
    HyperbolicLnPluginCommand(){}
    explicit HyperbolicLnPluginCommand(const HyperbolicLnPluginCommand& rhs);
    virtual ~HyperbolicLnPluginCommand(){}
    void deallocate() override;

protected:
    const char* checkPluginPreconditions() const noexcept override;

private:
    void executeImpl() noexcept override;

    // drops the result and returns the original number to the stack
    void undoImpl() noexcept override;

    HyperbolicLnPluginCommand* clonePluginImpl() const noexcept override;
    virtual HyperbolicLnPluginCommand* doClone() const = 0;

    virtual double unaryOperation(double top) const = 0;

    double top_;
};

void HyperbolicLnPluginCommand::deallocate()
{
    delete this;
}

HyperbolicLnPluginCommand::HyperbolicLnPluginCommand(const HyperbolicLnPluginCommand& rhs)
: PluginCommand(rhs)
, top_(rhs.top_)
{
}

const char* HyperbolicLnPluginCommand::checkPluginPreconditions() const  noexcept
{
    if( StackSize() < 1 )
        return "Stack must have one element";
    else return nullptr;
}

void HyperbolicLnPluginCommand::executeImpl() noexcept
{
    // suppress change signal so only one event raised for the execute
    top_ = StackPop(true);
    StackPush( unaryOperation(top_), false );

    return;
}

void HyperbolicLnPluginCommand::undoImpl() noexcept
{
    // suppress change signal so only one event raised for the execute
    StackPop(true);
    StackPush(top_, false);

    return;
}

HyperbolicLnPluginCommand* HyperbolicLnPluginCommand::clonePluginImpl() const noexcept
{
    HyperbolicLnPluginCommand* p;
    try
    {
        p = doClone();
    }
    catch(...)
    {
        return nullptr;
    }

    return p;
}

// takes the hyperbolic sine of a number on the stack
// precondition: at least one number on the stack
class Sinh : public HyperbolicLnPluginCommand
{
public:
    Sinh() { }
    explicit Sinh(const Sinh&);
    ~Sinh();

private:
    Sinh(Sinh&&) = delete;
    Sinh& operator=(const Sinh&) = delete;
    Sinh& operator=(Sinh&&) = delete;

    double unaryOperation(double top) const override;

    Sinh* doClone() const override;

    const char* helpMessageImpl() const noexcept override;
};

Sinh::Sinh(const Sinh& rhs)
: HyperbolicLnPluginCommand{rhs}
{ }

Sinh::~Sinh()
{ }

double Sinh::unaryOperation(double top) const
{
    return std::sinh(top);
}

Sinh* Sinh::doClone() const
{
    return new Sinh{*this};
}

const char* Sinh::helpMessageImpl() const noexcept
{
    return "Replace the first element, x, on the stack with sinh(x)";
}

// takes the hyperbolic cosine of a number on the stack
// precondition: at least one number on the stack
class Cosh : public HyperbolicLnPluginCommand
{
public:
    Cosh() { }
    explicit Cosh(const Cosh&);
    ~Cosh();

private:
    Cosh(Cosh&&) = delete;
    Cosh& operator=(const Cosh&) = delete;
    Cosh& operator=(Cosh&&) = delete;

    double unaryOperation(double top) const override;

    Cosh* doClone() const override;

    const char* helpMessageImpl() const noexcept override;
};

Cosh::Cosh(const Cosh& rhs)
: HyperbolicLnPluginCommand{rhs}
{ }

Cosh::~Cosh()
{ }

double Cosh::unaryOperation(double top) const
{
    return std::cosh(top);
}

Cosh* Cosh::doClone() const
{
    return new Cosh{*this};
}

const char* Cosh::helpMessageImpl() const noexcept
{
    return "Replace the first element, x, on the stack with cosh(x)";
}

// takes the hyperbolic tangent of a number on the stack
// precondition: at least one number on the stack
class Tanh : public HyperbolicLnPluginCommand
{
public:
    Tanh() { }
    explicit Tanh(const Tanh&);
    ~Tanh();

private:
    Tanh(Tanh&&) = delete;
    Tanh& operator=(const Tanh&) = delete;
    Tanh& operator=(Tanh&&) = delete;

    double unaryOperation(double top) const override;

    Tanh* doClone() const  override;

    const char* helpMessageImpl() const noexcept override;
};

Tanh::Tanh(const Tanh& rhs)
: HyperbolicLnPluginCommand{rhs}
{ }

Tanh::~Tanh()
{ }

double Tanh::unaryOperation(double top) const
{
    return std::tanh(top);
}

Tanh* Tanh::doClone() const
{
    return new Tanh{*this};
}

const char* Tanh::helpMessageImpl() const noexcept
{
    return "Replace the first element, x, on the stack with tanh(x)";
}

// takes the inverse hyperbolic sine of a number on the stack
// precondition: at least one number on the stack
class Arcsinh : public HyperbolicLnPluginCommand
{
public:
    Arcsinh() { }
    explicit Arcsinh(const Arcsinh&);
    ~Arcsinh();

private:
    Arcsinh(Arcsinh&&) = delete;
    Arcsinh& operator=(const Arcsinh&) = delete;
    Arcsinh& operator=(Arcsinh&&) = delete;

    double unaryOperation(double top) const override;

    Arcsinh* doClone() const  override;

    const char* helpMessageImpl() const noexcept override;
};

Arcsinh::Arcsinh(const Arcsinh& rhs)
: HyperbolicLnPluginCommand{rhs}
{ }

Arcsinh::~Arcsinh()
{ }

double Arcsinh::unaryOperation(double top) const
{
    return std::log( top + std::sqrt(top * top + 1.0)  );
}

Arcsinh* Arcsinh::doClone() const
{
    return new Arcsinh{*this};
}

const char* Arcsinh::helpMessageImpl() const noexcept
{
    return "Replace the first element, x, on the stack with arcsinh(x)";
}

// takes the inverse hyperbolic cosine of a number on the stack
// preconditions: at least one number on the stack
//                number must be >= 1.0
class Arccosh : public HyperbolicLnPluginCommand
{
public:
    Arccosh() { }
    explicit Arccosh(const Arccosh&);
    ~Arccosh();

private:
    Arccosh(Arccosh&&) = delete;
    Arccosh& operator=(const Arccosh&) = delete;
    Arccosh& operator=(Arccosh&&) = delete;

    const char* checkPluginPreconditions() const noexcept override;
    double unaryOperation(double top) const override;

    Arccosh* doClone() const  override;

    const char* helpMessageImpl() const noexcept override;
};

Arccosh::Arccosh(const Arccosh& rhs)
: HyperbolicLnPluginCommand{rhs}
{ }

Arccosh::~Arccosh()
{ }

const char* Arccosh::checkPluginPreconditions() const noexcept
{
    const char* r = HyperbolicLnPluginCommand::checkPluginPreconditions();
    if(r) return r;

    auto v = StackFirstElement();
    if(v < 1.0)
        return "Imaginary result";

    return nullptr;
}

double Arccosh::unaryOperation(double top) const
{
    return std::log( top + std::sqrt(top * top - 1.0)  );
}

Arccosh* Arccosh::doClone() const
{
    return new Arccosh{*this};
}

const char* Arccosh::helpMessageImpl() const noexcept
{
    return "Replace the first element, x, on the stack with arccosh(x)";
}

// takes the inverse hyperbolic tangent of a number on the stack
// preconditions: at least one number on the stack
//                abs(number) must be < 1.0
class Arctanh : public HyperbolicLnPluginCommand
{
public:
    Arctanh() { }
    explicit Arctanh(const Arctanh&);
    ~Arctanh();

private:
    Arctanh(Arctanh&&) = delete;
    Arctanh& operator=(const Arctanh&) = delete;
    Arctanh& operator=(Arctanh&&) = delete;

    const char* checkPluginPreconditions() const noexcept override;
    double unaryOperation(double top) const override;

    Arctanh* doClone() const  override;

    const char* helpMessageImpl() const noexcept override;
};

Arctanh::Arctanh(const Arctanh& rhs)
: HyperbolicLnPluginCommand{rhs}
{ }

Arctanh::~Arctanh()
{ }

const char* Arctanh::checkPluginPreconditions() const noexcept
{
    const char* r = HyperbolicLnPluginCommand::checkPluginPreconditions();
    if(r) return r;

    auto v = StackFirstElement();
    if(std::fabs(v) >= 1.0)
        return "Imaginary result";

    return nullptr;
}

double Arctanh::unaryOperation(double top) const
{
    return 0.5 * std::log( (1.0 + top) / (1.0 - top) );
}

Arctanh* Arctanh::doClone() const
{
    return new Arctanh{*this};
}

const char* Arctanh::helpMessageImpl() const noexcept
{
    return "Replace the first element, x, on the stack with arctanh(x)";
}

// takes the exp of a number on the stack
// precondition: at least one number on the stack
class Exp : public HyperbolicLnPluginCommand
{
public:
    Exp() { }
    explicit Exp(const Exp&);
    ~Exp();

private:
    Exp(Exp&&) = delete;
    Exp& operator=(const Exp&) = delete;
    Exp& operator=(Exp&&) = delete;

    double unaryOperation(double top) const override;

    Exp* doClone() const  override;

    const char* helpMessageImpl() const noexcept override;
};

Exp::Exp(const Exp& rhs)
: HyperbolicLnPluginCommand{rhs}
{ }

Exp::~Exp()
{ }

double Exp::unaryOperation(double top) const
{
    return std::exp(top);
}

Exp* Exp::doClone() const
{
    return new Exp{*this};
}

const char* Exp::helpMessageImpl() const noexcept
{
    return "Replace the first element, x, on the stack with exp(x)";
}

// takes the natural logarithm of a number on the stack
// preconditions: at least one number on the stack
//                number must be > 0.0
class NaturalLog : public HyperbolicLnPluginCommand
{
public:
    NaturalLog() { }
    explicit NaturalLog(const NaturalLog&);
    ~NaturalLog();

private:
    NaturalLog(NaturalLog&&) = delete;
    NaturalLog& operator=(const NaturalLog&) = delete;
    NaturalLog& operator=(NaturalLog&&) = delete;

    const char* checkPluginPreconditions() const noexcept override;
    double unaryOperation(double top) const override;

    NaturalLog* doClone() const  override;

    const char* helpMessageImpl() const noexcept override;
};

NaturalLog::NaturalLog(const NaturalLog& rhs)
: HyperbolicLnPluginCommand{rhs}
{ }

NaturalLog::~NaturalLog()
{ }

const char* NaturalLog::checkPluginPreconditions() const noexcept
{
    const char* r = HyperbolicLnPluginCommand::checkPluginPreconditions();
    if(r) return r;

    auto v = StackFirstElement();
    if(v == 0.0)
        return "Infinite result";
    if(v < 0.0)
         return "Imaginary result";

    return nullptr;
}

double NaturalLog::unaryOperation(double top) const
{
    return std::log(top);
}

NaturalLog* NaturalLog::doClone() const
{
    return new NaturalLog{*this};
}

const char* NaturalLog::helpMessageImpl() const noexcept
{
    return "Replace the first element, x, on the stack with ln(x)";
}

// The double buffering of the PluginDescriptor and PluginButtonDescriptor is to maintain exception safety by
// keeping all memory allocation in RAII containers. Without regard to exception safety, this can be made
// much simpler without buffering.
class HyperbolicLnPlugin::HyperbolicLnPluginImpl
{
public:
    HyperbolicLnPluginImpl();
    ~HyperbolicLnPluginImpl();

    const PluginDescriptor& getPluginDescriptor() const { return pd_; }
    const PluginButtonDescriptor* getPluginButtonDescriptor() { return &pb_; }

private:
    void createPluginDescriptor();
    void createPluginButtonDescriptor();

    pdCalc::Plugin::PluginDescriptor pd_;
    vector<pdCalc::Command*> rawCommands_;
    vector<unique_ptr<pdCalc::Command>> commands_;
    vector<char*> rawNames_;
    vector<string> commandNames_;

    PluginButtonDescriptor pb_;
    vector<string> dispPrimaryCmd_;
    vector<string> primaryCmd_;
    vector<string> dispShftCmd_;
    vector<string> shftCmd_;
    vector<char*> rawDispP_;
    vector<char*> rawP_;
    vector<char*> rawDispS_;
    vector<char*> rawS_;
};

void HyperbolicLnPlugin::HyperbolicLnPluginImpl::createPluginDescriptor()
{
    const int n = 8;
    pd_.nCommands = n;
    commandNames_.reserve(n);
    commands_.reserve(n);

    commandNames_.emplace_back("sinh\0");
    commands_.emplace_back(new Sinh);

    commandNames_.emplace_back("cosh\0");
    commands_.emplace_back(new Cosh);

    commandNames_.emplace_back("tanh\0");
    commands_.emplace_back(new Tanh);

    commandNames_.emplace_back("arcsinh\0");
    commands_.emplace_back(new Arcsinh);

    commandNames_.emplace_back("arccosh\0");
    commands_.emplace_back(new Arccosh);

    commandNames_.emplace_back("arctanh\0");
    commands_.emplace_back(new Arctanh);

    commandNames_.emplace_back("exp\0");
    commands_.emplace_back(new Exp);

    commandNames_.emplace_back("ln\0");
    commands_.emplace_back(new NaturalLog);

    rawNames_.resize(n);
    rawCommands_.resize(n);
    for(int i = 0; i < n; ++i)
    {
        rawCommands_[i] = commands_[i].get();
        rawNames_[i] = &commandNames_[i][0];
    }

    pd_.commands = &rawCommands_[0];
    pd_.commandNames = &rawNames_[0];

    return;
}

void HyperbolicLnPlugin::HyperbolicLnPluginImpl::createPluginButtonDescriptor()
{
    const int n = 4;
    pb_.nButtons = n;
    dispPrimaryCmd_.reserve(n);
    primaryCmd_.reserve(n);
    dispShftCmd_.reserve(n);
    shftCmd_.reserve(n);

    dispPrimaryCmd_.emplace_back("sinh\0");
    primaryCmd_.emplace_back("sinh\0");
    dispShftCmd_.emplace_back("asinh\0");
    shftCmd_.emplace_back("arcsinh\0");

    dispPrimaryCmd_.emplace_back("cosh\0");
    primaryCmd_.emplace_back("cosh\0");
    dispShftCmd_.emplace_back("acosh\0");
    shftCmd_.emplace_back("arccosh\0");

    dispPrimaryCmd_.emplace_back("tanh\0");
    primaryCmd_.emplace_back("tanh\0");
    dispShftCmd_.emplace_back("atanh\0");
    shftCmd_.emplace_back("arctanh\0");

    dispPrimaryCmd_.emplace_back("ln\0");
    primaryCmd_.emplace_back("ln\0");
    dispShftCmd_.emplace_back("exp\0");
    shftCmd_.emplace_back("exp\0");

    rawDispP_.resize(n);
    rawP_.resize(n);
    rawDispS_.resize(n);
    rawS_.resize(n);
    for(int i = 0; i < n; ++i)
    {
        rawDispP_[i] = &dispPrimaryCmd_[i][0];
        rawP_[i] = &primaryCmd_[i][0];
        rawDispS_[i] =& dispShftCmd_[i][0];
        rawS_[i] = &shftCmd_[i][0];
    }

    pb_.dispPrimaryCmd = &rawDispP_[0];
    pb_.primaryCmd = &rawP_[0];
    pb_.dispShftCmd = &rawDispS_[0];
    pb_.shftCmd = &rawS_[0];

    return;
}

HyperbolicLnPlugin::HyperbolicLnPluginImpl::HyperbolicLnPluginImpl()
{
    createPluginDescriptor();
    createPluginButtonDescriptor();
}

HyperbolicLnPlugin::HyperbolicLnPluginImpl::~HyperbolicLnPluginImpl()
{ }

HyperbolicLnPlugin::HyperbolicLnPlugin()
: Plugin{}
, pimpl_{ std::make_unique<HyperbolicLnPluginImpl>()}
{ }

HyperbolicLnPlugin::~HyperbolicLnPlugin()
{ }

const pdCalc::Plugin::PluginDescriptor&HyperbolicLnPlugin::getPluginDescriptor() const
{
    return pimpl_->getPluginDescriptor();
}

const pdCalc::Plugin::PluginButtonDescriptor*HyperbolicLnPlugin::getPluginButtonDescriptor() const
{
    return pimpl_->getPluginButtonDescriptor();
}

pdCalc::Plugin::ApiVersion HyperbolicLnPlugin::apiVersion() const
{
    return {1, 0};
}

extern "C" void* AllocPlugin()
{
    return new HyperbolicLnPlugin;
}

extern "C" void DeallocPlugin(void* p)
{
    auto d = static_cast<pdCalc::Plugin*>(p);
    delete d;
}
