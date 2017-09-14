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

#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

// This class can load the plugins specified in a file.
// The file has the path and name of the plugins specified
// on separate lines.
// If a plugin cannot be loaded, the loader fails informs
// the UI but ignores the error.

#include <vector>
#include <string>
#include <memory>

namespace pdCalc {

class Plugin;
class UserInterface;

class PluginLoader
{
    class PluginLoaderImpl;
public:
    PluginLoader();
    ~PluginLoader();

    void loadPlugins(UserInterface& ui, const std::string& pluginFileName);
    const std::vector<const Plugin*> getPlugins();

private:
    PluginLoader(const PluginLoader&) = delete;
    PluginLoader(PluginLoader&&) = delete;
    PluginLoader& operator=(const PluginLoader&) = delete;
    PluginLoader& operator=(PluginLoader&&) = delete;

    std::unique_ptr<PluginLoaderImpl> pimpl_;
};

}

#endif
