cmake_minimum_required(VERSION 3.10)
project(SmartHome CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Исключения
set(EXCEPTIONS_SOURCES
    exceptions/src/Exception.cpp
    exceptions/src/InputException.cpp
    exceptions/src/FileException.cpp
    exceptions/src/ContainerException.cpp
    exceptions/src/Date.cpp
    exceptions/src/InputHandler.cpp
)

# Модели
set(MODELS_SOURCES
    models/src/User.cpp
    models/src/devices/Device.cpp
    models/src/devices/SmartDevice.cpp
    models/src/devices/SmartAirConditioner.cpp
    models/src/devices/SmartHeater.cpp
    models/src/devices/SmartHumidifier.cpp
    models/src/devices/SmartDehumidifier.cpp
    models/src/devices/SmartFan.cpp
    models/src/devices/SmartLight.cpp
)

# Репозитории
set(REPOSITORIES_SOURCES
    repositories/src/FileRepository.cpp
)

# Сервисы
set(SERVICES_SOURCES
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
    ${REPOSITORIES_SOURCES}
    ${SERVICES_SOURCES}
    main/main.cpp
    main/Menu.cpp
)

# Исполняемый файл
add_executable(smartHouse ${ALL_SOURCES})

# Включаемые директории
target_include_directories(smartHouse PRIVATE
    exceptions/include
    models/include
    repositories/include
    services/include
    main
)

