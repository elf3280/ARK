﻿/*
* This source file is part of ARK
* For the latest info, see https://github.com/QuadHex
*
* Copyright (c) 2013-2018 QuadHex authors.
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

#pragma once

#include "Common/AFProtoCPP.hpp"
#include "SDK/Interface/AFILogModule.h"
#include "SDK/Interface/AFIKernelModule.h"
#include "SDK/Interface/AFIClassModule.h"
#include "SDK/Interface/AFIConfigModule.h"
#include "SDK/Interface/AFIPluginManager.h"
#include "SDK/Interface/AFIGUIDModule.h"
#include "SDK/Interface/AFIBusModule.h"
#include "SDK/Interface/AFIMsgModule.h"
#include "SDK/Interface/AFINetServiceManagerModule.h"
#include "Server/Interface/AFISceneProcessModule.h"
#include "Server/Interface/AFIAccountModule.h"
#include "Server/Interface/AFIGameNetModule.h"

namespace ark
{

    class AFCGameNetModule : public AFIGameNetModule
    {
    public:
        explicit AFCGameNetModule() = default;

        bool Init() override;
        bool PostInit() override;
        bool PreUpdate() override;

        virtual void SendMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message& xMsg, const AFGUID& self);
        virtual void SendMsgPBToGate(const uint16_t nMsgID, const std::string& strMsg, const AFGUID& self);
        virtual AFINetServerService* GetNetServerService();

        virtual bool AddPlayerGateInfo(const AFGUID& nRoleID, const AFGUID& nClientID, const int nGateID);
        virtual bool RemovePlayerGateInfo(const AFGUID& nRoleID);
        virtual ARK_SHARE_PTR<GateBaseInfo> GetPlayerGateInfo(const AFGUID& nRoleID);

        virtual ARK_SHARE_PTR<GateServerInfo> GetGateServerInfo(const int nGateID);
        virtual ARK_SHARE_PTR<GateServerInfo> GetGateServerInfoByClientID(const AFGUID& nClientID);

        virtual int OnViewDataNodeEnter(const AFIDataList& argVar, const AFGUID& self);
        virtual int OnSelfDataNodeEnter(const AFGUID& self);
        virtual int OnViewDataTableEnter(const AFIDataList& argVar, const AFGUID& self);
        virtual int OnSelfDataTableEnter(const AFGUID& self);

        virtual int OnEntityListEnter(const AFIDataList& self, const AFIDataList& argVar);
        virtual int OnEntityListLeave(const AFIDataList& self, const AFIDataList& argVar);

        void OnSocketEvent(const NetEventType event, const AFGUID& conn_id, const std::string& ip, const int bus_id);
        void OnClientDisconnect(const AFGUID& xClientID);
        void OnClientConnected(const AFGUID& xClientID);

        void OnProxyServerRegisteredProcess(const ARK_PKG_BASE_HEAD& head, const int msg_id, const char* msg, const uint32_t msg_len, const AFGUID& conn_id);
        void OnProxyServerUnRegisteredProcess(const ARK_PKG_BASE_HEAD& head, const int msg_id, const char* msg, const uint32_t msg_len, const AFGUID& conn_id);
        void OnRefreshProxyServerInfoProcess(const ARK_PKG_BASE_HEAD& head, const int msg_id, const char* msg, const uint32_t msg_len, const AFGUID& conn_id);
        void OnReqiureRoleListProcess(const ARK_PKG_BASE_HEAD& head, const int msg_id, const char* msg, const uint32_t msg_len, const AFGUID& conn_id);
        void OnCreateRoleGameProcess(const ARK_PKG_BASE_HEAD& head, const int msg_id, const char* msg, const uint32_t msg_len, const AFGUID& conn_id);
        void OnDeleteRoleGameProcess(const ARK_PKG_BASE_HEAD& head, const int msg_id, const char* msg, const uint32_t msg_len, const AFGUID& conn_id);

        void OnClienEnterGameProcess(const ARK_PKG_BASE_HEAD& head, const int msg_id, const char* msg, const uint32_t msg_len, const AFGUID& conn_id);
        void OnClientLeaveGameProcess(const ARK_PKG_BASE_HEAD& head, const int msg_id, const char* msg, const uint32_t msg_len, const AFGUID& conn_id);
        void OnClienSwapSceneProcess(const ARK_PKG_BASE_HEAD& head, const int msg_id, const char* msg, const uint32_t msg_len, const AFGUID& conn_id);


        ///////////WORLD_START///////////////////////////////////////////////////////////////
        void OnTransWorld(const ARK_PKG_BASE_HEAD& head, const int msg_id, const char* msg, const uint32_t msg_len, const AFGUID& conn_id);

        //Common Node & table & class event
        int OnCommonDataNodeEvent(const AFGUID& self, const std::string& strPropertyName, const AFIData& oldVar, const AFIData& newVar);
        int OnCommonDataTableEvent(const AFGUID& self, const DATA_TABLE_EVENT_DATA& xEventData, const AFIData& oldVar, const AFIData& newVar);
        int OnCommonClassEvent(const AFGUID& self, const std::string& strClassName, const ARK_ENTITY_EVENT eClassEvent, const AFIDataList& var);

        int OnGroupEvent(const AFGUID& self, const std::string& strPropertyName, const AFIData& oldVar, const AFIData& newVar);
        int OnContainerEvent(const AFGUID& self, const std::string& strPropertyName, const AFIData& oldVar, const AFIData& newVar);

        int OnEntityEvent(const AFGUID& self, const std::string& strClassName, const ARK_ENTITY_EVENT eClassEvent, const AFIDataList& var);
        int OnSwapSceneResultEvent(const AFGUID& self, const int nEventID, const AFIDataList& var);

        int GetNodeBroadcastEntityList(const AFGUID& self, const std::string& strPropertyName, AFIDataList& valueObject);
        int GetTableBroadcastEntityList(const AFGUID& self, const std::string& strPropertyName, AFIDataList& valueObject);
        int GetBroadcastEntityList(const int nObjectContainerID, const int nGroupID, AFIDataList& valueObject);

        bool ProcessLeaveGroup(const AFGUID& self, int nSceneID, int nOldGroupID);
        bool ProcessEnterGroup(const AFGUID& self, int nSceneID, int nNewGroupID);

    protected:
        int StartServer();
        int StartClient();

        void CommonDataTableAddEvent(const AFGUID& self, const std::string& strTableName, int nRow, int nCol, const AFCDataList& valueBroadCaseList);
        void CommonDataTableDeleteEvent(const AFGUID& self, const std::string& strTableName, int nRow, const AFCDataList& valueBroadCaseList);
        void CommonDataTableSwapEvent(const AFGUID& self, const std::string& strTableName, int nRow, int target_row, const AFCDataList& valueBroadCaseList);
        void CommonDataTableUpdateEvent(const AFGUID& self, const std::string& strTableName, int nRow, int nCol, const AFIData& newVar, const AFCDataList& valueBroadCaseList);

        int CommonClassDestoryEvent(const AFGUID& self);

    private:
        //<角色id,角色网关基础信息>//其实可以在object系统中被代替
        AFMapEx<AFGUID, GateBaseInfo> mRoleBaseData;
        //gate id,data
        AFMapEx<int, GateServerInfo> mProxyMap;
        //////////////////////////////////////////////////////////////////////////
        AFIGUIDModule* m_pUUIDModule;
        AFIKernelModule* m_pKernelModule;
        AFIClassModule* m_pClassModule;
        AFILogModule* m_pLogModule;
        AFIConfigModule* m_pConfigModule;
        AFIBusModule* m_pBusModule;
        AFINetServiceManagerModule* m_pNetServiceManagerModule;
        //////////////////////////////////////////////////////////////////////////


        AFISceneProcessModule* m_pSceneProcessModule;
        AFIAccountModule* m_AccountModule;

        AFINetServerService* m_pNetServerService;
        AFIMsgModule* m_pMsgModule;
    };

}