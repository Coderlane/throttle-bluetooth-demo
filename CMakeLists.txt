# Root CMakeList.txt 

set(PROJECT_NAME "throttle-bluetooth-demo")

cmake_minimum_required(VERSION 2.8.11)
project(${PROJECT_NAME} C)

include(FindPkgConfig)
include(CMakePackageConfigHelpers)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(TBD_EXE "throttle-bluetooth-demo")
set(TBD_SRC "${PROJECT_SOURCE_DIR}/src")
set(TBD_VERSION_MAJOR "0")
set(TBD_VERSION_MINOR "1")
set(TBD_VERSION_PATCH "0")
set(TBD_VERSION 
	"${TBD_VERSION_MAJOR}.${TBD_VERSION_MINOR}.${TBD_VERSION_PATCH}")

# CMake Install Directories
set(CMAKE_INSTALL_BINDIR "bin")

# C flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG -O3")

# Source Code
add_subdirectory(${TBD_SRC})
set(TBD_SOURCE_FILES ${SOURCE_FILES})

# Build and link the executable
add_executable(${TBD_EXE} ${TBD_SOURCE_FILES})

# Config File Settings
set(TBD_INSTALL_PREFIX      "${CMAKE_INSTALL_PREFIX}")
set(TBD_INSTALL_EXEC_PREFIX "${CMAKE_INSTALL_PREFIX}")
set(TBD_INSTALL_BINDIR 
	"${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")
set(TBD_INSTALL_INCLUDEDIR 
	"${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")

# Set install targets
install(TARGETS ${TBD_EXE} 
	DESTINATION ${CMAKE_INSTALL_BINDIR})

# build a CPack driven installer package
include(InstallRequiredSystemLibraries)
# CPack Options
set(CPACK_PACKAGE_CONTACT "" CACHE STRING 
	"The contact information for the package distributor")
set(CPACK_TARGET_ARCH "armhf" CACHE STRING "Target architecture.")
set(CPACK_TARGET_OS "Linux" CACHE STRING "Target operating system.")
# CPack Settings 
SET(CPACK_GENERATOR "DEB")
SET(CPACK_SOURCE_GENERATOR "TGZ;TBZ2;ZIP")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README  "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_VERSION_MAJOR "${TBD_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${TBD_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${TBD_VERSION_PATCH}")
SET(CPACK_SYSTEM_NAME "${CPACK_TARGET_OS}-${CPACK_TARGET_ARCH}")
SET(CPACK_TOPLEVEL_TAG "${CPACK_TARGET_OS}-${CPACK_TARGET_ARCH}")
include(CPack)
