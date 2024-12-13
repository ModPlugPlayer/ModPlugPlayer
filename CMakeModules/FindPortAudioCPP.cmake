#  Find PortaudioCPP
#
#  It sets the following variables:
#
#  PORTAUDIOCPP_FOUND - system has Portaudio
#  PORTAUDIOCPP_INCLUDE_DIRS - the Portaudio include directory
#  PORTAUDIOCPP_LIBRARIES - Link these to use Portaudio
#

#Check whether to search static or dynamic libs
set( CMAKE_FIND_LIBRARY_SUFFIXES_SAV ${CMAKE_FIND_LIBRARY_SUFFIXES} )

if( ${PortAudioCPP_USE_STATIC_LIBS} )
  set( CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX} )
else()
  set( CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_SHARED_LIBRARY_SUFFIX} )
endif()

find_package(PkgConfig)

if(PKG_CONFIG_FOUND)
    pkg_check_modules(PortAudioCPP portaudiocpp QUIET)
endif()

set(PORTAUDIOCPP_INCLUDE_DIRS {PortAudioCPP_INCLUDEDIR})
find_library(PORTAUDIOCPP_LIBRARIES NAMES portaudiocpp
                             PATHS ${PortAudioCPP_LIBDIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PortAudioCPP DEFAULT_MSG PORTAUDIOCPP_INCLUDE_DIRS PORTAUDIOCPP_LIBRARIES)

# Return back to the original value before exit
set( CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES_SAV} )

mark_as_advanced(PORTAUDIOCPP_INCLUDE_DIRS PORTAUDIOCPP_LIBRARIES)
