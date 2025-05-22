# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Lenovo/esp/v5.2.1/esp-idf/components/bootloader/subproject"
  "C:/ESP/esp32/gpio/generic_gpio/build/bootloader"
  "C:/ESP/esp32/gpio/generic_gpio/build/bootloader-prefix"
  "C:/ESP/esp32/gpio/generic_gpio/build/bootloader-prefix/tmp"
  "C:/ESP/esp32/gpio/generic_gpio/build/bootloader-prefix/src/bootloader-stamp"
  "C:/ESP/esp32/gpio/generic_gpio/build/bootloader-prefix/src"
  "C:/ESP/esp32/gpio/generic_gpio/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/ESP/esp32/gpio/generic_gpio/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/ESP/esp32/gpio/generic_gpio/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
