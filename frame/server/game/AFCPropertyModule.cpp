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

#include "base/AFDateTime.hpp"
#include "AFCPropertyModule.h"

namespace ark
{

    bool AFCPropertyModule::Init()
    {
        m_pKernelModule = pPluginManager->FindModule<AFIKernelModule>();
        m_pConfigModule = pPluginManager->FindModule<AFIConfigModule>();
        m_pClassModule = pPluginManager->FindModule<AFIMetaClassModule>();
        m_pPropertyConfigModule = pPluginManager->FindModule<AFIPropertyConfigModule>();
        m_pLevelModule = pPluginManager->FindModule<AFILevelModule>();

        m_pKernelModule->AddClassCallBack(AFEntityMetaPlayer::self_name(), this, &AFCPropertyModule::OnObjectClassEvent);
        m_pClassModule->AddNodeCallBack(AFEntityMetaPlayer::self_name(), AFEntityMetaPlayer::level(), this, &AFCPropertyModule::OnObjectLevelEvent);
        //m_pClassModule->AddTableCallBack(AFEntityMetaPlayer::self_name(), ark::Player::R_CommPropertyValue(), this, &AFCPropertyModule::OnPropertyTableEvent);

        return true;
    }

    int AFCPropertyModule::GetPropertyValue(const AFGUID& self, const std::string& attr_name, const ArkPropertyGroup eGroupType)
    {
        if (ArkPropertyGroup::APG_ALL != eGroupType)
        {
            //return m_pKernelModule->GetTableInt(self, ark::Player::R_CommPropertyValue(), eGroupType, PropertyNameToCol(strPropertyName));
        }

        return m_pKernelModule->GetNodeInt(self, attr_name);
    }

    int AFCPropertyModule::SetPropertyValue(const AFGUID& self, const std::string& strPropertyName, const ArkPropertyGroup eGroupType, const int32_t nValue)
    {
        //if (ArkPropertyGroup::APG_ALL != eGroupType)
        //{
        //    ARK_SHARE_PTR<AFIEntity> pEntity = m_pKernelModule->GetEntity(self);

        //    if (pEntity != nullptr)
        //    {
        //        AFDataTable* pTable = m_pKernelModule->FindTable(self, ark::Player::R_CommPropertyValue());

        //        if (pTable != nullptr)
        //        {
        //            return pTable->SetInt(eGroupType, PropertyNameToCol(strPropertyName), nValue);
        //        }
        //    }
        //}

        ////动态表中没有，则设置到最终值
        //m_pKernelModule->SetNodeInt(self, strPropertyName, nValue);

        return 0;
    }


    int AFCPropertyModule::AddPropertyValue(const AFGUID& self, const std::string& strPropertyName, const ArkPropertyGroup eGroupType, const int32_t nValue)
    {
        //if (ArkPropertyGroup::APG_ALL != eGroupType)
        //{
        //    ARK_SHARE_PTR<AFIEntity> pEntity = m_pKernelModule->GetEntity(self);

        //    if (nullptr != pEntity)
        //    {
        //        AFDataTable* pTable = m_pKernelModule->FindTable(self, ark::Player::R_CommPropertyValue());

        //        if (nullptr != pTable)
        //        {
        //            int nPropertyValue = pTable->GetInt(eGroupType, PropertyNameToCol(strPropertyName));
        //            return pTable->SetInt(eGroupType, PropertyNameToCol(strPropertyName), nPropertyValue + nValue);
        //        }
        //    }
        //}

        return 0;
    }

    int AFCPropertyModule::SubPropertyValue(const AFGUID& self, const std::string& strPropertyName, const ArkPropertyGroup eGroupType, const int32_t nValue)
    {
        //if (ArkPropertyGroup::APG_ALL != eGroupType)
        //{
        //    ARK_SHARE_PTR<AFIEntity> pEntity = m_pKernelModule->GetEntity(self);

        //    if (nullptr != pEntity)
        //    {
        //        AFDataTable* pTable = m_pKernelModule->FindTable(self, ark::Player::R_CommPropertyValue());

        //        if (nullptr != pTable)
        //        {
        //            int nPropertyValue = pTable->GetInt(eGroupType, PropertyNameToCol(strPropertyName));

        //            return pTable->SetInt(eGroupType, PropertyNameToCol(strPropertyName), nPropertyValue - nValue);
        //        }
        //    }
        //}

        return 0;
    }

    int AFCPropertyModule::OnObjectLevelEvent(const AFGUID& self, const std::string& strPropertyName, const AFIData& oldVar, const AFIData& newVar)
    {
        RefreshBaseProperty(self);

        FullHPMP(self);

        return 0;
    }

