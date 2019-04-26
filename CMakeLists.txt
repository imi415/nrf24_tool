cmake_minimum_required(VERSION 3.14)
project(nRF24 C)

set(CMAKE_C_STANDARD 11)

add_definitions(-DHAL_DRIVER=LINUX)
add_compile_options(-g)

include_directories(include)

link_libraries(gpiod pthread)

add_executable(nRF24 main.c src/nrf24_hal.c src/nrf24_driver.c src/nrf24_api.c)
