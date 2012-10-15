# Checks for which hash map is supported with the current compiler.
# HAS_CXX11_UNORDERED_MAP 

# Note: as CheckTypeSize can't be used to test C++ file, this script relies on
# Rolf Eike Beer, and Andreas Weis script CheckCXX11Features.cmake
#
# Original script by Luc Hermitte
#

SET(CHECK_CXX11_OLD_CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "-std=c++0x")
endif()

CXX11_CHECK_FEATURE("unordered_map"      ""   HAS_CXX11_UNORDERED_MAP)
IF (NOT HAS_CXX11_UNORDERED_MAP)
    CXX11_CHECK_FEATURE("tr1-unordered_map"      ""   HAS_CXX_TR1_UNORDERED_MAP)
    IF (NOT HAS_CXX_TR1_UNORDERED_MAP)
        CXX11_CHECK_FEATURE("hash_map"               ""   HAS_HASH_MAP)
        IF (NOT HAS_HASH_MAP)
            CXX11_CHECK_FEATURE("ext_hash_map"           ""   HAS_EXT_HASH_MAP)
            IF (NOT HAS_EXT_HASH_MAP)
                CXX11_CHECK_FEATURE("stdext-hash_map"        ""   HAS_STDEXT_HASH_MAP)
            ENDIF(NOT HAS_EXT_HASH_MAP)
        ENDIF(NOT HAS_HASH_MAP)
    ENDIF (NOT HAS_CXX_TR1_UNORDERED_MAP)
ENDIF(NOT HAS_CXX11_UNORDERED_MAP)

SET(HAS_CXX11_UNORDERED_MAP   ${HAS_CXX11_UNORDERED_MAP}   CACHE INTERNAL "C++11 support for unordered_set")    
SET(HAS_CXX_TR1_UNORDERED_MAP ${HAS_CXX_TR1_UNORDERED_MAP} CACHE INTERNAL "C++11 support for unordered_set")
SET(HAS_EXT_HASH_MAP          ${HAS_EXT_HASH_MAP}          CACHE INTERNAL "Deprecated support for hash map by GCC")           
SET(HAS_HASH_MAP              ${HAS_HASH_MAP}              CACHE INTERNAL "Deprecated support for hash map by GCC")                   
SET(HAS_STDEXT_HASH_MAP       ${HAS_STDEXT_HASH_MAP}       CACHE INTERNAL "Deprecated support for hash map by GCC")     
# We suppose that sets and maps are supported all together
SET(HAS_CXX11_UNORDERED_SET   ${HAS_CXX11_UNORDERED_MAP}   CACHE INTERNAL "C++11 support for unordered_set")    
SET(HAS_CXX_TR1_UNORDERED_SET ${HAS_CXX_TR1_UNORDERED_MAP} CACHE INTERNAL "C++11 support for unordered_set")
SET(HAS_EXT_HASH_SET          ${HAS_EXT_HASH_MAP}          CACHE INTERNAL "Deprecated support for hash map by GCC")           
SET(HAS_HASH_SET              ${HAS_HASH_MAP}              CACHE INTERNAL "Deprecated support for hash map by GCC")                   
SET(HAS_STDEXT_HASH_SET       ${HAS_STDEXT_HASH_MAP}       CACHE INTERNAL "Deprecated support for hash map by GCC")     
                                                                                                            
SET(CXX11_FEATURE_LIST        ${CXX11_FEATURE_LIST}        CACHE STRING "C++11 feature support list")                     
MARK_AS_ADVANCED(FORCE CXX11_FEATURE_LIST)

SET(CMAKE_CXX_FLAGS ${CHECK_CXX11_OLD_CMAKE_CXX_FLAGS})
UNSET(CHECK_CXX11_OLD_CMAKE_CXX_FLAGS)


