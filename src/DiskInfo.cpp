#include "DiskInfo.h"
#include "StringUtils.h"
#include "WinUtils.h"
#include "WmiHandler.h"
#include "WmiObject.h"
#include "WmiObjectProperties.h"
#include "WmiQuery.h"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <wbemidl.h>

namespace
{

std::wstring HealtStatusDescFromCode(const std::wstring& codeStr)
{
    int code = -1;
    if (!FromStdString(codeStr, code))
        return L"FailedToConvert";

    switch (code) {
    case 0:
        return L"Healthy";
    case 1:
        return L"Warning";
    case 2:
        return L"Unhealthy";
    default:
        return L"unknown";
    }
}

std::wstring BusTypeFromCode(const std::wstring& codeStr)
{
    int code = -1;
    if (!FromStdString(codeStr, code))
        return L"FailedToConvert";

    switch (code) {
    case 1:
        return L"SCSI";
    case 2:
        return L"ATAPI";
    case 3:
        return L"ATA";
    case 4:
        return L"IEEE 1394";
    case 5:
        return L"SSA";
    case 6:
        return L"Fibre Channel";
    case 7:
        return L"USB";
    case 8:
        return L"RAID";
    case 9:
        return L"iSCSI";
    case 10:
        return L"Serial Attached SCSI (SAS)";
    case 11:
        return L"Serial ATA (SATA)";
    case 12:
        return L"Secure Digital (SD)";
    case 13:
        return L"Multimedia Card (MMC)";
    case 14:
        return L"Virtual(This value is reserved for system use)";
    case 15:
        return L"File-Backed Virtual";
    case 16:
        return L"Storage spaces";
    case 17:
        return L"NVMe";
    case 0:
    default:
        return L"unknown";
    }
}

std::wstring GetSizeStr(const std::wstring& bytesStr)
{
    uint64_t bytes = 0;
    if (!FromStdString(bytesStr, bytes))
        return L"failed to parse";

    const double GBs = bytes * 1e-9; // number of decimal (not binary) gigabytes
    std::wostringstream str;
    str << std::setprecision(5) << GBs << " GB";
    return str.str();
}

} // namespace anonymous

std::unique_ptr<DiskInfo> DiskInfo::CreateFromProperties(const IPropertyCollection& p)
{
    if (!p.PropertyExists(L"FriendlyName")) {
        std::cerr << __FUNCTION__ " missing mandatory properties" << std::endl;
        return {};
    }

    DiskInfo info = {};
    info.m_friendlyName = p.GetPropertyText(L"FriendlyName");
    info.m_number = p.GetPropertyText(L"Number");
    info.m_serialNumber = p.GetPropertyText(L"SerialNumber");
    info.m_model = p.GetPropertyText(L"Model");
    info.m_size = GetSizeStr(p.GetPropertyText(L"Size"));
    info.m_numberOfPartitions = p.GetPropertyText(L"NumberOfPartitions");
    info.m_busType = BusTypeFromCode(p.GetPropertyText(L"BusType"));
    info.m_isBoot = p.GetPropertyText(L"IsBoot");
    info.m_healthStatus = HealtStatusDescFromCode(p.GetPropertyText(L"HealthStatus"));
    return std::make_unique<DiskInfo>(std::move(info));
}

std::map<std::wstring, std::wstring> DiskInfo::Properties() const
{
    return
    {       { L"FriendlyName", m_friendlyName },
            { L"Number", m_number },
            { L"SerialNumber", m_serialNumber },
            { L"Model", m_model },
            { L"Size", m_size },
            { L"NumberOfPartitions", m_numberOfPartitions },
            { L"BusType", m_busType },
            { L"IsBoot", m_isBoot },
            { L"HealthStatus", m_healthStatus },
    };
}


std::unique_ptr<DiskHealthInfo> DiskHealthInfo::CreateFromProperties(const IPropertyCollection& p)
{
    DiskHealthInfo info = {};
    info.m_temperature = p.GetPropertyText(L"Temperature");
    info.m_temperatureMax = p.GetPropertyText(L"TemperatureMax");
    info.m_readErrorsTotal = p.GetPropertyText(L"ReadErrorsTotal");
    info.m_readErrorsCorrected = p.GetPropertyText(L"ReadErrorsCorrected");
    info.m_readErrorsUncorrected = p.GetPropertyText(L"ReadErrorsUncorrected");
    info.m_writeErrorsTotal = p.GetPropertyText(L"WriteErrorsTotal");
    info.m_writeErrorsCorrected = p.GetPropertyText(L"WriteErrorsCorrected");
    info.m_writeErrorsUncorrected = p.GetPropertyText(L"WriteErrorsUncorrected");
    info.m_manufactureDate = p.GetPropertyText(L"ManufactureDate");
    info.m_startStopCycleCount = p.GetPropertyText(L"StartStopCycleCount");
    info.m_startStopCycleCountMax = p.GetPropertyText(L"StartStopCycleCountMax");
    info.m_loadUnloadCycleCount = p.GetPropertyText(L"LoadUnloadCycleCount");
    info.m_loadUnloadCycleCountMax = p.GetPropertyText(L"LoadUnloadCycleCountMax");
    info.m_wear = p.GetPropertyText(L"Wear");
    info.m_powerOnHours = p.GetPropertyText(L"PowerOnHours");
    info.m_readLatencyMax = p.GetPropertyText(L"ReadLatencyMax");
    info.m_writeLatencyMax = p.GetPropertyText(L"WriteLatencyMax");
    info.m_flushLatencyMax = p.GetPropertyText(L"FlushLatencyMax");

    return std::make_unique<DiskHealthInfo>(std::move(info));
}

