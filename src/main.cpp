#include "DiskInfo.h"
#include "WinUtils.h"
#include "Print.h"

int main()
{
    ComInitializer com(COINIT_MULTITHREADED);

    if (FAILED(CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    )))
    {
        std::cerr << "CoInitializeSecurity failed" << std::endl;
        return -1;
    }

    auto disks = GetDisks();
    PrintDisksInfoTable(disks);

    std::cout << "\nDisks Health information:\n";
    for (const auto& disk : disks) {
        std::wcout << L"\n----#" << disk->m_number << " \'" << disk->m_friendlyName << L"\'----\n";
        if (disk->m_healthInfo)
            PrintDiskHealthInfo(*disk->m_healthInfo);
        else
            std::cout << L"Health info wasn't retrieved" << std::endl;
    }

    return 0;
}
