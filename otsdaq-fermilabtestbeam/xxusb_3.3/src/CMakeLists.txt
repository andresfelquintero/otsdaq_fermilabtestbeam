# Use pkg-config to find libusb
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb)

include_directories(
  ${LIBUSB_INCLUDE_DIRS}
  $ENV{OTSDAQ_FERMILABTESTBEAM_DIR}/otsdaq-fermilabtestbeam/xxusb_3.3/include
)

link_directories(${LIBUSB_LIBRARY_DIRS})

cet_make(LIBRARY_NAME xx_usb
         LIBRARIES
         ${LIBUSB_LIBRARIES}
)

install_headers()
install_source()


#include_directories($ENV{OTSDAQ_FERMILABTESTBEAM_DIR}/otsdaq-fermilabtestbeam/xxusb_3.3/include)

#cet_make(LIBRARY_NAME xx_usb
#		 LIBRARIES
#		 usb
#)


#install_headers()
#install_source()


