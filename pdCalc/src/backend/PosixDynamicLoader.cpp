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

#include "PosixDynamicLoader.h"
#include "utilities/Exception.h"
#include <dlfcn.h>
#include <iostream>
#include <iostream>

using std::cout;
using std::endl;

namespace pdCalc {

PosixDynamicLoader::PosixDynamicLoader()
: DynamicLoader{}
, handle_{nullptr}
{
}

PosixDynamicLoader::~PosixDynamicLoader()
{
    if(handle_) dlclose(handle_);
}

Plugin* PosixDynamicLoader::allocatePlugin(const std::string& pluginName)
{
    handle_ = dlopen(pluginName.c_str(), RTLD_LAZY);

    if(!handle_) return nullptr;
    else
    {
        auto alloc = dlsym(handle_, GetPluginAllocationName().c_str());
        PluginAllocator allocator{ reinterpret_cast<PluginAllocator>(alloc) };
        if(allocator)
        {
            auto p = static_cast<Plugin*>((*allocator)());
            return p;
        }
        else return nullptr;
    }

    return nullptr;
}

void PosixDynamicLoader::deallocatePlugin(Plugin* p)
{
    if(!handle_)
        throw Exception("Trying to deallocate a plugin, but shared library is not open.");
    else
    {
        auto dealloc = dlsym(handle_, GetPluginDeallocationName().c_str());
        auto deallocator = reinterpret_cast<PluginDeallocator>(dealloc);
        if(deallocator)
        {
            (*deallocator)(p);
        }
        else throw Exception("Could not load the deallocator function in the plugin");
    }

    return;
}

}
