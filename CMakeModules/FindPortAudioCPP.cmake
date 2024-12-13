#  Find PortaudioCPP
#
#  It sets the following variables:
#
#  PORTAUDIOCPP_FOUND - system has Portaudio
#  PORTAUDIOCPP_INCLUDE_DIRS - the Portaudio include directory
#  PORTAUDIOCPP_LIBRARIES - Link these to use Portaudio
#

if(PORTAUDIOCPP_LIBRARIES AND PORTAUDIOCPP_INCLUDE_DIRS)
    # already in cache
    set(PORTAUDIOCPP_FOUND TRUE)
else(PORTAUDIOCPP_LIBRARIES AND PORTAUDIOCPP_INCLUDE_DIRS)
    find_package(PkgConfig)

    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PortAudioCPP portaudiocpp QUIET)
    endif()

    set(PORTAUDIOCPP_INCLUDE_DIRS {PortAudioCPP_INCLUDEDIR})
    find_library(PORTAUDIOCPP_LIBRARIES NAMES portaudiocpp
                                 PATHS ${PortAudioCPP_LIBDIR})

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(PortAudioCPP DEFAULT_MSG PORTAUDIOCPP_INCLUDE_DIRS PORTAUDIOCPP_LIBRARIES)

    mark_as_advanced(PORTAUDIOCPP_INCLUDE_DIRS PORTAUDIOCPP_LIBRARIES)
endif(PORTAUDIOCPP_LIBRARIES AND PORTAUDIOCPP_INCLUDE_DIRS)
