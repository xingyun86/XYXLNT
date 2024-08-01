cmake_minimum_required(VERSION 3.2)
project(xyxlntpyarrow)

if(NOT COMBINED_PROJECT)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../source ${CMAKE_CURRENT_BINARY_DIR}/source)
endif()

add_subdirectory(../third-party/pybind11 pybind11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

find_package(Arrow)

if(NOT ARROW_FOUND)
    message(FATAL_ERROR "Arrow not found.")
endif()

pybind11_add_module(xyxlntpyarrowlib xyxlntpyarrow.lib.cpp)

set_target_properties(xyxlntpyarrowlib PROPERTIES
		OUTPUT_NAME "lib")

target_include_directories(xyxlntpyarrowlib
  	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
  	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../source
  	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third-party/pybind11/include
  	PRIVATE ${ARROW_INCLUDE_DIR})
target_link_libraries(xyxlntpyarrowlib
    PRIVATE xyxlnt)

if(MSVC)
    target_compile_definitions(xyxlntpyarrowlib
        PRIVATE _CRT_SECURE_NO_WARNINGS=1)
    target_link_libraries(xyxlntpyarrowlib
      	PRIVATE ${ARROW_SHARED_IMP_LIB}
      	PRIVATE ${ARROW_PYTHON_SHARED_IMP_LIB})
else()
    target_link_libraries(xyxlntpyarrowlib
        PRIVATE ${ARROW_SHARED_LIB}
        PRIVATE ${ARROW_PYTHON_SHARED_LIB})
endif()

if(NOT STATIC)
    add_custom_command(TARGET xyxlntpyarrowlib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:xyxlnt>
        $<TARGET_FILE_DIR:xyxlntpyarrowlib>)
endif()
