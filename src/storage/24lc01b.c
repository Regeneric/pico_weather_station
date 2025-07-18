#include <config/arm.h>
#if hkEEPROM_24LC01B

#include <FreeRTOS.h>
#include <task.h>

#include <stdio.h>
#include <string.h>

#include <hardware/i2c.h>

#include <core/logger.h>
#include <storage/storage.h>
#include <storage/eeprom.h>


#define EEPROM_PAGE_SIZE    8

b8 EEPROM_Write(const void* config, const void* packet) {
    HTRACE("24lc01b.c -> EEPROM_Write(const void*, const void*):b8");

    I2C_Config_t* i2c = (I2C_Config_t*)config;
    const DataPacket_t* data = (DataPacket_t*)packet;
    u8 address = (u8)data->address;

    // if(hkEEPROM_24FC01 == false && i2c->speed > (400 * 1000)) {
    //     HERROR("EEPROM_Write(): Clock faster than 400 KHz is supported only on 24FC01. Current clock: ", (i2c->speed / 1000));
    //     return false;
    // } 

    // Sanity check
    if(data->size == 0 || data->size > EEPROM_PAGE_SIZE) {
        HDEBUG("EEPROM_Write(): Invalid page size %u", data->size);
        return false;
    }

    // Init write
    mutex_enter_blocking(i2c->mutex);
    HTRACE("EEPROM_Write(): I2C mutex acquired");

    if(i2c_write_blocking(i2c->i2c, EEPROM_ADDR, &address, 1, true) != 1) {
        HDEBUG("EEPROM_Write(): Failed to send control signal to: 0x%x", EEPROM_ADDR);
        mutex_exit(i2c->mutex);
        return false;
    }

    // Data page write
    u8 buffer[1 + EEPROM_PAGE_SIZE];
    buffer[0] = address;
    memcpy(&buffer[1], data->data, data->size);

    if(i2c_write_blocking(i2c->i2c, EEPROM_ADDR, buffer, (1+data->size), false) != (1+data->size)) {
        HDEBUG("EEPROM_Write(): Failed to write data to: 0x%x ; 0x%x", EEPROM_ADDR, address);
        mutex_exit(i2c->mutex);
        HTRACE("EEPROM_Write(): I2C mutex released");
        return false;
    } 

    mutex_exit(i2c->mutex);
    HTRACE("EEPROM_Write(): I2C mutex released");

    // Wait for ACK so we're sure that data write is complete
    // while(i2c_write_blocking(i2c->i2c, EEPROM_ADDR, &address, 1, false) != 1);
    sleep_ms(6);
    return true;
}

b8 EEPROM_Read(const void* config, void* packet) {
    HTRACE("24lc01b.c -> EEPROM_Read(const void*, const void*):b8");

    I2C_Config_t* i2c  = (I2C_Config_t*)config;
    DataPacket_t* data = (DataPacket_t*)packet;
    u8 address = (u8)data->address;

    if(hkEEPROM_24FC01 == false && i2c->speed > 400) {
        HERROR("EEPROM_Read(): Clock faster than %u KHz is supported only on 24FC01", i2c->speed);
        return false;
    } 
    
    mutex_enter_blocking(i2c->mutex);
    HTRACE("EEPROM_Read(): mutex acquired");

    if(i2c_write_blocking(i2c->i2c, EEPROM_ADDR, &address, 1, true) != 1) {
        HDEBUG("EEPROM_Read(): Failed to send control signal to: 0x%x", EEPROM_ADDR);
        mutex_exit(i2c->mutex);
        HTRACE("EEPROM_Read(): I2C mutex released");
        return false;
    }

    if(i2c_read_blocking(i2c->i2c, EEPROM_ADDR, data->data, data->size, false) != data->size) {
        HDEBUG("EEPROM_Read(): Failed to read data from: 0x%x ; 0x%x", EEPROM_ADDR, address);
        mutex_exit(i2c->mutex);
        HTRACE("EEPROM_Read(): I2C mutex released");
        return false;
    }

    mutex_exit(i2c->mutex); 
    HTRACE("EEPROM_Read(): I2C mutex released");
    return true;
}

u16 EEPROM_WriteBlob(const I2C_Config_t* i2c, u16 addr, const u8* data, size_t len) {
    size_t offset = 0;
    while(offset < len) {
        u16 absAddr = addr + offset;
        size_t pageOffset = absAddr % EEPROM_PAGE_SIZE;
        size_t chunk      = EEPROM_PAGE_SIZE - pageOffset;
        if(chunk > len - offset) chunk = len - offset;

        DataPacket_t packet = {
            .address = absAddr,
            .data    = (u8*)(data + offset),
            .size    = chunk
        };

        if(!EEPROM_Write(i2c, &packet)) return false;
        
        vTaskDelay(pdMS_TO_TICKS(6));
        offset += chunk;
    } 
    
    return (addr + offset);
}

b8 EEPROM_ReadBlob(const I2C_Config_t* i2c, u16 addr, u8* buff, size_t len) {
    DataPacket_t packet = { .address = addr, .data = buff, .size = len };
    return EEPROM_Read(i2c, &packet);
}
#endif