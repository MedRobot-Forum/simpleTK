cmake_minimum_required( VERSION 3.18 )  

###########
# Project #
###########
SET(PROJECT_NAME simpleTK)
project(${PROJECT_NAME})  
set(CMAKE_INCLUDE_CURRENT_DIR ON)  

# Link to external third-party libraries
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)


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
					
#find_package(DICOM QUIET)
#if(DICOM_FOUND)
#include_directories( ${DICOM_USE_FILE} )
#endif()
#set(VTK_DICOM_LIBRARIES vtkDICOM)	
	
	
#find_package(DCMTK REQUIRED ${DCMTK_FIND_PACKAGE_STATEGY})
#include_directories(${DCMTK_INCLUDE_DIRS})

###########
# Targets #
###########

#file(GLOB_RECURSE SOURCE "*.cpp" "*.cxx" "*.cc")
#file(GLOB_RECURSE RESOURCES "*.qrc")
#file(GLOB_RECURSE HEADERS "*.h")
#file(GLOB_RECURSE FORMS *.ui)

#set(SRCS ${SOURCE} ${HEADERS} ${RESOURCES} ${FORMS})

FILE(GLOB SRC_FILES "./*.cpp")
FILE(GLOB HEAD_FILES "./*.h")
FILE(GLOB UI_FILES "./*.ui")
file(GLOB_RECURSE RESOURCES "*.qrc")
qt5_wrap_ui(WRAP_FILES ${UI_FILES})
#set(RCC_FILES RcfServer.qrc)
source_group("Ui" FILES ${UI_FILES} ${WRAP_FILES} )

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEAD_FILES} ${WRAP_FILES} ${RESOURCES})




#set(PROJECT_HDRS
#simpleTK.h
#VtkInclude.h
#TagDialog.h
#)

#set(PROJECT_SRCS 
#main.cpp
#simpleTK.cpp
#TagDialog.cpp
#)

#file(GLOB PROJECT_UIS "*.ui")
#QT5_WRAP_UI(PROJECT_UIS_H ${PROJECT_UIS})

#set(PROJECT_MOC_HDRS
#simpleTK.h
#TagDialog.h
#)
#QT5_WRAP_CPP(PROJECT_MOC_SRCS ${PROJECT_MOC_HDRS})

#set(RESOURCE_FILE simpleTK.qrc)
#QT5_ADD_RESOURCES(RESOURCE ${RESOURCE_FILE})


TARGET_LINK_LIBRARIES ( 
    ${PROJECT_NAME}
    ${VTK_LIBRARIES}
    #${DCMTK_LIBRARIES}
	#${VTK_DICOM_LIBRARIES}
    #${OpenCV_LIBS}
	Qt5::Widgets  
	Qt5::Core
	Qt5::Gui
  )
vtk_module_autoinit(TARGETS simpleTK MODULES ${VTK_LIBRARIES})
