/*
 *  IXMeloLog.cpp
 */

#include "IXMeloLog.h"

namespace ix
{
    static MeloLogCallback g_meloLogCallback;

    void setMeloLogCallback(MeloLogCallback callback)
    {
        g_meloLogCallback = std::move(callback);
    }

    bool meloLogEnabled()
    {
        return static_cast<bool>(g_meloLogCallback);
    }

    void meloLog(const std::string& message)
    {
        if (g_meloLogCallback) g_meloLogCallback(message);
    }
} // namespace ix
