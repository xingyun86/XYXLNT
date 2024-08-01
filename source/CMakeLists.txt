cmake_minimum_required(VERSION 3.1)
project(xyxlnt VERSION 1.5.0)

set(CMAKE_CXX_STANDARD ${XYXLNT_CXX_LANG})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)

# Project metadata
set(PROJECT_VENDOR "xingyun86")
set(PROJECT_CONTACT "375182886@qq.com")
set(PROJECT_URL "https://github.com/xingyun86/XYXLNT")
set(PROJECT_DESCRIPTION "cross-platform xlsx library")

# Commonly used project directories
set(XYXLNT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(XYXLNT_INCLUDE_DIR ${XYXLNT_ROOT_DIR}/include)
set(XYXLNT_SOURCE_DIR ${XYXLNT_ROOT_DIR}/source)
set(THIRD_PARTY_DIR ${XYXLNT_ROOT_DIR}/third-party)

# Include libstudxml library
add_subdirectory(${THIRD_PARTY_DIR}/libstudxml.build
  ${CMAKE_CURRENT_BINARY_DIR}/third-party/libstudxml)

if(COVERAGE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

# Non-target-specific compiler settings
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4") # level 4 warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") # multi-processor compilation
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall") # all warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra") # extra warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas") # ignore MSVC and Clang pragmas
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-maybe-uninitialized") # GCC diagnostic with lots of false positives
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything") # all warnings
  # blacklist warnings that are not relevant
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat") # ignore warnings about C++98 compatibility
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat-pedantic") # ignore pedantic warnings about C++98 compatibility
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-padded") # ignore padding warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-documentation-unknown-command") # ignore unknown commands in Javadoc-style comments
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas") # ignore Windows and GCC pragmas
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option") # ignore Windows and GCC pragmas
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-newline-eof") # no longer an issue with post-c++11 standards which mandate include add a newline if neccesary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-covered-switch-default") # default is often added to switches for completeness or to cover future alternatives
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-exit-time-destructors") # this is just a warning to notify that the destructor will run during exit
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-braces") # Wmissing-field-initializers has less false positives
endif()

if(STATIC_CRT)
  include(${XYXLNT_ROOT_DIR}/cmake/ucm.cmake)
  ucm_set_runtime(STATIC)
endif()

if(APPLE)
  # Prevent a warning about deployment target not being set by setting it to current OSX version
  execute_process(COMMAND "sw_vers -productVersion | awk -F'.' '{print $1\".\"$2}'"
    OUTPUT_VARIABLE OSX_VERSION)
  set(CMAKE_OSX_DEPLOYMENT_TARGET ${OSX_VERSION})
endif()

