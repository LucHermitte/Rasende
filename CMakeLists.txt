CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)
# The minimum version required doesn't seem to disable cygwin warning.
# Sorry, I can't do anything about that.
# set(CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required

#======================================================================
# Version
SET(RASENDE_VERSION_MAJOR "1")
SET(RASENDE_VERSION_MINOR "2")
SET(RASENDE_VERSION_PATCH "0")

SET(RASENDE_VERSION_STRING "${RASENDE_VERSION_MAJOR}.${RASENDE_VERSION_MINOR}.${RASENDE_VERSION_PATCH}")




#======================================================================
# COTS
# set(Boost_DEBUG 1)
# set(Boost_ADDITIONAL_VERSIONS "1.47" "1.47.0" "1.47.1" "1.51" "1.51.0" "1.51.1")
# set(Boost_DEBUG TRUE)
# set(Boost_DETAILED_FAILURE_MSG TRUE)
# find_package(Boost)
# IF(Boost_FOUND)
    # MESSAGE(STATUS "Boost_INCLUDE_DIR: ${Boost_INCLUDE_DIR}")
    # MESSAGE(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
    # MESSAGE(STATUS "Boost_FILESYSTEM_LIBRARY: ${Boost_FILESYSTEM_LIBRARY}")
    # MESSAGE(STATUS "Boost_DATE_TIME_LIBRARY: ${Boost_DATE_TIME_LIBRARY}")
# ELSE(Boost_FOUND)
  # MESSAGE(STATUS "Boost_DIR is: $ENV{Boost_DIR}")
  # MESSAGE(FATAL_ERROR
      # "Cannot build RASENDE without Boost.  Please set Boost_DIR.")
# ENDIF(Boost_FOUND)


# set(COTS_INC "${CMAKE_CURRENT_SOURCE_DIR}/cots")
set(SRC_INC "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INPUT_DATA "${CMAKE_CURRENT_SOURCE_DIR}/data")

#======================================================================
# C++11
# Using: http://pageant.ghulbus.eu/?p=664
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")
include(CMake/CheckCXX11Features.cmake)
# Check a blocking bug from GNU libstdc++ implementation of std::bitset
include(CMake/CheckBitSetMaxSize.cmake)

# GCC specifics
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -std=c++0x")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb")
endif()
# Visual Studio specifics
if(MSVC)
  # Warning Level 4 for Debug builds
  list(APPEND CMAKE_CXX_FLAGS_DEBUG " /W4")
  list(REMOVE_DUPLICATES CMAKE_CXX_FLAGS_DEBUG)
endif()

CONFIGURE_FILE( ${SRC_INC}/config.h.cmakein ${CMAKE_CURRENT_BINARY_DIR}/config.h)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

#======================================================================
# Sub-projects
# - lib 
add_subdirectory(src)

# - tests
# OPTION(RASENDE_BUILD_TESTING  "Build testing." OFF)
# if(RASENDE_BUILD_TESTING)
    # find_package(Boost COMPONENTS unit_test_framework)
    # IF (NOT Boost_UNIT_TEST_FRAMEWORK_FOUND)
        # MESSAGE(FATAL_ERROR
            # "Cannot build RASENDE without a version of Boost that supports unit testing.  Please set Boost_DIR to the proper version of boost.")
    # ENDIF(NOT Boost_UNIT_TEST_FRAMEWORK_FOUND)
    # MESSAGE(STATUS "Boost_UNIT_TEST_FRAMEWORK_LIBRARY: ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")


    # ENABLE_TESTING()
    # include(CTest)
    # add_subdirectory(tests)
# endif(RASENDE_BUILD_TESTING)

#======================================================================
# CPack
OPTION(RASENDE_USE_CPACK "Generate installer using CPack" OFF)
MARK_AS_ADVANCED(RASENDE_USE_CPACK)

# variables also used for doxygen
SET(RASENDE_PACKAGE_NAME "Rasende-Roboter-Solver" CACHE STRING "Package name")
SET(RASENDE_PACKAGE_DESCRIPTION_SUMMARY "Solver for Rasende Roboter puzzles")
SET(RASENDE_PACKAGE_VENDOR "Luc Hermitte")

IF(RASENDE_USE_CPACK)
    # The following components are regex's to match anywhere (unless anchored)
    # in absolute path + filename to find files or directories to be excluded
    # from source tarball.
    # set(CPACK_SOURCE_IGNORE_FILES
        # "~$"
        # "\\.git$"
        # "^${PROJECT_SOURCE_DIR}.*/.git/"
        # "\\.swp$"
        # "\\.swo$"
        # )
    #message("CPACK_SOURCE_IGNORE_FILES = ${CPACK_SOURCE_IGNORE_FILES}")



# FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/AllVariables.txt "")
# GET_CMAKE_PROPERTY(res VARIABLES)
# FOREACH(var ${res})
  # FILE(APPEND ${CMAKE_CURRENT_BINARY_DIR}/AllVariables.txt 
             # "V:${var} \"${${var}}\"\n")
# ENDFOREACH(var ${res})
# GET_CMAKE_PROPERTY(res CACHE_VARIABLES)
# FOREACH(var ${res})
  # FILE(APPEND ${CMAKE_CURRENT_BINARY_DIR}/AllVariables.txt 
      # "C:${var} \"${${var}}\"\n")
