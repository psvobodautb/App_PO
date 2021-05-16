cmake_minimum_required(VERSION 3.5)

project(poApp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED Sql)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED Sql)

set(SOURCE_LIST
    src/main.cpp
    src/mainwindow.cpp

    include/enums.h

    src/widgets/subjectswidget.cpp
    src/widgets/employeeswidget.cpp
    src/widgets/groupswidget.cpp
    src/widgets/labelswidget.cpp
    src/widgets/connectingwidget.cpp

    include/mainwindow.h
    include/functions.h
    include/widgets/subjectswidget.h
    include/widgets/employeeswidget.h
    include/widgets/groupswidget.h
    include/widgets/labelswidget.h
    include/widgets/connectingwidget.h

    src/models/employeemodel.cpp
    src/models/groupmodel.cpp
    src/models/labelmodel.cpp
    src/models/subjectmodel.cpp

    include/models/employeemodel.h
    include/models/groupmodel.h
    include/models/labelmodel.h
    include/models/subjectmodel.h
)

if(ANDROID)
  add_library(poApp SHARED
    ${SOURCE_LIST}
  )
else()
  add_executable(poApp WIN32
    ${SOURCE_LIST}
    resources/stylesheet.qrc
  )
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
target_link_libraries(poApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)
