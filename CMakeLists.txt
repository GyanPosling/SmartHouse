cmake_minimum_required(VERSION 3.10)
project(SmartHome CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.2 COMPONENTS Widgets REQUIRED)

# Исключения
set(EXCEPTIONS_SOURCES
    exceptions/src/Exception.cpp
    exceptions/src/InputException.cpp
    exceptions/src/FileException.cpp
    exceptions/src/Date.cpp
    exceptions/src/InputHandler.cpp
)

# Модели
set(MODELS_SOURCES
    models/src/User.cpp
    models/src/AddDeviceCommand.cpp
    models/src/RemoveDeviceCommand.cpp
    models/src/devices/Device.cpp
    models/src/devices/SmartDevice.cpp
    models/src/devices/SmartAirConditioner.cpp
    models/src/devices/SmartHeater.cpp
    models/src/devices/SmartHumidifier.cpp
    models/src/devices/SmartDehumidifier.cpp
    models/src/devices/SmartFan.cpp
    models/src/devices/SmartLight.cpp
)

# Сервисы
set(SERVICES_SOURCES
    templates/List.tpp
    services/src/AuthenticationService.cpp
    services/src/DeviceService.cpp
    services/src/CommandHistory.cpp
    services/src/ClimateNormalizer.cpp
    services/src/ClimateData.cpp
)

# Все исходные файлы
set(ALL_SOURCES
    ${EXCEPTIONS_SOURCES}
    ${MODELS_SOURCES}
    ${SERVICES_SOURCES}
    main/main.cpp
    main/Menu.cpp
)

set(GUI_SOURCES
    gui/main.cpp
    gui/MainWindow.cpp
    gui/DeviceDialog.cpp
)

# Исполняемый файл
add_executable(smartHouse ${ALL_SOURCES})

# Включаемые директории
target_include_directories(smartHouse PRIVATE
    exceptions/include
    models/include
    models/include/devices
    services/include
    main
)

add_executable(smartHouse_gui
    ${EXCEPTIONS_SOURCES}
    ${MODELS_SOURCES}
    ${SERVICES_SOURCES}
    ${GUI_SOURCES}
)

target_include_directories(smartHouse_gui PRIVATE
    exceptions/include
    models/include
    models/include/devices
    services/include
    repositories/include
    template/include
    gui
)

target_link_libraries(smartHouse_gui PRIVATE Qt6::Widgets)
