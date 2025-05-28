#pragma once

// Tell Mongoose we're on FreeRTOS
#define MG_ARCH MG_ARCH_FREERTOS

// Use custom mg_millis function for timing
#define MG_ENABLE_CUSTOM_MILLIS 1

// Enable HTTP and TLS client support
#define MG_ENABLE_HTTP 1
#define MG_ENABLE_MBEDTLS 1
#define MG_ENABLE_NET 1
#define MG_ENABLE_LOG 1
#define MG_ENABLE_SOCKS 0
#define MG_ENABLE_TCPIP 0
