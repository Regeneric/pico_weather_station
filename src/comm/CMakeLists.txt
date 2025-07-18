add_subdirectory(onewire)
add_subdirectory(network)

add_library(libcomm 
    STATIC
        i2c.c
        uart_dma.c
        dma_irq_handler.c
)


if(BUILD_FOR_ARM)
    add_compile_definitions(HPLATFORM_ARM)
    add_compile_definitions(__arm_gcc__)
elseif(BUILD_FOR_RISCV)
    add_compile_definitions(HPLATFORM_RISCV)
    add_compile_definitions(__riscv_gcc__)
else()
    message(WARNING "Unknown platform — no compile options set")
endif()


target_include_directories(libcomm
    PUBLIC
        ${PROJECT_INC_DIR}
)

target_link_libraries(libcomm
    PRIVATE
        libonewire
        libnetwork

    PUBLIC
        pico_stdlib 
        # pico_lwip
        pico_cyw43_arch_lwip_sys_freertos

        hardware_gpio
        hardware_pio
        hardware_i2c 
        hardware_uart
        hardware_dma
        hardware_irq

        FreeRTOS-Kernel-Heap4
)