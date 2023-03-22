cmake_minimum_required(VERSION 2.8.3)
project(rosneuro_acquisition_lsl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(ROSNEURO_ACQUISITION_MIN_VERSION "1.0.0")

find_package(catkin REQUIRED COMPONENTS 
			 roscpp 
			 std_msgs
			 pluginlib
			 rosneuro_data
			 rosneuro_msgs)
find_package(rosneuro_acquisition ${ROSNEURO_ACQUISITION_MIN_VERSION} REQUIRED)


find_package(PkgConfig)

SET(CMAKE_BUILD_TYPE RelWithDebInfo)
find_library(LSL_LIBRARIES lsl REQUIRED)
find_path(LSL_INCLUDE_DIRS lsl_cpp.h REQUIRED)

if(NOT LSL_LIBRARIES)
	message(WARNING "Library LSL not found in the system. 
	The plugin will not be compiled and its functionalities will not be
	available. To fix it, install LSL in your system and re-compile 
	the project")
else()
	message(STATUS "Library LSL found. The plugin will be installed")


catkin_package(
  INCLUDE_DIRS 
	include
  LIBRARIES 
	${PROJECT_NAME}
  CATKIN_DEPENDS
  	roscpp
	std_msgs
	pluginlib
	rosneuro_data
	rosneuro_msgs
	rosneuro_acquisition

  DEPENDS
)


###########
## Build ##
###########

include_directories(${catkin_INCLUDE_DIRS} 
					${rosneuro_acquisition_INCLUDE_DIRS}
					${LSL_INCLUDE_DIRS} 
					include
)

add_library(${PROJECT_NAME} 
			src/LSLDevice.cpp
)

target_link_libraries(${PROJECT_NAME} 
					  ${catkin_LIBRARIES} 
					  ${rosneuro_acquisition_LIBRARIES}
					  ${LSL_LIBRARIES}
)

add_dependencies(${PROJECT_NAME}
				 ${catkin_EXPORTED_TARGETS}
)

#################
## Executables ##
#################

add_executable(lslclient src/lslclient.cpp)
target_link_libraries(lslclient rosneuro_acquisition_lsl)

#################
##   Install   ##
#################
install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})

install(FILES plugin_lsl.xml
	DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)


endif()
