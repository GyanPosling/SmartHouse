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
)

# Утилиты
set(UTILS_SOURCES
    utils/src/InputUtils.cpp
)

# Модели
set(MODELS_SOURCES
    models/src/Sensor.cpp
    models/src/User.cpp
    models/src/SmartHome.cpp
    models/src/EnvironmentSimulator.cpp
    models/src/devices/Device.cpp
    models/src/devices/SmartDevice.cpp
    models/src/devices/SmartAirConditioner.cpp
    models/src/devices/SmartHeater.cpp
    models/src/devices/SmartHumidifier.cpp
    models/src/devices/SmartDehumidifier.cpp
    models/src/devices/SmartFan.cpp
    models/src/devices/SmartLight.cpp
    models/src/DeviceContainer.cpp
)

# Репозитории
set(REPOSITORIES_SOURCES
    repositories/src/UserRepository.cpp
    repositories/src/DeviceRepository.cpp
    repositories/src/SensorRepository.cpp
)

# Сервисы
set(SERVICES_SOURCES
    services/src/AuthenticationService.cpp
    services/src/DeviceService.cpp
    services/src/CommandHistory.cpp
    services/src/AutomationService.cpp
)

# Все исходные файлы
set(ALL_SOURCES
    ${EXCEPTIONS_SOURCES}
    ${UTILS_SOURCES}
    ${MODELS_SOURCES}
    ${REPOSITORIES_SOURCES}
    ${SERVICES_SOURCES}
    main/main.cpp
)

# Исполняемый файл
add_executable(smartHouse ${ALL_SOURCES})

# Включаемые директории
target_include_directories(smartHouse PRIVATE
    exceptions/include
    utils/include
    models/include
    repositories/include
    services/include
    main
)
