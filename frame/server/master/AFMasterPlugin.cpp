/*
* This source file is part of ARK
* For the latest info, see https://github.com/QuadHex
*
* Copyright (c) 2013-2019 QuadHex authors.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#include "AFMasterPlugin.h"
#include "AFCMasterNetModule.h"

namespace ark
{

    ARK_DLL_PLUGIN_ENTRY(AFMasterPlugin)
    ARK_DLL_PLUGIN_EXIT(AFMasterPlugin)

    //////////////////////////////////////////////////////////////////////////
    int AFMasterPlugin::GetPluginVersion()
    {
        return 0;
    }

    const std::string AFMasterPlugin::GetPluginName()
    {
        return GET_CLASS_NAME(AFMasterPlugin)
    }

    void AFMasterPlugin::Install()
    {
        RegisterModule<AFIMasterNetModule, AFCMasterNetModule>();
    }

    void AFMasterPlugin::Uninstall()
    {
        DeregisterModule<AFIMasterNetModule, AFCMasterNetModule>();
    }

}