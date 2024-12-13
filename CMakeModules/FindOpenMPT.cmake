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

find_path(OPENMPT_INCLUDE_DIRS libopenmpt/libopenmpt.h
                               PATHS ${OpenMPT_INCLUDEDIR})
find_path(OPENMPT_INCLUDE_CONFIG libopenmpt_config.h
                                 PATH_SUFFIXES libopenmpt
                                 PATHS ${OpenMPT_INCLUDEDIR})
list(APPEND OPENMPT_INCLUDE_DIRS ${OPENMPT_INCLUDE_CONFIG})
find_library(OPENMPT_LIBRARIES NAMES openmpt
                               PATHS ${OpenMPT_LIBDIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Openmpt DEFAULT_MSG OPENMPT_INCLUDE_DIRS OPENMPT_LIBRARIES)

mark_as_advanced(OPENMPT_INCLUDE_DIRS OPENMPT_LIBRARIES)
