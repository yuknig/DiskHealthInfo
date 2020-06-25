#pragma once
#include "WmiQuery.h"

#include <atlbase.h>
#include <memory>
#include <string>

struct IWbemLocator;
struct IWbemServices;

class WmiHandler
{
public:
    WmiHandler(const std::wstring& wmiNamespace);

    std::unique_ptr<WmiQuery> ExecQuery(const std::wstring& query);

private:
    CComPtr<IWbemLocator> m_locator;
    CComPtr<IWbemServices> m_services;
};
