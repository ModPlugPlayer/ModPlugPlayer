# FindKissFFT
# ------------
# Finds the KissFFT as a Fast fourier Transofrmation (FFT) library
#
# This will define the following variables:
#
# KISSFFT_FOUND       - system has KissFFT
# KISSFFT_INCLUDE_DIR - the KissFFT include directory
#

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PC_KISSFFT kissfft QUIET)
endif()

find_path(KISSFFT_INCLUDE_DIR kissfft/kiss_fft.h
                          PATHS ${PC_KISSFFT_INCLUDEDIR})
find_library(KISSFFT_LIBRARIES NAMES kissfft-float kissfft-int32 kissfft-int16 kissfft-simd
                               PATHS ${PC_KISSFFT_LIBDIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(KissFFT REQUIRED_VARS KISSFFT_INCLUDE_DIR)

mark_as_advanced(KISSFFT_INCLUDE_DIR)