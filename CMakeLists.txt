cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_COMPILER "C:\\MinGW\\bin\\g++.exe")
set(CMAKE_C_COMPILER "C:\\MinGW\\bin\\gcc.exe")

project(CMakeSFMLProject LANGUAGES CXX)
message(TEST="${CYGWIN}")
message(TEST="${MSYS}")
message(TEST="${MINGW}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(CMakeSFMLProject src/main.cpp)
target_link_libraries(CMakeSFMLProject PRIVATE sfml-graphics)
target_compile_features(CMakeSFMLProject PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET CMakeSFMLProject
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:CMakeSFMLProject>
        VERBATIM)
endif()

install(TARGETS CMakeSFMLProject)

add_custom_command(
	OUTPUT CMakeSFMLProject.lic
	COMMAND CMakeSFMLProject
	DEPENDS any/dependency.file
	VERBATIM
)

add_custom_target(
	run ALL
	DEPENDS CMakeSFMLProject.lic
)