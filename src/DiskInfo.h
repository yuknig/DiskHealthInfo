#pragma once
#include "IPropertyCollection.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

struct DiskHealthInfo;

// partial mapping of MSFT_Disk 
// https://docs.microsoft.com/en-us/previous-versions/windows/desktop/stormgmt/msft-disk
struct DiskInfo
{
    static std::unique_ptr<DiskInfo> CreateFromProperties(const IPropertyCollection& p);

    std::map<std::wstring, std::wstring> Properties() const;

    std::wstring m_friendlyName;
    std::wstring m_number;
    std::wstring m_serialNumber;
    std::wstring m_model;
    std::wstring m_size;
    std::wstring m_numberOfPartitions;
    std::wstring m_busType;
    std::wstring m_isBoot;
    std::wstring m_healthStatus;
    std::unique_ptr<DiskHealthInfo> m_healthInfo;
};

// partial mapping from MSFT_StorageReliabilityCounter
// https://docs.microsoft.com/en-us/previous-versions/windows/desktop/stormgmt/msft-storagereliabilitycounter
struct DiskHealthInfo
{
    static std::unique_ptr<DiskHealthInfo> CreateFromProperties(const IPropertyCollection& p);

    std::map<std::wstring, std::wstring> Properties() const;

    std::wstring m_temperature;
    std::wstring m_temperatureMax;
    std::wstring m_readErrorsTotal;
    std::wstring m_readErrorsCorrected;
    std::wstring m_readErrorsUncorrected;
    std::wstring m_writeErrorsTotal;
    std::wstring m_writeErrorsCorrected;
    std::wstring m_writeErrorsUncorrected;
    std::wstring m_manufactureDate;
    std::wstring m_startStopCycleCount;
    std::wstring m_startStopCycleCountMax;
    std::wstring m_loadUnloadCycleCount;
    std::wstring m_loadUnloadCycleCountMax;
    std::wstring m_wear;
    std::wstring m_powerOnHours;
    std::wstring m_readLatencyMax;
    std::wstring m_writeLatencyMax;
    std::wstring m_flushLatencyMax;
};

std::vector<std::unique_ptr<DiskInfo>> GetDisks();
