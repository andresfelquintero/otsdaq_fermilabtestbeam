include_directories($ENV{OTSDAQ_FERMILABTESTBEAM_DIR}/otsdaq-fermilabtestbeam/xxusb_3.3/include)

cet_make(LIBRARY_NAME xx_usb
		 LIBRARIES
		 usb
)

install_headers()
install_source()


