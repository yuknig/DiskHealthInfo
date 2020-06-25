#include "WmiObject.h"
#include "StringUtils.h"
#include "WmiObjectProperties.h"
#include <wbemcli.h>
#include <iostream>

WmiObject::WmiObject(const CComPtr<IWbemClassObject>& instance)
    : m_instance(instance)
{}

WmiObject::~WmiObject()
{}

const WmiObjectProperties* WmiObject::Properties()
{
    if (!m_propertiesInited) {
        m_propertiesInited = true;
        try
        {
            m_properties = std::make_unique<WmiObjectProperties>(m_instance);
        }
        catch (const std::runtime_error&)
        {
            std::cerr << "Failed to create WmiObjectProperties" << std::endl;
        }
    }

    return m_properties.get();
}
