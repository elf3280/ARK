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

#include "Common/AFBaseStruct.hpp"
#include "AFCTCPClient.h"

namespace ark
{

    AFCTCPClient::AFCTCPClient(const brynet::net::TcpService::PTR& service/* = nullptr*/, const brynet::net::AsyncConnector::PTR& connector/* = nullptr*/)
    {
        brynet::net::base::InitSocket();

        tcp_service_ptr_ = (service != nullptr ? service : brynet::net::TcpService::Create());
        connector_ptr_ = (connector != nullptr ? connector : brynet::net::AsyncConnector::Create());
    }

    AFCTCPClient::~AFCTCPClient()
    {
        Shutdown();
        brynet::net::base::DestroySocket();
    }

    void AFCTCPClient::Update()
    {
        ProcessMsgLogicThread();
    }

    bool AFCTCPClient::Start(const int dst_busid, const std::string& ip, const int port, bool ip_v6/* = false*/)
    {
        tcp_service_ptr_->startWorkerThread(1);
        connector_ptr_->startWorkerThread();

        brynet::net::TcpSocket::PTR socket = brynet::net::SyncConnectSocket(ip, port, ARK_CONNECT_TIMEOUT, connector_ptr_);
        if (socket == nullptr)
        {
            return false;
        }

        socket->SocketNodelay();
        auto OnEnterCallback = [ = ](const brynet::net::DataSocket::PTR & session)
        {
            AFCTCPClient* pTcpClient = this;
            AFTCPMsg* pMsg = ARK_NEW AFTCPMsg(GetHeadLength(), session);
            pMsg->conn_id_.nLow = pTcpClient->conn_id_++;
            session->setUD(static_cast<int64_t>(pMsg->conn_id_.nLow));
            pMsg->event_ = CONNECTED;

            do
            {
                AFScopeWLock xGuard(pTcpClient->rw_lock_);

                AFTCPEntityPtr pEntity = ARK_NEW AFTCPEntity(pTcpClient, pMsg->conn_id_, session);
                pTcpClient->client_entity_ptr_.reset(pEntity);
                pEntity->msg_queue_.Push(pMsg);
            } while (false);

            //data cb
            session->setDataCallback([pTcpClient, session](const char* buffer, size_t len)
            {
                const auto ud = brynet::net::cast<int64_t>(session->getUD());

                AFScopeRLock xGuard(pTcpClient->rw_lock_);
                if (pTcpClient->client_entity_ptr_->GetConnID() == *ud)
                {
                    pTcpClient->client_entity_ptr_->AddBuff(buffer, len);
                    pTcpClient->DismantleNet(pTcpClient->client_entity_ptr_.get());
                }

                return len;
            });

            //disconnect cb
            session->setDisConnectCallback([pTcpClient](const brynet::net::DataSocket::PTR & session)
            {
                const auto ud = brynet::net::cast<int64_t>(session->getUD());
                AFGUID conn_id(0, *ud);

                AFTCPMsg* pMsg = ARK_NEW AFTCPMsg(pTcpClient->GetHeadLength(), session);
                pMsg->conn_id_ = conn_id;
                pMsg->event_ = DISCONNECTED;

                do
                {
                    AFScopeWLock xGuard(pTcpClient->rw_lock_);
                    pTcpClient->client_entity_ptr_->msg_queue_.Push(pMsg);
                } while (false);
            });
        };

        tcp_service_ptr_->addDataSocket(std::move(socket),
                                        brynet::net::TcpService::AddSocketOption::WithEnterCallback(OnEnterCallback),
                                        brynet::net::TcpService::AddSocketOption::WithMaxRecvBufferSize(ARK_TCP_RECV_BUFFER_SIZE));

        dst_bus_id_ = dst_busid;
        SetWorking(true);
        return true;
    }

    bool AFCTCPClient::Shutdown()
    {
        if (!CloseSocketAll())
        {
            //add log
        }

        connector_ptr_->stopWorkerThread();
        tcp_service_ptr_->stopWorkerThread();
        SetWorking(false);
        return true;
    }

    bool AFCTCPClient::CloseSocketAll()
    {
        if (nullptr != client_entity_ptr_)
        {
            client_entity_ptr_->GetSession()->postDisConnect();
        }

        return true;
    }

