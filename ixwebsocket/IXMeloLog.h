/*
 *  IXMeloLog.h
 *  Melo diagnostic log sink for ixwebsocket. The host app installs a callback
 *  (routed to juce::Logger) so internal TLS/handshake events reach the melo
 *  logs and get uploaded. No-op until a callback is installed.
 */
#pragma once

#include <functional>
#include <string>

namespace ix
{
    using MeloLogCallback = std::function<void(const std::string&)>;

    // Install once at startup, before any socket is used.
    void setMeloLogCallback(MeloLogCallback callback);

    bool meloLogEnabled();

    // Forwards to the installed callback; no-op if none. Safe from any thread
    // once the callback is installed (install-before-use, not re-entrant-set).
    void meloLog(const std::string& message);
} // namespace ix
