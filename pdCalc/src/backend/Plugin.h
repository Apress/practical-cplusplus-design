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

#ifndef PLUGIN_H
#define PLUGIN_H

// This is the base class to extend pdCalc with plugin commands. The plugin
// must provide a command with its name (for the command structure), which
// automatically provides a CLI interface. Optionally, a plugin can provide
// a GUI interface.

#include "Command.h"

namespace pdCalc {

class CommandButton;

// Plugins are responsible for deleting the memory in the descriptors
class Plugin
{
public:
    Plugin() { }
    virtual ~Plugin() { }

    struct PluginDescriptor
    {
        int nCommands;
        char** commandNames;
        Command** commands;
    };

    virtual const PluginDescriptor& getPluginDescriptor() const = 0;

    struct PluginButtonDescriptor
    {
        int nButtons;
        char** dispPrimaryCmd;
        char** primaryCmd;
        char** dispShftCmd;
        char** shftCmd;
    };

    // pointer instead of reference so that nullptr can be returned if buttons are not provided
    virtual const PluginButtonDescriptor* getPluginButtonDescriptor() const = 0;

    struct ApiVersion
    {
        int major;
        int minor;
    };

    virtual ApiVersion apiVersion() const = 0;

private:
    Plugin(const Plugin&) = delete;
    Plugin& operator=(const Plugin&) = delete;
    Plugin(Plugin&&) = delete;
    Plugin& operator=(Plugin&&) = delete;
};

}
#endif
