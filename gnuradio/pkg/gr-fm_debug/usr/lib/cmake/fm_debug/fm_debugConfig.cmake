INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_FM_DEBUG fm_debug)

FIND_PATH(
    FM_DEBUG_INCLUDE_DIRS
    NAMES fm_debug/api.h
    HINTS $ENV{FM_DEBUG_DIR}/include
        ${PC_FM_DEBUG_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    FM_DEBUG_LIBRARIES
    NAMES gnuradio-fm_debug
    HINTS $ENV{FM_DEBUG_DIR}/lib
        ${PC_FM_DEBUG_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FM_DEBUG DEFAULT_MSG FM_DEBUG_LIBRARIES FM_DEBUG_INCLUDE_DIRS)
MARK_AS_ADVANCED(FM_DEBUG_LIBRARIES FM_DEBUG_INCLUDE_DIRS)

