find_package(messagefacility REQUIRED)

cet_make(LIBRARY_NAME DetectorConfiguration
         LIBRARIES
                 TableCore
                 ConfigurationInterface
                 Macros
                 #messagefacility
                 MF_MessageLogger
                 #               ${MF_UTILITIES}
                 # cetlib
                 # fhiclcpp
                 # cetlib_except
)

install_headers()
install_source()
