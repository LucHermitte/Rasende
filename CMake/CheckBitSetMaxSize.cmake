# Non bugged bitset implementations give sizeof bitset<0xffffffff> == 512Mb.
# See GCC Bug libstdc++/45713:
# http://gcc.gnu.org/bugzilla/show_bug.cgi?id=45713
# test script by Luc Hermitte
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.6)

IF (NOT DEFINED HAS_BITSET_0xffffffff)
    SET(_bindir "${CMAKE_CURRENT_BINARY_DIR}/cxx/cxx_test-bitset-size")
    SET(_SRCFILE_BASE ${CMAKE_CURRENT_LIST_DIR}/c++-test-bitset-size)
    SET(_SRCFILE "${_SRCFILE_BASE}.cpp")
    SET(_LOG_NAME "std::bitset<0xffffffff>")
    MESSAGE(STATUS "Checking support for $std::bitset<0xffffffff>")

    IF (CROSS_COMPILING)
        try_compile(HAS_BITSET_0xffffffff "${_bindir}" "${_SRCFILE}")
    ELSE (CROSS_COMPILING)
        try_run(_RUN_RESULT_VAR _COMPILE_RESULT_VAR
            "${_bindir}" "${_SRCFILE}")
        IF (_COMPILE_RESULT_VAR AND NOT _RUN_RESULT_VAR)
            SET(HAS_BITSET_0xffffffff TRUE)
        ELSE (_COMPILE_RESULT_VAR AND NOT _RUN_RESULT_VAR)
            SET(HAS_BITSET_0xffffffff FALSE)
        ENDIF (_COMPILE_RESULT_VAR AND NOT _RUN_RESULT_VAR)
    ENDIF (CROSS_COMPILING)
    SET(HAS_BITSET_0xffffffff ${HAS_BITSET_0xffffffff} CACHE INTERNAL "Support for std::bitset<0xffffffff>")
    IF (NOT HAS_BITSET_0xffffffff)
        MESSAGE(STATUS "Checking support for std::bitset<0xffffffff> -- OK")
    ENDIF(NOT HAS_BITSET_0xffffffff)
ENDIF(NOT DEFINED HAS_BITSET_0xffffffff)

IF (NOT HAS_BITSET_0xffffffff)
    MESSAGE(FATAL_ERROR "Your implementation of std::bitset is bugged. See GCC Bug libstdc++/45713: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=45713")
ENDIF(NOT HAS_BITSET_0xffffffff)
