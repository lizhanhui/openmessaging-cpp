add_library(oms_c SHARED Connect.cpp)

set_target_properties(oms_c PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
                      OUTPUT_NAME oms_jni_ext)