    int AFCPropertyModule::OnPropertyTableEvent(const AFGUID& self, const DATA_TABLE_EVENT_DATA& xEventData, const AFIData& oldVar, const AFIData& newVar)
    {
        ////计算总值
        //const int nOpType = xEventData.nOpType;
        //const int nRow = xEventData.nRow;
        //const int nCol = xEventData.nCol;

        //int nAllValue = 0;
        //AFDataTable* pTable = m_pKernelModule->FindTable(self, ark::Player::R_CommPropertyValue());

        //for (size_t i = 0; i < (size_t)(AFIPropertyModule::APG_ALL) && i < pTable->GetRowCount(); ++i)
        //{
        //    int nValue = pTable->GetInt(i, nCol);
        //    nAllValue += nValue;
        //}

        //m_pKernelModule->SetNodeInt(self, ColToPropertyName(nCol), nAllValue);

        return 0;
    }

    const std::string& AFCPropertyModule::ColToPropertyName(const size_t nCol)
    {
        if (nCol > mColToName.size())
        {
            return NULL_STR;
        }

        return mColToName[nCol];
    }

    int AFCPropertyModule::PropertyNameToCol(const std::string& strPropertyName)
    {
        auto iter = mNameToCol.find(strPropertyName);

        if (iter != mNameToCol.end())
        {
            return iter->second;
        }

        return -1;
    }

    int AFCPropertyModule::OnObjectClassEvent(const AFGUID& self, const std::string& strClassName, const ArkEntityEvent eClassEvent, const AFIDataList& var)
    {
        //if (ArkEntityEvent::ENTITY_EVT_PRE_LOAD_DATA == eClassEvent)
        //{
        //    AFDataTable* pTable = m_pKernelModule->FindTable(self, ark::Player::R_CommPropertyValue());

        //    if (nullptr != pTable)
        //    {
        //        for (int i = 0; i < APG_ALL; i++)
        //        {
        //            pTable->AddRow();
        //        }
        //    }

        //}
        //else if (ArkEntityEvent::ENTITY_EVT_EFFECT_DATA == eClassEvent)
        //{
        //    int nOnlineCount = m_pKernelModule->GetNodeInt(self, ark::Player::OnlineCount());

        //    if (nOnlineCount <= 0 && m_pKernelModule->GetNodeInt(self, ark::Player::MapID()) > 0)
        //    {
        //        //第一次出生，设置基础属性
        //        m_pKernelModule->SetNodeInt(self, ark::Player::Level(), 1);
        //    }
        //}
        //else if (ArkEntityEvent::ENTITY_EVT_ALL_FINISHED == eClassEvent)
        //{
        //    int nOnlineCount = m_pKernelModule->GetNodeInt(self, ark::Player::OnlineCount());
        //    m_pKernelModule->SetNodeInt(self, ark::Player::OnlineCount(), (nOnlineCount + 1));
        //}

        return 0;
    }

    int AFCPropertyModule::RefreshBaseProperty(const AFGUID& self)
    {
        //AFDataTable* pTable = m_pKernelModule->FindTable(self, ark::Player::R_CommPropertyValue());

        //if (nullptr == pTable)
        //{
        //    return 1;
        //}

        ////初始属性+等级属性(职业决定)
        //int eJobType = m_pKernelModule->GetNodeInt(self, ark::Player::Career());
        //int nLevel = m_pKernelModule->GetNodeInt(self, ark::Player::Level());

        //for (size_t i = 0; i < pTable->GetColCount(); ++i)
        //{
        //    const std::string& strPropertyName = ColToPropertyName(i);
        //    int nValue = m_pPropertyConfigModule->CalculateBaseValue(eJobType, nLevel, strPropertyName);
        //    SetPropertyValue(self, strPropertyName, ArkPropertyGroup::APG_JOBLEVEL, nValue);
        //}

        return 0;
    }

    bool AFCPropertyModule::FullHPMP(const AFGUID& self)
    {
        //int32_t nMaxHP = m_pKernelModule->GetNodeInt(self, ark::Player::HP());

        //if (nMaxHP > 0)
        //{
        //    m_pKernelModule->SetNodeInt(self, ark::Player::HP(), nMaxHP);
        //}

        //int32_t nMaxMP = m_pKernelModule->GetNodeInt(self, ark::Player::MP());

        //if (nMaxMP > 0)
        //{
        //    m_pKernelModule->SetNodeInt(self, ark::Player::MP(), nMaxMP);
        //}

        return true;
    }

    bool AFCPropertyModule::AddHP(const AFGUID& self, const int32_t& nValue)
    {
        //if (nValue <= 0)
        //{
        //    return false;
        //}

        //int32_t nCurValue = m_pKernelModule->GetNodeInt(self, AFEntityMetaPlayer::hp());
        //int32_t nMaxValue = m_pKernelModule->GetNodeInt(self, AFEntityMetaPlayer::HP());

        //if (nCurValue > 0)
        //{
        //    nCurValue += nValue;

        //    if (nCurValue > nMaxValue)
        //    {
        //        nCurValue = nMaxValue;
        //    }

        //    m_pKernelModule->SetNodeInt(self, ark::Player::HP(), nCurValue);
        //}

        return true;
    }

