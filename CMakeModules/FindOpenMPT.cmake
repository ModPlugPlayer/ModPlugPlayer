# Find OpenMPT Library
#
# Usage:
#   find_package(FFTW [REQUIRED] [QUIET] )
#
# It sets the following variables:
# OPENMPT_FOUND - system has openmpt
# OPENMPT_INCLUDE_DIRS - the openmpt include directory
# OPENMPT_LIBRARIES - The openmpt libraries

find_package(PkgConfig REQUIRED)

if(NOT PKG_CONFIG_FOUND)
  message(FATAL_ERROR "FindOpenMPT CMake module requires pkg-config to be run." )
endif()

if(PKG_CONFIG_FOUND)
  pkg_check_modules(OpenMPT libopenmpt QUIET)
endif()

#Check whether to search static or dynamic libs
set( CMAKE_FIND_LIBRARY_SUFFIXES_SAV ${CMAKE_FIND_LIBRARY_SUFFIXES} )

if( ${OpenMPT_USE_STATIC_LIBS} )
  set( CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX} )
else()
  set( CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_SHARED_LIBRARY_SUFFIX} )
endif()

find_path(OPENMPT_INCLUDE_DIRS libopenmpt/libopenmpt.h
                               PATHS ${OPENMPT_INCLUDEDIR})
find_path(OPENMPT_INCLUDE_CONFIG libopenmpt_config.h
                                 PATH_SUFFIXES libopenmpt
                                 PATHS ${OPENMPT_INCLUDEDIR})
list(APPEND OPENMPT_INCLUDE_DIRS ${OPENMPT_INCLUDE_CONFIG})
find_library(OPENMPT_LIBRARIES NAMES openmpt
                               PATHS ${OPENMPT_LIBDIR})


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenMPT DEFAULT_MSG OPENMPT_INCLUDE_DIRS OPENMPT_LIBRARIES)

# Return back to the original value before exit
set( CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES_SAV} )

mark_as_advanced(OPENMPT_INCLUDE_DIRS OPENMPT_LIBRARIES)
