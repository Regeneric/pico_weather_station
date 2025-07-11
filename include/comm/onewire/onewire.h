#pragma once
#include <defines.h>

#include <hardware/pio.h>

#define ONEW_READ_ROM         0x33
#define ONEW_MATCH_ROM        0x55
#define ONEW_SKIP_ROM         0xCC
#define ONEW_ALARM_SEARCH     0xEC
#define ONEW_SEARCH_ROM       0xF0

typedef enum {
    ONEW_INIT,
    ONEW_INIT_SUCCESS,
    ONEW_READ_SUCCESS,
    ONEW_READ_IN_PROGRESS,
    ONEW_WRITE_SUCCESS,
    ONEW_WRITE_IN_PROGRESS,
    ONEW_DMA_IN_PROGRESS,
    ONEW_DMA_SUCCESS
} OneWire_Status_t;

#if hkOW_USE_DMA
    typedef struct OneWire_Config_t {
        u8  gpio;
        vu8 status;
        PIO pio;
        u8  sm;
        u8  bitMode;
        u8* data;
        u8  packetSize;
        size_t length;
    } OneWire_Config_t;

    void OneWire_Init(OneWire_Config_t* config);
    b8   OneWire_Reset(OneWire_Config_t* config);
    void OneWire_Read(OneWire_Config_t* config);
    // u8   OneWire_Read(OneWire_Config_t* config);
    b8   OneWire_WriteByte(OneWire_Config_t* config, u32 data);
    void OneWire_Write(OneWire_Config_t* config, u8* data, size_t len);
    // b8   OneWire_Write(OneWire_Config_t* config, u8* buffer, size_t length);
    u8   OneWire_SearchROM(OneWire_Config_t* config, u64* devices, u8 maxDevices, u8 command);
#else
    typedef struct OneWire_Config_t {
        u8  gpio;
        vu8 status;
        PIO pio;
        u8  sm;
        u8  bitMode;
    } OneWire_Config_t;

    void OneWire_Init(OneWire_Config_t* config);
    b8   OneWire_Reset(OneWire_Config_t* config);
    u8   OneWire_Read(OneWire_Config_t* config);
    b8   OneWire_WriteByte(OneWire_Config_t* config, u32 data);
    b8   OneWire_Write(OneWire_Config_t* config, u8* buffer, size_t length);
    u8   OneWire_SearchROM(OneWire_Config_t* config, u64* devices, u8 maxDevices, u8 command);
#endif