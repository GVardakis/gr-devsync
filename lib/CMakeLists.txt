# Copyright 2011,2012,2016 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
include_directories(${ARMADILLO_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
link_directories(${Armadillo_LIBRARY_DIRS})

list(APPEND devsync_sources
    alligner_impl.cc
    peak_detector_impl.cc
    rms_normalization_impl.cc
)

set(devsync_sources "${devsync_sources}" PARENT_SCOPE)
if(NOT devsync_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT devsync_sources)

add_library(gnuradio-devsync SHARED ${devsync_sources})
target_link_libraries(gnuradio-devsync ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-devsync PROPERTIES DEFINE_SYMBOL "gnuradio_devsync_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-devsync PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-devsync RUNTIME_COMPONENT "devsync_runtime" DEVEL_COMPONENT "devsync_devel")

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

