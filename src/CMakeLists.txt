add_executable(${PROJECT_NAME}
        main.cpp 
        MathClass.cpp
        # blink.c
)

target_link_libraries(${PROJECT_NAME} pico_stdlib freertos)
pico_add_extra_outputs(${PROJECT_NAME})

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