# ENDFOREACH(var ${res})


    INCLUDE(InstallRequiredSystemLibraries)
    
    SET(CPACK_PACKAGE_NAME "${RASENDE_PACKAGE_NAME}" CACHE STRING "Package name")
    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${RASENDE_PACKAGE_DESCRIPTION_SUMMARY}")
    SET(CPACK_PACKAGE_VENDOR "${RASENDE_PACKAGE_VENDOR}")
    MARK_AS_ADVANCED(CPACK_PACKAGE_NAME)
    MARK_AS_ADVANCED(CPACK_PACKAGE_DESCRIPTION_SUMMARY)
    
    SET(CPACK_PACKAGE_VERSION  "${RASENDE_VERSION_STRING}")
    SET(CPACK_PACKAGE_VERSION_MAJOR "${RASENDE_VERSION_MAJOR}")
    SET(CPACK_PACKAGE_VERSION_MINOR "${RASENDE_VERSION_MINOR}")
    SET(CPACK_PACKAGE_VERSION_PATCH "${RASENDE_VERSION_PATCH}")
    SET(CPACK_PACKAGE_CONTACT "luc.hermitte {at} gmail.com" CACHE STRING  "Luc Hermitte contact email")
    # SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../Description.txt")
    
    # SET(CPACK_PACKAGE_INSTALL_DIRECTORY
        # "RASENDE-Familles-${RASENDE_VERSION_MAJOR}.${RASENDE_VERSION_MINOR}")
    
    # SET(CPACK_RESOURCE_FILE_LICENSE
        # "${CMAKE_CURRENT_SOURCE_DIR}/../../Copyright/RouilleCopyright.txt")
    
    INCLUDE(CPack)

ENDIF(RASENDE_USE_CPACK)

#======================================================================
# Doxygen
# http://majewsky.wordpress.com/2010/08/14/tip-of-the-day-cmake-and-doxygen/
# add a target to generate API documentation with Doxygen
function(do_fix_cygwin_paths convertion)
    find_program(CYGPATH cygpath)
    if (NOT CYGPATH)
        message(FATAL_ERROR "cannot find cygpath: thus cannot use doxygen")
    endif (NOT CYGPATH)
    foreach(path_var ${ARGN})
        execute_process(COMMAND
            ${CYGPATH} ${convertion} "${${path_var}}"
            RESULT_VARIABLE cygpath_result
            ERROR_VARIABLE cygpath_err
            OUTPUT_VARIABLE ${path_var}
            )
        if (cygpath_result OR cygpath_err)
            message(FATAL_ERROR "*** ${CYGPATH} failed: ***\n${cygpath_err}")
        endif (cygpath_result OR cygpath_err)
        string(REGEX REPLACE "[\r\n]" "" ${path_var} "${${path_var}}")
        # propagate the result to the parent scope
        set(${path_var} "${${path_var}}" PARENT_SCOPE)
        # message("converting ${path_var} ${convertion} -> ${${path_var}}")
    endforeach()
endfunction()

find_package(Doxygen)
if(DOXYGEN_FOUND)
    find_package(Cygwin)
    # Variables to substitute in Doxyfile.in
    set(OUTPUT_DOXYGEN_DIRECTORY ${PROJECT_BINARY_DIR}/dox)
    set(INPUT_DOXYGEN_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
    set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    # Special case for cygwin when cmake doesn't match doxygen (i.e. when one
    # of the two is the win32 version while the other one has been compiled
    # for cygwin)
    string(REGEX MATCH "cygwin" DOX_CYGWIN_MATCH "${DOXYGEN_EXECUTABLE}")
    IF (DOX_CYGWIN_MATCH) # todo: it should be simplifiable
        set(DOX_IS_CYGWIN TRUE)
    ELSE(DOX_CYGWIN_MATCH)
        set(DOX_IS_CYGWIN FALSE)
    ENDIF(DOX_CYGWIN_MATCH)
    IF (DOX_IS_CYGWIN AND NOT CYGWIN)
        # convert win32 paths to *nix
        do_fix_cygwin_paths(-u
            INPUT_DOXYGEN_DIRECTORY OUTPUT_DOXYGEN_DIRECTORY
            DOXYFILE)
    ELSEIF (NOT DOX_IS_CYGWIN AND CYGWIN)
        # convert *nix paths to win32
        do_fix_cygwin_paths(-m
            INPUT_DOXYGEN_DIRECTORY OUTPUT_DOXYGEN_DIRECTORY
            DOXYFILE)
    ENDIF(DOX_IS_CYGWIN AND NOT CYGWIN)
    # message("INTPUT_DOXYGEN_DIRECTORY: ${INPUT_DOXYGEN_DIRECTORY}")
    # message("OUTPUT_DOXYGEN_DIRECTORY: ${OUTPUT_DOXYGEN_DIRECTORY}")

    # Generate the actual Doxyfile from Doxyfile.in
    configure_file(${PROJECT_SOURCE_DIR}/src/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY IMMEDIATE)
    # And now the target for "make doc"
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
else(DOXYGEN_FOUND)
    message(FATAL_ERROR 
        "Doxygen is needed to build the documentation. Please install it correctly")
endif(DOXYGEN_FOUND)


#======================================================================
# How-to build with cmake
# open OSGeo windows, then:
#     cmake --build . --config Release --target clean
# Clean with:
#     cmake --build . --config Release --target clean
# prepare for CPack
#     cmake --build . --config Release --target install
#     cpack
# tests
#     cmake --build . --config Release
#     ctest .

