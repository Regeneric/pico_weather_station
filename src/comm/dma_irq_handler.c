#include <hardware/dma.h>
#include <hardware/irq.h>

#include <comm/dma_irq_handler.h>
#include <comm/uart.h>
#include <comm/onewire/onewire.h>

#include <sensors/dht11_22/dht11_22.h>

static u32 sgDMA_UART_Channel = -1;
static UART_Config_t* sgUART_Config; 

static u32 sgDMA_OneWire_RX_Channel = -1;
static u32 sgDMA_OneWire_TX_Channel = -1;
static OneWire_Config_t* sgOneWire_Config;

// static u32 sgDMA_DHT_Channel = -1;
// static DHT_Config_t* sgDHT_Config;

#define MAX_DMA_CHANNELS 12
static DHT_Config_t* dhtConfigsByChannel[MAX_DMA_CHANNELS] = {NULL};


static void DMA_IRQ0_ISR() {
    // DHT11/22
    // if(sgDMA_DHT_Channel != -1 && dma_channel_get_irq0_status(sgDMA_DHT_Channel)) {
    //     dma_hw->ints0 = (1u << sgDMA_DHT_Channel);
    //     DMA_DHT_ISR();
    // }

    // DHT11/22
    for(u32 channel = 0; channel < MAX_DMA_CHANNELS; ++channel) {
        if(dma_irqn_get_channel_status(0, channel)) {
            dma_irqn_acknowledge_channel(0, channel);

            if(dhtConfigsByChannel[channel] != NULL) {
                DHT_Config_t* config = dhtConfigsByChannel[channel];
                
                BaseType_t xHigherPriorityTaskWoken = pdFALSE;
                if (config->dmaSemaphore != NULL) {
                    xSemaphoreGiveFromISR(config->dmaSemaphore, &xHigherPriorityTaskWoken);
                    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
                }
            }
        }
    }

}

static void DMA_IRQ1_ISR() {
    // UART
    if(sgDMA_UART_Channel != -1 && dma_channel_get_irq1_status(sgDMA_UART_Channel)) {
        dma_hw->ints1 = (1u << sgDMA_UART_Channel);
        DMA_UART_ISR();
    }

    // OneWire
    if(sgDMA_OneWire_RX_Channel != -1 && dma_channel_get_irq1_status(sgDMA_OneWire_RX_Channel)) {
        dma_hw->ints1 = (1u << sgDMA_OneWire_RX_Channel);
        DMA_OneWire_ISR();
    }
    if(sgDMA_OneWire_TX_Channel != -1 && dma_channel_get_irq1_status(sgDMA_OneWire_TX_Channel)) {
        dma_hw->ints1 = (1u << sgDMA_OneWire_TX_Channel);
        DMA_OneWire_ISR();
    }
}


void DMA_Master_Init() {
    irq_set_exclusive_handler(DMA_IRQ_0, DMA_IRQ0_ISR);
    irq_set_exclusive_handler(DMA_IRQ_1, DMA_IRQ1_ISR);

    irq_set_enabled(DMA_IRQ_0, true);
    irq_set_enabled(DMA_IRQ_1, true);
}

void DMA_UART_Register(void* config, u32 channel) {
    sgUART_Config = (UART_Config_t*)config;
    sgDMA_UART_Channel = channel; 
    sgUART_Config = config;
}

void DMA_OneWire_RX_Register(void* config, u32 channel) {
    sgOneWire_Config = (OneWire_Config_t*)config;
    sgDMA_OneWire_RX_Channel = channel; 
    sgOneWire_Config = config;
}
void DMA_OneWire_TX_Register(void* config, u32 channel) {
    sgOneWire_Config = (OneWire_Config_t*)config;
    sgDMA_OneWire_TX_Channel = channel; 
    sgOneWire_Config = config;
}

void DMA_DHT_Register(void* config, u32 channel) {
    // sgDHT_Config = (DHT_Config_t*)config;
    // sgDMA_DHT_Channel = channel;
    // sgDHT_Config = config;

    if(channel < MAX_DMA_CHANNELS) dhtConfigsByChannel[channel] = (DHT_Config_t*)config;
}


void DMA_UART_ISR() {
    if(sgUART_Config != NULL && sgUART_Config->dmaSemaphore != NULL) {
        sgUART_Config->status = UART_DATA_RX_SUCCESS;

        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(sgUART_Config->dmaSemaphore, &xHigherPriorityTaskWoken);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void DMA_OneWire_ISR() {
    if(sgOneWire_Config != NULL) {
        sgOneWire_Config->status = ONEW_DMA_SUCCESS;
    }
}

// void DMA_DHT_ISR() {
//     if(sgDHT_Config != NULL) {
//         sgDHT_Config->status = DHT_READ_SUCCESS;

//         BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//         xSemaphoreGiveFromISR(sgDHT_Config->dmaSemaphore, &xHigherPriorityTaskWoken);

//         portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//     }
// }