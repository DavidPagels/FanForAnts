add_library(triac Triac.cpp)
target_link_libraries(triac pico_stdlib hardware_pio)
target_include_directories(triac PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(fanController FanController.cpp)
target_link_libraries(fanController pico_stdlib hardware_clocks rgbLed zwiftServer triac)
target_include_directories(fanController PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(rgbLed RgbLed.cpp)
target_link_libraries(rgbLed pico_stdlib hardware_pwm)
target_include_directories(rgbLed PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(zwiftServer ZwiftServer.cpp)
target_link_libraries(zwiftServer pico_stdlib pico_cyw43_arch_lwip_threadsafe_background fanController rgbLed)
target_include_directories(zwiftServer PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} pico_stdlib fanController zwiftServer rgbLed triac)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

pico_add_extra_outputs(${PROJECT_NAME})
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_generate_pio_header(triac ${CMAKE_CURRENT_LIST_DIR}/Triac.pio)