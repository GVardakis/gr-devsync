INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_DEVSYNC devsync)

FIND_PATH(
    DEVSYNC_INCLUDE_DIRS
    NAMES devsync/api.h
    HINTS $ENV{DEVSYNC_DIR}/include
        ${PC_DEVSYNC_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    DEVSYNC_LIBRARIES
    NAMES gnuradio-devsync
    HINTS $ENV{DEVSYNC_DIR}/lib
        ${PC_DEVSYNC_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DEVSYNC DEFAULT_MSG DEVSYNC_LIBRARIES DEVSYNC_INCLUDE_DIRS)
MARK_AS_ADVANCED(DEVSYNC_LIBRARIES DEVSYNC_INCLUDE_DIRS)

