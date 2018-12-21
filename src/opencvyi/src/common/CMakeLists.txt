project(common)

message(STATUS "Configuring Sub_project: common")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    /usr/local/include
    ${CURRENT_PROJET_DIR}/include
)

#driver
add_library(common 
    common_serial.cpp
    LinuxSerial.cpp
   
)

target_link_libraries(common /home/cuicheng/work/src/opencvyi/include
    ${OpenCV_LIBRARIES}
)
target_include_directories(common
  PUBLIC
  ${OpenCV_INCLUDE_DIRS}
)