std::map<std::wstring, std::wstring> DiskHealthInfo::Properties() const
{
    return
    {
        { L"Temperature", m_temperature },
        { L"TemperatureMax", m_temperatureMax },
        { L"ReadErrorsTotal", m_readErrorsTotal },
        { L"ReadErrorsCorrected", m_readErrorsCorrected },
        { L"ReadErrorsUncorrected", m_readErrorsUncorrected },
        { L"WriteErrorsTotal", m_writeErrorsTotal },
        { L"WriteErrorsCorrected", m_writeErrorsCorrected },
        { L"WriteErrorsUncorrected", m_writeErrorsUncorrected },
        { L"ManufactureDate", m_manufactureDate },
        { L"StartStopCycleCount", m_startStopCycleCount },
        { L"StartStopCycleCountMax", m_startStopCycleCountMax },
        { L"LoadUnloadCycleCount", m_loadUnloadCycleCount },
        { L"LoadUnloadCycleCountMax", m_loadUnloadCycleCountMax },
        { L"Wear", m_wear },
        { L"PowerOnHours", m_powerOnHours },
        { L"ReadLatencyMax", m_readLatencyMax },
        { L"WriteLatencyMax", m_writeLatencyMax },
        { L"FlushLatencyMax", m_flushLatencyMax },
    };
}


std::vector<std::unique_ptr<DiskInfo>> GetDisks()
{
    const std::wstring wmiNamespace = L"ROOT\\Microsoft\\Windows\\Storage";
    WmiHandler wmi(wmiNamespace);

    auto disksQuery = wmi.ExecQuery(L"SELECT * FROM MSFT_Disk");
    if (!disksQuery) {
        std::cerr << "Disks query failed" << std::endl;
        return {};
    }

    std::vector<std::unique_ptr<DiskInfo>> disks;
    {
        const auto& diskObjects = disksQuery->GetObjects();
        disks.reserve(diskObjects.size());
        for (size_t diskNum = 0; diskNum < diskObjects.size(); ++diskNum) {
            const auto* diskProps = diskObjects[diskNum]->Properties();
            if (!diskProps) {
                std::cerr << "Failed to get disk properties for disk#" << diskNum << std::endl;
                continue;
            }

            auto disk = DiskInfo::CreateFromProperties(*diskProps);
            if (!disk) {
                std::cerr << "Failed to create DiskInfo due to missing properties for disk#" << diskNum << std::endl;
                continue;
            }

            const auto diskName = ToStringA(disk->m_friendlyName);

            auto wmiPath = diskProps->GetPropertyText(L"__PATH");
            if (wmiPath.empty()) {
                std::cerr << "Failed to get needed property '__PATH' for disk#" << diskNum << std::endl;
                continue;
            }

            // get associated object with Disk Reliability info
            const std::wstring reliabilityQueryText = L"ASSOCIATORS OF {" + wmiPath + L"}" +
                L" WHERE ResultClass=MSFT_StorageReliabilityCounter";

            auto reliabilityQuery = wmi.ExecQuery(reliabilityQueryText);
            if (!reliabilityQuery) {
                std::cerr << "Failed to perform Disk Reliability query" << std::endl;
                continue;
            }

            const auto reliabilityObjects = reliabilityQuery->GetObjects();
            if (reliabilityObjects.empty()) {
                std::cerr << "Failed to get Reliability Counters for disk '" << diskName << "\'";
                static bool isAdmin = IsAdministrator();
                if (!isAdmin)
                    std::cerr << ". Could be due to lack of the admin rights";
                std::cerr << std::endl;
                continue;
            }

            const auto& reliabilityProps = reliabilityObjects[0]->Properties();
            if (!reliabilityProps) {
                std::cerr << "Failed to get disk reliability properties for disk " << diskName << std::endl;
                continue;
            }

            disk->m_healthInfo = DiskHealthInfo::CreateFromProperties(*reliabilityProps);
            if (!disk->m_healthInfo) {
                std::cerr << "Failed to create DiskHealthInfo for disk " << diskName << std::endl;
                continue;
            }
            disks.push_back(std::move(disk));
        }
    }
    return disks;
}
