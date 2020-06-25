#pragma once
#include "DiskInfo.h"

#include <memory>
#include <vector>

void PrintDisksInfoTable(const std::vector<std::unique_ptr<DiskInfo>>& disks);
void PrintDiskHealthInfo(const DiskHealthInfo& info);
