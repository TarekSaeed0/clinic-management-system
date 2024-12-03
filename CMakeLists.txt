cmake_minimum_required(VERSION 3.13.4)
project(
	clinic_management_system
	VERSION 1.0
	LANGUAGES C
	DESCRIPTION "A clinic management system written in c"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(clinic_management_system src/ui.c src/patients.c src/doctors.c src/main.c)
target_include_directories(clinic_management_system PRIVATE include)
target_compile_options(
	clinic_management_system
	PRIVATE -Werror
			-Wall
			-Wextra
			-pedantic
			-Wshadow
			-Wpointer-arith
			-Wcast-align
			-Wstrict-prototypes
			-Wstrict-overflow=5
			-Wwrite-strings
			-Wcast-qual
			-Wconversion
)
