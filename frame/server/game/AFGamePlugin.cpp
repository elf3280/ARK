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

#include "AFGamePlugin.h"
#include "AFCSceneProcessModule.h"
#include "AFCPropertyModule.h"
#include "AFCLevelModule.h"
#include "AFCPropertyConfigModule.h"
#include "AFCAccountModule.h"
#include "AFCGameNetModule.h"

namespace ark
{

    ARK_DLL_PLUGIN_ENTRY(AFGamePlugin)
    ARK_DLL_PLUGIN_EXIT(AFGamePlugin)

    //////////////////////////////////////////////////////////////////////////
    int AFGamePlugin::GetPluginVersion()
    {
        return 0;
    }

    const std::string AFGamePlugin::GetPluginName()
    {
        return GET_CLASS_NAME(AFGamePlugin)
    }

    void AFGamePlugin::Install()
    {
        RegisterModule<AFISceneProcessModule, AFCSceneProcessModule>();
        RegisterModule<AFIPropertyModule, AFCPropertyModule>();
        RegisterModule<AFILevelModule, AFCLevelModule>();
        RegisterModule<AFIPropertyConfigModule, AFCPropertyConfigModule>();
        RegisterModule<AFIAccountModule, AFCAccountModule>();
        RegisterModule<AFIGameNetModule, AFCGameNetModule>();
    }

    void AFGamePlugin::Uninstall()
    {
        DeregisterModule<AFIGameNetModule, AFCGameNetModule>();
        DeregisterModule<AFIAccountModule, AFCAccountModule>();
        DeregisterModule<AFIPropertyConfigModule, AFCPropertyConfigModule>();
        DeregisterModule<AFILevelModule, AFCLevelModule>();
        DeregisterModule<AFIPropertyModule, AFCPropertyModule>();
        DeregisterModule<AFISceneProcessModule, AFCSceneProcessModule>();
    }

}