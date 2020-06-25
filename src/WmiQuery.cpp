#include "WmiQuery.h"
#include "WmiObject.h"

#include <Wbemidl.h>

WmiQuery::WmiQuery(const CComPtr<IEnumWbemClassObject>& enumerator)
    : m_queriedInstances(ReadObjects(enumerator))
{}

WmiQuery::~WmiQuery()
{}

WmiQuery::ObjectCollection WmiQuery::ReadObjects(const CComPtr<IEnumWbemClassObject>& enumerator)
{
    // enumerate instances
    ObjectCollection objects;
    {
        enumerator->Reset();
        HRESULT hret = WBEM_S_NO_ERROR;
        do
        {
            CComPtr<IWbemClassObject> curObject;
            ULONG received = 0;

            hret = enumerator->Next(WBEM_INFINITE, 1, &curObject, &received);
            if (!curObject || !received)
                break;

            objects.push_back(std::make_shared<WmiObject>(curObject));
        } while (hret == WBEM_S_NO_ERROR);
    }
    return objects;
}

const WmiQuery::ObjectCollection& WmiQuery::GetObjects() const
{
    return m_queriedInstances;
}