    bool AFCPropertyModule::EnoughHP(const AFGUID& self, const int32_t& nValue)
    {
        //int32_t nCurValue = m_pKernelModule->GetNodeInt(self, ark::Player::HP());

        //if ((nCurValue > 0) && (nCurValue - nValue >= 0))
        //{
        //    return true;
        //}

        return false;
    }

    bool AFCPropertyModule::ConsumeHP(const AFGUID& self, const int32_t& nValue)
    {
        //int32_t nCurValue = m_pKernelModule->GetNodeInt(self, ark::Player::HP());

        //if ((nCurValue > 0) && (nCurValue - nValue >= 0))
        //{
        //    nCurValue -= nValue;
        //    m_pKernelModule->SetNodeInt(self, ark::Player::HP(), nCurValue);

        //    return true;
        //}

        return false;
    }

    bool AFCPropertyModule::AddMP(const AFGUID& self, const int32_t& nValue)
    {
        //if (nValue <= 0)
        //{
        //    return false;
        //}

        //int32_t nCurValue = m_pKernelModule->GetNodeInt(self, ark::Player::MP());
        //int32_t nMaxValue = m_pKernelModule->GetNodeInt(self, ark::Player::MP());

        //nCurValue += nValue;

        //if (nCurValue > nMaxValue)
        //{
        //    nCurValue = nMaxValue;
        //}

        //m_pKernelModule->SetNodeInt(self, ark::Player::MP(), nCurValue);

        return true;
    }

    bool AFCPropertyModule::ConsumeMP(const AFGUID& self, const int32_t& nValue)
    {
        //int32_t nCurValue = m_pKernelModule->GetNodeInt(self, ark::Player::MP());

        //if ((nCurValue > 0) && (nCurValue - nValue >= 0))
        //{
        //    nCurValue -= nValue;
        //    m_pKernelModule->SetNodeInt(self, ark::Player::MP(), nCurValue);

        //    return true;
        //}

        return false;
    }

    bool AFCPropertyModule::EnoughMP(const AFGUID& self, const int32_t& nValue)
    {
        //int32_t nCurValue = m_pKernelModule->GetNodeInt(self, ark::Player::MP());

        //if ((nCurValue > 0) && (nCurValue - nValue >= 0))
        //{
        //    return true;
        //}

        return false;
    }

    bool AFCPropertyModule::AddMoney(const AFGUID& self, const int32_t& nValue)
    {
        if (nValue <= 0)
        {
            return false;
        }

        //int32_t nCurValue = m_pKernelModule->GetNodeInt(self, ark::Player::Gold());
        //nCurValue += nValue;
        //m_pKernelModule->SetNodeInt(self, ark::Player::Gold(), nCurValue);

        return false;
    }

    bool AFCPropertyModule::ConsumeMoney(const AFGUID& self, const int32_t& nValue)
    {
        if (nValue <= 0)
        {
            return false;
        }

        //int32_t nCurValue = m_pKernelModule->GetNodeInt(self, ark::Player::Gold());
        //nCurValue -= nValue;

        //if (nCurValue >= 0)
        //{
        //    m_pKernelModule->SetNodeInt(self, ark::Player::Gold(), nCurValue);

        //    return true;
        //}

        return false;
    }

    bool AFCPropertyModule::EnoughMoney(const AFGUID& self, const int32_t& nValue)
    {
        //int32_t nCurValue = m_pKernelModule->GetNodeInt(self, ark::Player::Gold());

        //if ((nCurValue > 0) && (nCurValue - nValue >= 0))
        //{
        //    return true;
        //}

        return false;
    }

    bool AFCPropertyModule::AddDiamond(const AFGUID& self, const int32_t& nValue)
    {
        //if (nValue <= 0)
        //{
        //    return false;
        //}

        //int32_t nCurValue = m_pKernelModule->GetNodeInt(self, ark::Player::Money());
        //nCurValue += nValue;
        //m_pKernelModule->SetNodeInt(self, ark::Player::Money(), nCurValue);

        return false;
    }

    bool AFCPropertyModule::ConsumeDiamond(const AFGUID& self, const int32_t& nValue)
    {
        //if (nValue <= 0)
        //{
        //    return false;
        //}

        //int32_t nCurValue = m_pKernelModule->GetNodeInt(self, ark::Player::Money());
        //nCurValue -= nValue;

        //if (nCurValue >= 0)
        //{
        //    m_pKernelModule->SetNodeInt(self, ark::Player::Money(), nCurValue);

        //    return true;
        //}

        return false;
    }

    bool AFCPropertyModule::EnoughDiamond(const AFGUID& self, const int32_t& nValue)
    {
        //int32_t nCurValue = m_pKernelModule->GetNodeInt(self, ark::Player::Money());
        //if ((nCurValue > 0) && (nCurValue - nValue >= 0))
        //{
        //    return true;
        //}

        return false;
    }

}