file(GLOB ROOT_HEADERS ${XYXLNT_INCLUDE_DIR}/xyxlnt/*.hpp)
file(GLOB CELL_HEADERS ${XYXLNT_INCLUDE_DIR}/xyxlnt/cell/*.hpp)
file(GLOB CELL_SOURCES ${XYXLNT_SOURCE_DIR}/cell/*.cpp)
file(GLOB DRAWING_HEADERS ${XYXLNT_INCLUDE_DIR}/xyxlnt/drawing/*.hpp)
file(GLOB DRAWING_SOURCES ${XYXLNT_SOURCE_DIR}/drawing/*.cpp)
file(GLOB CHARTS_HEADERS ${XYXLNT_INCLUDE_DIR}/xyxlnt/charts/*.hpp)
file(GLOB CHARTS_SOURCES ${XYXLNT_SOURCE_DIR}/charts/*.cpp)
file(GLOB CHARTSHEET_HEADERS ${XYXLNT_INCLUDE_DIR}/xyxlnt/chartsheet/*.hpp)
file(GLOB CHARTSHEET_SOURCES ${XYXLNT_SOURCE_DIR}/chartsheet/*.cpp)
file(GLOB DRAWING_HEADERS ${XYXLNT_INCLUDE_DIR}/xyxlnt/drawing/*.hpp)
file(GLOB DRAWING_SOURCES ${XYXLNT_SOURCE_DIR}/drawing/*.cpp)
file(GLOB FORMULA_HEADERS ${XYXLNT_INCLUDE_DIR}/xyxlnt/formula/*.hpp)
file(GLOB FORMULA_SOURCES ${XYXLNT_SOURCE_DIR}/formula/*.cpp)
file(GLOB PACKAGING_HEADERS ${XYXLNT_INCLUDE_DIR}/xyxlnt/packaging/*.hpp)
file(GLOB PACKAGING_SOURCES ${XYXLNT_SOURCE_DIR}/packaging/*.cpp)
file(GLOB STYLES_HEADERS ${XYXLNT_INCLUDE_DIR}/xyxlnt/styles/*.hpp)
file(GLOB STYLES_SOURCES ${XYXLNT_SOURCE_DIR}/styles/*.cpp)
file(GLOB UTILS_HEADERS ${XYXLNT_INCLUDE_DIR}/xyxlnt/utils/*.hpp)
file(GLOB UTILS_SOURCES ${XYXLNT_SOURCE_DIR}/utils/*.cpp)
file(GLOB WORKBOOK_HEADERS ${XYXLNT_INCLUDE_DIR}/xyxlnt/workbook/*.hpp)
file(GLOB WORKBOOK_SOURCES ${XYXLNT_SOURCE_DIR}/workbook/*.cpp)
file(GLOB WORKSHEET_HEADERS ${XYXLNT_INCLUDE_DIR}/xyxlnt/worksheet/*.hpp)
file(GLOB WORKSHEET_SOURCES ${XYXLNT_SOURCE_DIR}/worksheet/*.cpp)
file(GLOB MINIZ_HEADERS ${THIRD_PARTY_DIR}/miniz/*.h)
file(GLOB MINIZ_SOURCES ${THIRD_PARTY_DIR}/miniz/*.c)

file(GLOB DETAIL_ROOT_HEADERS ${XYXLNT_SOURCE_DIR}/detail/*.hpp)
file(GLOB DETAIL_ROOT_SOURCES ${XYXLNT_SOURCE_DIR}/detail/*.cpp)
file(GLOB DETAIL_CRYPTOGRAPHY_HEADERS ${XYXLNT_SOURCE_DIR}/detail/cryptography/*.hpp)
file(GLOB DETAIL_CRYPTOGRAPHY_SOURCES ${XYXLNT_SOURCE_DIR}/detail/cryptography/*.c*)
file(GLOB DETAIL_EXTERNAL_HEADERS ${XYXLNT_SOURCE_DIR}/detail/external/*.hpp)
file(GLOB DETAIL_HEADER_FOOTER_HEADERS ${XYXLNT_SOURCE_DIR}/detail/header_footer/*.hpp)
file(GLOB DETAIL_HEADER_FOOTER_SOURCES ${XYXLNT_SOURCE_DIR}/detail/header_footer/*.cpp)
file(GLOB DETAIL_IMPLEMENTATIONS_HEADERS ${XYXLNT_SOURCE_DIR}/detail/implementations/*.hpp)
file(GLOB DETAIL_IMPLEMENTATIONS_SOURCES ${XYXLNT_SOURCE_DIR}/detail/implementations/*.cpp)
file(GLOB DETAIL_NUMBER_FORMAT_HEADERS ${XYXLNT_SOURCE_DIR}/detail/number_format/*.hpp)
file(GLOB DETAIL_NUMBER_FORMAT_SOURCES ${XYXLNT_SOURCE_DIR}/detail/number_format/*.cpp)
file(GLOB DETAIL_SERIALIZATION_HEADERS ${XYXLNT_SOURCE_DIR}/detail/serialization/*.hpp)
file(GLOB DETAIL_SERIALIZATION_SOURCES ${XYXLNT_SOURCE_DIR}/detail/serialization/*.cpp)


set(DETAIL_HEADERS ${DETAIL_ROOT_HEADERS} ${DETAIL_CRYPTOGRAPHY_HEADERS}
  ${DETAIL_EXTERNAL_HEADERS} ${DETAIL_HEADER_FOOTER_HEADERS}
  ${DETAIL_IMPLEMENTATIONS_HEADERS} ${DETAIL_NUMBER_FORMAT_HEADERS}
  ${DETAIL_SERIALIZATION_HEADERS})
set(DETAIL_SOURCES ${DETAIL_ROOT_SOURCES} ${DETAIL_CRYPTOGRAPHY_SOURCES}
  ${DETAIL_EXTERNAL_SOURCES} ${DETAIL_HEADER_FOOTER_SOURCES}
  ${DETAIL_IMPLEMENTATIONS_SOURCES} ${DETAIL_NUMBER_FORMAT_SOURCES}
  ${DETAIL_SERIALIZATION_SOURCES})

set(XYXLNT_HEADERS ${ROOT_HEADERS} ${CELL_HEADERS} ${CHARTS_HEADERS}
  ${CHARTSHEET_HEADERS} ${DRAWING_HEADERS} ${FORMULA_HEADERS}
  ${PACKAGING_HEADERS} ${STYLES_HEADERS} ${UTILS_HEADERS}
  ${WORKBOOK_HEADERS} ${WORKSHEET_HEADERS} ${DETAIL_HEADERS} ${DETAIL_CRYPTO_HEADERS}
  ${DRAWING_HEADERS} ${MINIZ_HEADERS})
set(XYXLNT_SOURCES ${CELL_SOURCES} ${CHARTS_SOURCES} ${CHARTSHEET_SOURCES}
  ${DRAWING_SOURCES} ${FORMULA_SOURCES} ${PACKAGING_SOURCES}
  ${STYLES_SOURCES} ${UTILS_SOURCES} ${WORKBOOK_SOURCES}
  ${WORKSHEET_SOURCES} ${DETAIL_SOURCES} ${DETAIL_CRYPTO_SOURCES}
  ${DRAWING_SOURCES} ${MINIZ_SOURCES})

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  # Set a default CMAKE_INSTALL_PREFIX if one wasn't specified

  if(MSVC)
    # No good place to install on Windows so just install to cmake build directory in /installed
    set(DEFAULT_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/installed")
  else()
    # Install to /usr/local on other platforms
    set(DEFAULT_INSTALL_PREFIX "/usr/local")
  endif()

  # Apply the variable and save to cache
  set(CMAKE_INSTALL_PREFIX ${DEFAULT_INSTALL_PREFIX}
    CACHE PATH "default install path" FORCE)
endif()

# Append "d" to the name of the compiled library
#set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Set debug library postfix")
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "Set debug library postfix")

include(GNUInstallDirs)

set(XYXLNT_INC_DEST_DIR ${CMAKE_INSTALL_INCLUDEDIR}
	CACHE PATH "Default location to install include files")
set(XYXLNT_LIB_DEST_DIR ${CMAKE_INSTALL_LIBDIR}
	CACHE PATH "Default location to install library files")
set(XYXLNT_BIN_DEST_DIR ${CMAKE_INSTALL_BINDIR}
	CACHE PATH "Default location to install runtime files")
set(XYXLNT_MAN_DEST_DIR ${CMAKE_INSTALL_MANDIR}
	CACHE PATH "Default location to install runtime files")
set(XYXLNT_CMAKE_CFG_DEST_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	CACHE PATH "Default location to install CMake config files")

if(NOT STATIC)
  # Compile shared library
  add_library(xyxlnt SHARED
    ${XYXLNT_HEADERS}
    ${XYXLNT_SOURCES}
    $<TARGET_OBJECTS:libstudxml>)

  target_compile_definitions(xyxlnt PRIVATE XYXLNT_SHARED=1)

  # Set SO version (for symbolic links like libxyxlnt.so.1.4 and libxyxlnt.so.1)
  set_target_properties(xyxlnt
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    INSTALL_NAME_DIR "${XYXLNT_LIB_DEST_DIR}")
else()
  # Compile static library
  add_library(xyxlnt STATIC ${XYXLNT_HEADERS} ${XYXLNT_SOURCES} $<TARGET_OBJECTS:libstudxml>)
  target_compile_definitions(xyxlnt PUBLIC XYXLNT_STATIC=1)
endif()

# requires cmake 3.8+
#target_compile_features(xyxlnt PUBLIC cxx_std_${XYXLNT_CXX_LANG})

# Includes
target_include_directories(xyxlnt
	PUBLIC
	$<BUILD_INTERFACE:${XYXLNT_INCLUDE_DIR}>
	$<INSTALL_INTERFACE:${XYXLNT_INC_DEST_DIR}>
	PRIVATE
	${XYXLNT_SOURCE_DIR}
	${XYXLNT_SOURCE_DIR}/../third-party/libstudxml
    ${XYXLNT_SOURCE_DIR}/../third-party/miniz
	${XYXLNT_SOURCE_DIR}/../third-party/utfcpp)

# Platform- and file-specific settings, MSVC
if(MSVC)
  target_compile_definitions(xyxlnt PRIVATE _CRT_SECURE_NO_WARNINGS=1)

  if(NOT STATIC)
    target_compile_definitions(xyxlnt PRIVATE XYXLNT_EXPORT=1)
  endif()

  set_target_properties(xyxlnt PROPERTIES
    COMPILE_FLAGS "/wd\"4251\" /wd\"4275\" /wd\"4068\"")
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/detail/serialization/miniz.cpp
    PROPERTIES
    COMPILE_FLAGS "/wd\"4244\" /wd\"4334\" /wd\"4127\"")
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/detail/cryptography/aes.cpp
    PROPERTIES
    COMPILE_FLAGS "/wd\"4996\"")
else()
  # Platform- and file-specific settings, Clang
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/detail/serialization/miniz.cpp
	  PROPERTIES
	  COMPILE_FLAGS "-Wno-undef")
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/detail/serialization/zstream.cpp
	  PROPERTIES
      COMPILE_FLAGS "-Wno-undef -Wno-shorten-64-to-32")
  endif()
  
  # Platform- and file-specific settings, GCC
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/detail/serialization/miniz.cpp
	  PROPERTIES
	  COMPILE_FLAGS "-Wno-strict-aliasing")
  endif()
endif()

# Group files into pseudo-folders in IDEs
source_group(xyxlnt FILES ${ROOT_HEADERS})
source_group(cell FILES ${CELL_HEADERS} ${CELL_SOURCES})
source_group(detail FILES ${DETAIL_ROOT_HEADERS} ${DETAIL_ROOT_SOURCES})
source_group(detail\\cryptography FILES ${DETAIL_CRYPTOGRAPHY_HEADERS} ${DETAIL_CRYPTOGRAPHY_SOURCES})
source_group(detail\\external FILES ${DETAIL_EXTERNAL_HEADERS})
source_group(detail\\header_footer FILES ${DETAIL_HEADER_FOOTER_HEADERS} ${DETAIL_HEADER_FOOTER_SOURCES})
source_group(detail\\implementations FILES ${DETAIL_IMPLEMENTATIONS_HEADERS} ${DETAIL_IMPLEMENTATIONS_SOURCES})
source_group(detail\\number_format FILES ${DETAIL_NUMBER_FORMAT_HEADERS} ${DETAIL_NUMBER_FORMAT_SOURCES})
source_group(detail\\serialization FILES ${DETAIL_SERIALIZATION_HEADERS} ${DETAIL_SERIALIZATION_SOURCES})
source_group(drawing FILES ${DRAWING_HEADERS} ${DRAWING_SOURCES})
source_group(packaging FILES ${PACKAGING_HEADERS} ${PACKAGING_SOURCES})
source_group(styles FILES ${STYLES_HEADERS} ${STYLES_SOURCES})
source_group(utils FILES ${UTILS_HEADERS} ${UTILS_SOURCES})
source_group(workbook FILES ${WORKBOOK_HEADERS} ${WORKBOOK_SOURCES})
source_group(worksheet FILES ${WORKSHEET_HEADERS} ${WORKSHEET_SOURCES})
source_group(third-party\\miniz FILES ${MINIZ_HEADERS} ${MINIZ_SOURCES})

# Install library
install(TARGETS xyxlnt EXPORT XYXlntTargets
  LIBRARY DESTINATION ${XYXLNT_LIB_DEST_DIR}
  ARCHIVE DESTINATION ${XYXLNT_LIB_DEST_DIR}
  RUNTIME DESTINATION ${XYXLNT_BIN_DEST_DIR})

include(CMakePackageConfigHelpers)

set(XYXLNT_INCLUDE_INSTALL_DIR ${XYXLNT_INC_DEST_DIR})
if(CMAKE_INSTALL_PREFIX)
  set(XYXLNT_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${XYXLNT_INCLUDE_INSTALL_DIR})
endif()

# Install include directory
install(DIRECTORY ${XYXLNT_INCLUDE_DIR}/xyxlnt
  DESTINATION ${XYXLNT_INC_DEST_DIR}
  PATTERN ".DS_Store" EXCLUDE)

if(NOT TARGET uninstall)
  # Configure uninstall
  configure_file("${XYXLNT_ROOT_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  # Add uninstall target
  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P
      ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

if(NOT MSVC)
  # Set pkg-config variables
  set(PKG_CONFIG_LIBDIR ${XYXLNT_LIB_DEST_DIR})
  set(PKG_CONFIG_INCLUDEDIR ${XYXLNT_INC_DEST_DIR})
  set(PKG_CONFIG_LIBS "-L\${libdir} -lxyxlnt")
  set(PKG_CONFIG_CFLAGS "-I\${includedir}")

  # Replace variables in pkg-config template
  configure_file("${XYXLNT_ROOT_DIR}/cmake/pkg-config.pc.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/xyxlnt.pc")

  # pkg-config install
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/xyxlnt.pc"
    DESTINATION ${XYXLNT_LIB_DEST_DIR}/pkgconfig)
endif()

if(RELEASE)
  set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
  set(CPACK_GENERATOR "TGZ")
  set(CPACK_PACKAGE_NAME "xyxlnt")
  set(CPACK_SUFFIX "${CMAKE_SYSTEM_NAME}")
  if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    string(CONCAT CPACK_SUFFIX ${CPACK_SUFFIX} "-x86")
  else()
    string(CONCAT CPACK_SUFFIX ${CPACK_SUFFIX} "-x64")
  endif()
  if(STATIC)
    string(CONCAT CPACK_SUFFIX ${CPACK_SUFFIX} "-static")
  endif()
  string(TOLOWER "${CPACK_SUFFIX}" CPACK_SUFFIX)
  set(CPACK_SYSTEM_NAME "${CPACK_SUFFIX}")
  include(CPack)
endif()