    bool AFCTCPClient::SendMsg(const char* msg, const size_t msg_len, const AFGUID& conn_id/* = 0*/)
    {
        if (nullptr != client_entity_ptr_ && client_entity_ptr_->GetSession())
        {
            client_entity_ptr_->GetSession()->send(msg, msg_len);
        }

        return true;
    }

    bool AFCTCPClient::CloseNetEntity(const AFGUID& conn_id)
    {
        if (nullptr != client_entity_ptr_ && client_entity_ptr_->GetConnID() == conn_id)
        {
            client_entity_ptr_->GetSession()->postDisConnect();
        }

        return true;
    }

    bool AFCTCPClient::DismantleNet(AFTCPEntityPtr entity_ptr)
    {
        while (entity_ptr->GetBuffLen() >= GetHeadLength())
        {
            AFCSMsgHead head;
            int msg_body_len = DeCode(entity_ptr->GetBuff(), entity_ptr->GetBuffLen(), head);

            if (msg_body_len >= 0 && head.msg_id() > 0)
            {
                AFTCPMsg* pMsg = ARK_NEW AFTCPMsg(entity_ptr->GetSession());
                pMsg->head_ = head;
                pMsg->event_ = RECV_DATA;
                pMsg->msg_data_.append(entity_ptr->GetBuff() + GetHeadLength(), msg_body_len);
                entity_ptr->msg_queue_.Push(pMsg);
                entity_ptr->RemoveBuff(msg_body_len + GetHeadLength());
            }
            else
            {
                break;
            }
        }

        return true;
    }

    void AFCTCPClient::ProcessMsgLogicThread()
    {
        do
        {
            AFScopeRLock xGuard(rw_lock_);
            ProcessMsgLogicThread(client_entity_ptr_.get());
        } while (false);

        if (client_entity_ptr_ != nullptr && client_entity_ptr_->NeedRemove())
        {
            AFScopeWLock xGuard(rw_lock_);
            client_entity_ptr_.reset(nullptr);
        }

    }

    void AFCTCPClient::ProcessMsgLogicThread(AFTCPEntityPtr entity_ptr)
    {
        if (entity_ptr == nullptr)
        {
            return;
        }

        size_t nReceiveCount = entity_ptr->msg_queue_.Count();

        for (size_t i = 0; i < nReceiveCount; ++i)
        {
            AFTCPMsg* pMsg(nullptr);

            if (!entity_ptr->msg_queue_.Pop(pMsg))
            {
                break;
            }

            if (pMsg == nullptr)
            {
                continue;
            }

            switch (pMsg->event_)
            {
            case RECV_DATA:
                {
                    if (net_recv_cb_)
                    {
                        net_recv_cb_(pMsg->head_, pMsg->head_.msg_id(), pMsg->msg_data_.c_str(), pMsg->msg_data_.size(), entity_ptr->GetConnID());
                    }
                }
                break;
            case CONNECTED:
                net_event_cb_(pMsg->event_, pMsg->conn_id_, pMsg->session_->getIP(), dst_bus_id_);
                break;
            case DISCONNECTED:
                {
                    net_event_cb_(pMsg->event_, pMsg->conn_id_, pMsg->session_->getIP(), dst_bus_id_);
                    entity_ptr->SetNeedRemove(true);
                }
                break;

            default:
                break;
            }

            delete pMsg;
        }
    }

    bool AFCTCPClient::IsServer()
    {
        return false;
    }

    bool AFCTCPClient::Log(int severity, const char* msg)
    {
        //Will add log
        return true;
    }

    bool AFCTCPClient::SendRawMsg(const uint16_t msg_id, const char* msg, const size_t msg_len, const AFGUID& conn_id/* = 0*/, const AFGUID& actor_id/* = 0*/)
    {
        AFCSMsgHead head;
        head.set_msg_id(msg_id);
        head.set_uid(actor_id);
        head.set_body_length(msg_len);

        std::string out_data;
        size_t whole_len = EnCode(head, msg, msg_len, out_data);
        if (whole_len == msg_len + GetHeadLength())
        {
            return SendMsg(out_data.c_str(), out_data.length(), conn_id);
        }
        else
        {
            return false;
        }
    }

}