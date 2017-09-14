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

#include "PluginLoader.h"
#include "utilities/UserInterface.h"
#include <fstream>
#include <iterator>
#include <algorithm>
#include <memory>
#include "DynamicLoader.h"
#include "Plugin.h"
#include "PlatformFactory.h"

using std::vector;
using std::string;
using std::ifstream;
using std::unique_ptr;

namespace pdCalc {

class PluginDeleter
{
public:
    explicit PluginDeleter(DynamicLoader& d) : loader_{d} {}
    void operator()(Plugin* p) { loader_.deallocatePlugin(p); }

private:
    DynamicLoader& loader_;
};

class PluginLoader::PluginLoaderImpl
{
public:
    PluginLoaderImpl();

    void loadPlugins(UserInterface& ui, const string& pluginFileName);
    const vector<const Plugin *> getPlugins();

private:
    void load(UserInterface& ui, const string&);

    vector<unique_ptr<DynamicLoader>> loaders_;
    vector<unique_ptr<Plugin, PluginDeleter>> plugins_;
};

PluginLoader::PluginLoaderImpl::PluginLoaderImpl()
{
}

void PluginLoader::PluginLoaderImpl::loadPlugins(UserInterface& ui, const string& pluginFileName)
{
    ifstream ifs{ pluginFileName.c_str() };
    if(!ifs)
    {
        ui.postMessage("Could not open plugin file");
    }
    else
    {
        vector<string> pluginNames{ std::istream_iterator<string>(ifs), std::istream_iterator<string>() };

        for(auto i : pluginNames) load(ui, i);
    }

    return;
}

const vector<const Plugin*> PluginLoader::PluginLoaderImpl::getPlugins()
{
    vector<const Plugin*> v;
    for(auto& i : plugins_)
        v.push_back( i.get() );

    return v;
}

void PluginLoader::PluginLoaderImpl::load(UserInterface& ui, const string& name)
{
    loaders_.emplace_back( PlatformFactory::Instance().createDynamicLoader() );

    // may be null
    auto p = loaders_.back()->allocatePlugin(name);
    if(p) plugins_.emplace_back( p, PluginDeleter( *loaders_.back() ) );
    else ui.postMessage("Error opening plugin");

    return;
}

PluginLoader::PluginLoader()
: pimpl_{ new PluginLoaderImpl }
{ }

PluginLoader::~PluginLoader()
{ }

void PluginLoader::loadPlugins(UserInterface& ui, const string& pluginFileName)
{
    pimpl_->loadPlugins(ui, pluginFileName);
    return;
}

const std::vector<const Plugin*> PluginLoader::getPlugins()
{
    return pimpl_->getPlugins();
}

}

