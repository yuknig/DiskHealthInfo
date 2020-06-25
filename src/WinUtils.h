#pragma once
#include <atlbase.h>
#include <Objbase.h>
#include <Windows.h>

#include <iostream>

class ComInitializer final
{
public:
    ComInitializer(DWORD model)
    {
        auto hret = CoInitializeEx(0, model);
        if (FAILED(hret))
        {
            std::cerr << "CoInitializeEx call failed" << std::endl;
            throw std::runtime_error("Failed to initialize COM");
        }
    }

    ~ComInitializer()
    {
        CoUninitialize();
    }
};

inline bool IsAdministrator()
{
    CHandle token;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token.m_h)) {
        std::cerr << "OpenProcessToken failed" << std::endl;
        return false;
    }

    TOKEN_ELEVATION elevation{};
    DWORD cb_size = sizeof(elevation);
    if (!GetTokenInformation(token, TokenElevation, &elevation, sizeof(elevation), &cb_size)) {
        std::cerr << "GetTokenInformation failed" << std::endl;
        return false;
    }

    return (0 != elevation.TokenIsElevated);
}
