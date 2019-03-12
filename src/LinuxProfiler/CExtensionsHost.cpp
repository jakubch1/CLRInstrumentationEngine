// Copyright (c) Microsoft Corporation. All rights reserved.
// 

// CExtensionsHost.cpp : Implementation of CExtensionHost

#include "stdafx.h"
#include <unistd.h>
#include "CExtensionsHost.h"


HRESULT ExtensionsHostCrossPlat::CExtensionHost::Initialize(
    _In_ IProfilerManager* pProfilerManager
)
{
    CComPtr<ICorProfilerInfo> pCorProfilerInfo;
    pProfilerManager->GetCorProfilerInfo((IUnknown**)&pCorProfilerInfo);
    pCorProfilerInfo->SetEventMask(COR_PRF_MONITOR_EXCEPTIONS | COR_PRF_ENABLE_STACK_SNAPSHOT);
    return S_OK;
}

HRESULT ExtensionsHostCrossPlat::CExtensionHost::OnModuleLoaded(IModuleInfo* pModuleInfo)
{
    HRESULT hr = S_OK;

    CComBSTR bstrModuleName;
    IfFailRet(pModuleInfo->GetModuleName(&bstrModuleName));

    CComBSTR bstrModulePath;
    IfFailRet(pModuleInfo->GetFullPath(&bstrModulePath));

    wstring pdbReader = L"/home/maban/projects/XPlatPdbReader/bin/x64/Debug/libXPlatPdbReader.so";

    WCHAR wszModuleFullPath[MAX_PATH];
    memset(wszModuleFullPath, 0, MAX_PATH);
    wcscpy_s(wszModuleFullPath, MAX_PATH, pdbReader.c_str());
    
    m_hmod = ::LoadLibrary(wszModuleFullPath);

    typedef int(*ReadPdb)(wstring path);

    return hr;
}
