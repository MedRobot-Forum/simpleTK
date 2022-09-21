cmake_minimum_required( VERSION 3.18 )  

###########
# Project #
###########
SET(PROJECT_NAME simpleTK)
project(${PROJECT_NAME})  
set(CMAKE_INCLUDE_CURRENT_DIR ON)  


#########################
# External Dependencies #
#########################
#find_package( ITK REQUIRED )  
#include( ${ITK_USE_FILE}) 
find_package( VTK REQUIRED )  
#include( ${VTK_USE_FILE} )
#find_package(OpenCV REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Qt5 REQUIRED QUIET COMPONENTS Core Widgets OpenGL)
find_package(Qt5Widgets)  
include_directories(${PROJECT_SOURCE_DIR}  
                    ${CMAKE_CURRENT_BINARY_DIR}  
					${VTK_DIR}
					#${ITK_DIR}
                    #${OpenCV_LIBRARY_DIRS}
					) 
					
find_package(DICOM QUIET)
if(DICOM_FOUND)
include_directories( ${DICOM_USE_FILE} )
endif()
set(VTK_DICOM_LIBRARIES vtkDICOM)	
	
###########
# Targets #
###########
set(PROJECT_HDRS
simpleTK.h
VtkInclude.h
)

set(PROJECT_SRCS 
main.cpp
simpleTK.cpp
)

file(GLOB PROJECT_UIS "*.ui")
QT5_WRAP_UI(PROJECT_UIS_H ${PROJECT_UIS})

set(PROJECT_MOC_HDRS
simpleTK.h
)
QT5_WRAP_CPP(PROJECT_MOC_SRCS ${PROJECT_MOC_HDRS})

set(RESOURCE_FILE simpleTK.qrc)
QT5_ADD_RESOURCES(RESOURCE ${RESOURCE_FILE})

ADD_EXECUTABLE(${PROJECT_NAME}
	${PROJECT_HDRS}
	${PROJECT_SRCS}
	${PROJECT_UIS_H}
	${PROJECT_MOC_SRCS}
	${RESOURCE}
)

TARGET_LINK_LIBRARIES ( 
    ${PROJECT_NAME}
    ${VTK_LIBRARIES}
    ${VTK_DICOM_LIBRARIES}
    #${OpenCV_LIBS}
	Qt5::Widgets  
	Qt5::Core
	Qt5::Gui
  )
vtk_module_autoinit(TARGETS simpleTK MODULES ${VTK_LIBRARIES})