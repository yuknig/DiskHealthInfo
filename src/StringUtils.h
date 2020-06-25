#pragma once
#include <atlbase.h>
#include <atlsafe.h>
#include <codecvt>
#include <oaidl.h>
#include <string>
#include <sstream>

inline CComBSTR ToComBStr(const std::wstring& str)
{
    if (str.empty())
        return {};
    return CComBSTR(static_cast<int>(str.size()), str.data());
}

inline CComBSTR ToComBStr(const std::string& str)
{
    if (str.empty())
        return {};
    return CComBSTR(static_cast<int>(str.size()), str.data());
}

inline std::wstring FromCComBstr(const CComBSTR& str)
{
    return { static_cast<const wchar_t*>(str), str.Length() };
}

template <typename ToCharT, typename T>
inline std::basic_string<ToCharT> ToStdString(const T& value)
{
    std::basic_ostringstream<ToCharT> oss;
    oss.imbue(std::locale::classic());
    oss << value;
    return oss.str();
}

template <typename ToT, typename FromCharT>
bool FromStdString(const std::basic_string<FromCharT>& from, ToT& result)
{
    std::basic_istringstream<FromCharT> iss(from);
    iss.imbue(std::locale::classic());

    ToT v{};
    iss >> v;
    if (iss.fail() || !iss.eof())
        return false;

    result = v;
    return true;
}

inline std::wstring ToStringW(const std::string& str)
{
    return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(str);
}

inline std::string ToStringA(const std::wstring& str)
{
    return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(str);
}

template <typename CharT>
std::basic_string<CharT> VariantToString(const VARIANT& v)
{
    switch (v.vt)
    {
    case VT_EMPTY:
        return ToStdString<CharT>("<empty>");
    case VT_NULL:
        return ToStdString<CharT>("<NULL>");
    case VT_I2:
        return ToStdString<CharT>(v.iVal);
    case VT_I4:
        return ToStdString<CharT>(v.lVal);
    case VT_R4:
        return ToStdString<CharT>(v.fltVal);
    case VT_R8:
        return ToStdString<CharT>(v.dblVal);
    case VT_BSTR:
        return ToStdString<CharT>(v.bstrVal);
    case VT_BOOL:
        return ToStdString<CharT>(v.boolVal ? "true" : "false");
    case VT_I1:
        return ToStdString<CharT>(v.cVal);
    case VT_UI1:
        return ToStdString<CharT>(v.bVal);
    case VT_UI2:
        return ToStdString<CharT>(v.uiVal);
    case VT_UI4:
        return ToStdString<CharT>(v.ullVal);
    case VT_INT:
        return ToStdString<CharT>(v.lVal);
    case VT_UINT:
        return ToStdString<CharT>(v.ulVal);
    default:
    {
        if (v.vt & VT_ARRAY)
        {
            const auto* array = v.parray;
            std::basic_stringstream<CharT> str;
            str << '[';
            switch (v.vt ^ VT_ARRAY)
            {
            case VT_BSTR:
            {
                const CComSafeArray<BSTR> array(v.parray);
                for (size_t i = 0; i < array.GetCount(); ++i)
                {
                    if (i > 0)
                        str << ',';
                    str << ToStdString<CharT>(array.GetAt(static_cast<LONG>(i)).m_str);
                }
            }
            break;
            default:
                str << ToStdString<CharT>("unsupported array type");
            }
            str << ']';
            return str.str();
        }
        return ToStdString<CharT>("unsupported type");
    }
    };
}
