cmake_minimum_required(VERSION 3.28.2)
project(
  clinic_management_system
  VERSION 1.0
  LANGUAGES C
  DESCRIPTION "Clinic Management System GTK Project")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
pkg_check_modules(GMODULE_EXPORT REQUIRED IMPORTED_TARGET gmodule-export-2.0)

find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/clinic_management_system.gresource.c
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND
    ${GLIB_COMPILE_RESOURCES} data/clinic_management_system.gresource.xml
    --target=${CMAKE_CURRENT_BINARY_DIR}/clinic_management_system.gresource.c
    --sourcedir=data/clinic_management_system --generate-source
  VERBATIM
  MAIN_DEPENDENCY data/clinic_management_system.gresource.xml
  DEPENDS
    data/clinic_management_system/ui/window.ui
    data/clinic_management_system/ui/start_page.ui
    data/clinic_management_system/ui/sign_up_page.ui
    data/clinic_management_system/ui/sign_in_page.ui
    data/clinic_management_system/ui/main_page.ui
    data/clinic_management_system/ui/doctors_list_item.ui
    data/clinic_management_system/ui/doctors_list.ui
    data/clinic_management_system/ui/list_doctors_page.ui
    data/clinic_management_system/ui/search_doctors_page.ui
    data/clinic_management_system/style.css
    data/clinic_management_system/icons/hicolor/48x48/apps/com.github.TarekSaeed0.clinic_management_system.png
    data/clinic_management_system/icons/hicolor/scalable/apps/com.github.TarekSaeed0.clinic_management_system.svg
	data/clinic_management_system/images/logo.png
)

add_custom_target(
  clinic_management_system_compile_gresources ALL
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/clinic_management_system.gresource.c)

find_program(DESKTOP_FILE_VALIDATE NAMES desktop-file-validate)

if(DESKTOP_FILE_VALIDATE)
  add_custom_command(
    OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/com.github.TarekSaeed0.clinic_management_system.desktop.validated
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND
      ${DESKTOP_FILE_VALIDATE}
      data/clinic_management_system/applications/com.github.TarekSaeed0.clinic_management_system.desktop
    COMMAND
      ${CMAKE_COMMAND} -E touch
      ${CMAKE_CURRENT_BINARY_DIR}/com.github.TarekSaeed0.clinic_management_system.desktop.validated
    VERBATIM
    MAIN_DEPENDENCY
      data/clinic_management_system/applications/com.github.TarekSaeed0.clinic_management_system.desktop
  )

  add_custom_target(
    clinic_management_system_validate_desktop_file ALL
    DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/com.github.TarekSaeed0.clinic_management_system.desktop.validated
  )
endif()

add_executable(
  clinic_management_system
  ${CMAKE_CURRENT_BINARY_DIR}/clinic_management_system.gresource.c
  src/clinic_management_system/patients.c
  src/clinic_management_system/doctors.c
  src/clinic_management_system/ui/start_page.c
  src/clinic_management_system/ui/sign_up_page.c
  src/clinic_management_system/ui/sign_in_page.c
  src/clinic_management_system/ui/main_page.c
  src/clinic_management_system/ui/doctors_list.c
  src/clinic_management_system/ui/list_doctors_page.c
  src/clinic_management_system/ui/search_doctors_page.c
  src/clinic_management_system/application.c
  src/main.c)
target_sources(
  clinic_management_system
  PRIVATE
    data/clinic_management_system/icons/com.github.TarekSaeed0.clinic_management_system.rc
)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(clinic_management_system
                      PRIVATE PkgConfig::GTK4 PkgConfig::GMODULE_EXPORT)
target_compile_options(
  clinic_management_system
  PRIVATE -Werror
          -Wall
          -Wextra
          -pedantic
          -Wfloat-equal
          -Wundef
          -Wshadow
          -Wpointer-arith
          -Wcast-align
          -Wstrict-prototypes
          -Wstrict-overflow=5
          -Wwrite-strings
          -Wcast-qual
          -Wconversion
          -Wno-overlength-strings)
if(WIN32)
  target_compile_options(clinic_management_system PRIVATE -mwindows)
  target_link_options(clinic_management_system PRIVATE -Wl,-subsystem,console)
endif()

install(TARGETS clinic_management_system)
install(DIRECTORY data/clinic_management_system/icons
                  data/clinic_management_system/applications TYPE DATA)
