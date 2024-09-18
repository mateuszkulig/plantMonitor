cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(inzynierka)

# Seems like "app" target is hardcoded and shouldn't be changed
target_sources(app PRIVATE
                src/main.c
                src/external_sensors/temp_humid/temp_humid.c
                src/external_sensors/external_sensors.c)
