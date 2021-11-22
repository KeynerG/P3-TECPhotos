cmake_minimum_required(VERSION 3.20)

project(P3-TECPhotos VERSION 0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt/6.2.0/gcc_64/lib/cmake)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)
include_directories(${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIR})
include_directories("/usr/local/include/mongocxx/v_noabi")
include_directories("/usr/local/include/bsoncxx/v_noabi")
include_directories("/usr/local/include/libmongoc-1.0")
include_directories("/usr/local/include/libbson-1.0")
include_directories("/usr/local/lib")

set(PROJECT_SOURCES
        src/UI/Resources/resources.qrc
        src/main.cpp
        src/UI/TECPhotos.cpp
        src/UI/TECPhotos.h
        src/UI/TECPhotos.ui
        src/Communication/DataManager.cpp
        src/Communication/DataManager.h
        src/HuffmanCoding/HuffmanCompression.cpp
        src/HuffmanCoding/HuffmanCompression.h
        src/RAID5/RAID.cpp
        src/RAID5/RAID.h)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(P3-TECPhotos
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(P3-TECPhotos SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(P3-TECPhotos
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(P3-TECPhotos PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(P3-TECPhotos PRIVATE ${LIBMONGOCXX_LIBRARIES})
target_link_libraries(P3-TECPhotos PRIVATE ${LIBBSONCXX_LIBRARIES})

set_target_properties(P3-TECPhotos PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(P3-TECPhotos)
endif()
