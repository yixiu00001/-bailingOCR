cmake_minimum_required(VERSION 2.8) 
PROJECT (bailing)
find_package(OpenCV REQUIRED )

#head file path
INCLUDE_DIRECTORIES(
	include
)
#source directory
AUX_SOURCE_DIRECTORY(./src DIR_SRCS)
#set environment variable
SET(TEST_MATH
	${DIR_SRCS}
)
SET(CMAKE_BUILD_TYPE "Debug")

SET(CMAKE_C_FLAGS  "-O2 -pipe -fPIC -Wall -fmessage-length=0")  
SET(CMAKE_CXX_FLAGS "-O2 -pipe -fPIC -Wall -fmessage-length=0")  
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb ")

set(TestJNI_LIB BailingOCRJni)
add_library(${TestJNI_LIB} SHARED ${TEST_MATH})
target_link_libraries( ${TestJNI_LIB} lept tesseract ${OpenCV_LIBS} )

set(TestJNI_LIBs BailingOCR_static)
add_library(${TestJNI_LIBs} STATIC ${TEST_MATH})
target_link_libraries( ${TestJNI_LIBs} lept tesseract ${OpenCV_LIBS} )
SET_TARGET_PROPERTIES(${TestJNI_LIBs}  PROPERTIES OUTPUT_NAME "BailingOCR")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#add executable file
ADD_EXECUTABLE(bailing ${TEST_MATH})
#add link library
target_link_libraries( bailing lept tesseract ${OpenCV_LIBS} )

#install(TARGETS ${TestJNI_LIB} ${TestJNI_LIB}   
#		ARCHIVE DESTINATION  /usr/lib   
#		LIBRARY DESTINATION  /usr/lib )  


