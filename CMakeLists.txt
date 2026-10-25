cmake_minimum_required(VERSION 3.16)
project(easyskan VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Automatická správa Meta-Object Compileru (MOC) pro signály a sloty Qt6
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 1. NAJDEME POTŘEBNÉ MODULY PRO QT6 (VČETNĚ PRINTSUPPORT PRO PDF)
find_package(Qt6 COMPONENTS Core Gui Widgets PrintSupport REQUIRED)

# 2. ROVNÉ VYHLEDÁNÍ HARDWAROVÉ SANE KNIHOVNY VE FEDOŘE
find_path(SANE_INCLUDE_DIR NAMES sane/sane.h)
find_library(SANE_LIBRARY NAMES sane)

if(NOT SANE_INCLUDE_DIR OR NOT SANE_LIBRARY)
    message(FATAL_ERROR "SANE vývojové hlavičky nebo knihovny nebyly v systému nalezeny!")
endif()

# Definice zdrojových souborů naší skenovací linky
set(SOURCES
    main.cpp
    mainwindow.cpp
    skenwindow.cpp
)

# Definice spustitelné aplikace - kompletně malými písmeny!
add_executable(easyskan ${SOURCES})

# 3. PROPOJENÍ S QT6 KNIHOVNAMI A HARDWAROVÝM SANE SUBSYSTÉMEM
target_link_libraries(easyskan PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::PrintSupport
    ${SANE_LIBRARY}
)

# Přibalíme vyhledávací cesty pro hlavičky SANE
target_include_directories(easyskan PRIVATE
    ${SANE_INCLUDE_DIR}
)

# --- FINÁLNÍ FIX: INSTALACE TARGETU S OPRAVENÝM MALÝM NÁZVEM ---
install(TARGETS easyskan DESTINATION bin)

# --- AUTOMATICKÁ GENERACE PŘEKLADOVÝCH SOUBORŮ QT6 ---
find_package(Qt6 COMPONENTS LinguistTools REQUIRED)
qt_add_translations(easyskan
    TS_FILES easyskan_en.ts
)

# --- POCTIVÁ INSTALACE PODLE ARCHIVNÍCH STANDARDŮ GNU ---
include(GNUInstallDirs)
install(TARGETS easyskan DESTINATION ${CMAKE_INSTALL_BINDIR})
