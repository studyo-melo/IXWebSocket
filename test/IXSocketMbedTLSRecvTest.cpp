/*
 *  IXSocketMbedTLSRecvTest.cpp
 *  Regression test for MELO_MINGW_MBEDTLS_NST.
 */

#include <catch2/catch_test_macros.hpp>

#ifdef IXWEBSOCKET_USE_MBED_TLS

#include <ixwebsocket/IXSocketMbedTLS.h>
#include <mbedtls/ssl.h>

using namespace ix;

namespace ix
{
    TEST_CASE("mbedtls_recv_classify", "[mbedtls][nst]")
    {
        SECTION("a positive result is data")
        {
            REQUIRE(classifyMbedTLSReadResult(1) == MbedTLSReadOutcome::Data);
            REQUIRE(classifyMbedTLSReadResult(4096) == MbedTLSReadOutcome::Data);
        }

        // The regression this test guards. A TLS 1.3 server (Cloudflare) sends a
        // NewSessionTicket post-handshake; mbedtls returns this on the first read
        // of the HTTP 101 status line. Before the fix recv() returned -1 here, so
        // readLine saw 0 bytes and every WS route died with "Failed reading HTTP
        // status line (read 0 bytes)". It must retry, not error.
        SECTION("a new session ticket retries instead of erroring")
        {
            REQUIRE(classifyMbedTLSReadResult(MBEDTLS_ERR_SSL_RECEIVED_NEW_SESSION_TICKET)
                    == MbedTLSReadOutcome::Retry);
        }

        SECTION("zero is a connection reset")
        {
            REQUIRE(classifyMbedTLSReadResult(0) == MbedTLSReadOutcome::ConnectionReset);
        }

        SECTION("want read or write would block")
        {
            REQUIRE(classifyMbedTLSReadResult(MBEDTLS_ERR_SSL_WANT_READ)
                    == MbedTLSReadOutcome::WouldBlock);
            REQUIRE(classifyMbedTLSReadResult(MBEDTLS_ERR_SSL_WANT_WRITE)
                    == MbedTLSReadOutcome::WouldBlock);
        }

        SECTION("any other failure is an error")
        {
            REQUIRE(classifyMbedTLSReadResult(MBEDTLS_ERR_SSL_INTERNAL_ERROR)
                    == MbedTLSReadOutcome::Error);
            REQUIRE(classifyMbedTLSReadResult(MBEDTLS_ERR_SSL_FATAL_ALERT_MESSAGE)
                    == MbedTLSReadOutcome::Error);
            REQUIRE(classifyMbedTLSReadResult(-1) == MbedTLSReadOutcome::Error);
        }
    }
} // namespace ix

#endif // IXWEBSOCKET_USE_MBED_TLS
