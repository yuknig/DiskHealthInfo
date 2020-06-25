#include "WmiObjectProperties.h"
#include "StringUtils.h"

#include <wbemcli.h>
#include <iostream>

namespace
{

bool ReadProperties(const CComPtr<IWbemClassObject>& instance, std::map<std::wstring, CComVariant>& outProperties, LONG enumFlags)
{
    HRESULT hret = instance->BeginEnumeration(enumFlags);
    if (FAILED(hret)) {
        std::cerr << __FUNCTION__ << " : IWbemClassObject::BeginEnumeration() failed" << std::endl;
        return false;
    }

    do
    {
        CComBSTR name;
        CComVariant val;
        hret = instance->Next(0, &name, &val, nullptr, nullptr);
        if (FAILED(hret)) {
            std::cerr << __FUNCTION__ << " : IWbemClassObject::Next() failed" << std::endl;
            return false;
        }

        auto nameStr = FromCComBstr(name);
        outProperties.emplace(nameStr, val);
    } while (hret != WBEM_S_NO_MORE_DATA);

    instance->EndEnumeration();
    return true;
}

} // namespace anonymous


WmiObjectProperties::WmiObjectProperties(const CComPtr<IWbemClassObject>& object)
{
    if (!ReadProperties(object, m_properties, WBEM_FLAG_ALWAYS))
        throw std::runtime_error("Failed to get WMI object properties");
}

bool WmiObjectProperties::PropertyExists(const std::wstring& name) const
{
    return (m_properties.count(name) != 0);
}

std::wstring WmiObjectProperties::GetPropertyText(const std::wstring& name) const
{
    auto findIt = m_properties.find(name);
    if (findIt == m_properties.end())
        return {};

    auto text = VariantToString<wchar_t>(findIt->second);
    return text;
}
