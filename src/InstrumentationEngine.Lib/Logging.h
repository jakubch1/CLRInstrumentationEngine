// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <atomic>
#include <sstream>

#include "../InstrumentationEngine.Api/InstrumentationEngine.h"
#include "LoggerService.h"

#include "../Common.Lib/tstring.h"
#include "../Common.Lib/InitOnce.h"
#include "../Common.Lib/Singleton.h"

// CLogging uses *wprintf* methods for formatting strings.
#ifdef PLATFORM_UNIX
// Use %S for WCHAR* on Linux (which is char16_t*) since *wprintf* expects strings to be wchar_t*.
// The capital S specifier means "format the string using the width that is opposite of the formatting method".
// The capital S specifier is appropriate since WCHAR* on Linux is smaller than the *wprintf* width expectation.
#define WCHAR_SPEC _T("%S")
#else
// Use %s for WCHAR* on Windows (which is wchar_t*)
// The lowercase s specifier is appropriate since WCHAR* on Windows matches *wprintf* width expectation.
#define WCHAR_SPEC _T("%s")
#endif

using namespace CommonLib;

namespace MicrosoftInstrumentationEngine
{
    class CLogging
    {
    public:
        class XmlDumpHelper final
        {
        private:
            tstring m_tag;
            tstring m_indent;
            tstring m_childrenIndent;
            tstringstream m_result;
        public:
            XmlDumpHelper(const WCHAR* tag, const unsigned int indent);
            ~XmlDumpHelper();

            void WriteStringNode(const WCHAR* name, const WCHAR* value);
            void WriteUlongNode(const WCHAR* name, const ULONG32 value);
        };

    private:
        static CInitOnce s_initialize;
        static CSingleton<CLoggerService> s_loggerService;
        static std::atomic_size_t s_initCount;

    public:
        // Call this to determine if logging should be allowed for a specific log type.
        static bool AllowLogEntry(_In_ LoggingFlags flags);

        // Allows instrumentation methods and hosts to ask for the current logging flags
        static HRESULT GetLoggingFlags(_Out_ LoggingFlags* pLoggingFlags);

        static void LogMessage(_In_ const WCHAR* szMessage, ...);
        static void LogError(_In_ const WCHAR* szError, ...);
        static void LogDumpMessage(_In_ const WCHAR* szMessage, ...);

        static void VLogMessage(_In_ const WCHAR* szMessage, _In_ va_list argptr);
        static void VLogError(_In_ const WCHAR* szError, _In_ va_list argptr);
        static void VLogDumpMessage(_In_ const WCHAR* szMessage, _In_ va_list argptr);

        static HRESULT Initialize();

        static void SetLogToDebugPort(_In_ bool enable);

        // Allows instrumentation methods and hosts to modify the current logging flags
        static HRESULT SetLoggingFlags(_In_ LoggingFlags loggingFlags);

        static HRESULT UpdateInstrumentationMethodLoggingFlags(_In_ GUID classId, _In_ LoggingFlags loggingFlags);

        static HRESULT SetLoggingHost(_In_ IProfilerManagerLoggingHost* pLoggingHost);

        static HRESULT Shutdown();

    private:
        static HRESULT InitializeCore();
    };
}