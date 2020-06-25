#include "Print.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <functional>

using GetRowContent = std::function<std::map<std::wstring, std::wstring>(size_t rowNum)>; // returns map<columnName, columnValue>

void PrintTable(const std::vector<std::wstring>& columns, size_t rows, const GetRowContent& getContent)
{
    // find maximum length of each column
    std::map<std::wstring, size_t> maxColLength;
    // go through column headers
    for (const auto& col : columns)
        maxColLength[col] = col.length(); // init with length of column title

    // go through rows
    for (size_t row = 0; row < rows; ++row) {
        auto rowProps = getContent(row);
        for (const auto& col_value : rowProps) {
            const auto& colName = col_value.first;
            if (!maxColLength.count(colName))
                continue;
            size_t& maxLength = maxColLength[colName];
            maxLength = std::max<size_t>(maxLength, col_value.second.length());
        }
    }

    const size_t tableWidth = 1 + std::accumulate(maxColLength.begin(), maxColLength.end(), maxColLength.size(),
        [](const size_t& value, const auto& entry) {
            return value + entry.second;
        });

    // print table header
    {
        std::wcout << std::wstring(tableWidth, L'-') << std::endl;

        std::wstring row;
        for (const auto& colTitle : columns) {
            row += L'|';
            const size_t colLength = maxColLength.at(colTitle);
            const size_t padLength = std::max<size_t>(colTitle.length(), colLength) - colTitle.length();
            const size_t padLeft = padLength / 2;
            const size_t padRight = padLength - padLeft;
            row.insert(row.length(), padLeft, L' ');
            row += colTitle;
            row.insert(row.length(), padRight, L' ');
        }
        row += L'|';
        assert(row.length() == tableWidth);
        std::wcout << row << std::endl;

        std::wcout << std::wstring(tableWidth, L'=') << std::endl;
    }

    // print rows
    for (size_t rowNum = 0; rowNum < rows; ++rowNum) {
        const auto rowProps = getContent(rowNum);
        std::wstring row;
        for (const auto& col : columns) {
            const auto colValue = rowProps.count(col) ? rowProps.at(col) : L"";
            row += L'|' + colValue;

            const size_t colLength = maxColLength.at(col);
            const size_t padLength = std::max<size_t>(colValue.length(), colLength) - colValue.length();
            row.insert(row.length(), padLength, L' ');
        }
        row += '|';
        assert(row.length() == tableWidth);

        std::wcout << row << std::endl;
    }

    // print table bottom
    std::wcout << std::wstring(tableWidth, L'-') << std::endl;
}

void PrintDisksInfoTable(const std::vector<std::unique_ptr<DiskInfo>>& disks)
{
    if (disks.empty()) {
        std::cout << "No disks found..." << std::endl;
        return;
    }

    static const std::vector<std::wstring> columnNames = { L"Number", L"FriendlyName", L"SerialNumber", L"BusType",  L"Size", L"IsBoot", L"HealthStatus" };

    std::cout << "Disks found in system: " << disks.size() << std::endl;
    PrintTable(columnNames, disks.size(), [&](const auto& rowNum) {
        return disks.at(rowNum)->Properties();
    });
}

void PrintDiskHealthInfo(const DiskHealthInfo& info)
{
    const auto& props = info.Properties();

    static const std::vector<std::wstring> rowNames =
    {
        L"Temperature", L"TemperatureMax", L"ReadErrorsTotal", L"ReadErrorsCorrected", L"ReadErrorsUncorrected", L"WriteErrorsTotal", L"WriteErrorsCorrected",
        L"WriteErrorsUncorrected", L"ManufactureDate", L"StartStopCycleCount", L"StartStopCycleCountMax", L"LoadUnloadCycleCount", L"LoadUnloadCycleCountMax",
        L"Wear", L"PowerOnHours", L"ReadLatencyMax", L"WriteLatencyMax", L"FlushLatencyMax"
    };

    size_t maxLength = 0;
    for (const auto& name : rowNames)
        maxLength = std::max<size_t>(maxLength, name.length());

    for (const auto& rowName : rowNames)
    {
        std::wstring row = rowName;
        row.insert(row.length(), maxLength - row.length(), L' ');
        row += L": " + (props.count(rowName) ? props.at(rowName) : L"N/A");
        std::wcout << row << std::endl;
    }
}
