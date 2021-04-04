project(${ULTRASCHALL_FRAMEWORK_NAME} LANGUAGES C CXX VERSION 6.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PUBLIC_FRAMEWORK_INCLUDES
  ${PUBLIC_INCLUDE_PATH}/common.h
  ${PUBLIC_INCLUDE_PATH}/framework.h
  ${PUBLIC_INCLUDE_PATH}/frameworkfilemanager.h
  ${PUBLIC_INCLUDE_PATH}/frameworkglobals.h
  ${PUBLIC_INCLUDE_PATH}/frameworkhttpclient.h
  ${PUBLIC_INCLUDE_PATH}/frameworkhttprequest.h
  ${PUBLIC_INCLUDE_PATH}/frameworkhttpresponse.h
  ${PUBLIC_INCLUDE_PATH}/frameworkid3v2writer.h
  ${PUBLIC_INCLUDE_PATH}/frameworkpicture.h
  ${PUBLIC_INCLUDE_PATH}/frameworktagwriterfactory.h
  ${PUBLIC_INCLUDE_PATH}/frameworkxmlattribute.h
  ${PUBLIC_INCLUDE_PATH}/frameworkxmldocument.h
  ${PUBLIC_INCLUDE_PATH}/frameworkxmlelement.h
)

set(PRIVATE_FRAMEWORK_INCLUDES
  frameworkclass.h
  frameworkcommon.h
  frameworkid3v2.h
  frameworkid3v2context.h
  frameworkitagwriter.h
)

set(PRIVATE_FRAMEWORK_SOURCES
  frameworkfilemanager.cpp
  frameworkhttpclient.cpp
  frameworkhttprequest.cpp
  frameworkhttpresponse.cpp
  frameworkid3v2.cpp
  frameworkid3v2context.cpp
  frameworkid3v2writer.cpp
  frameworkpicture.cpp
  frameworktagwriterfactory.cpp
  frameworkxmlattribute.cpp
  frameworkxmlelement.cpp
  frameworkxmldocument.cpp
)

add_library(${ULTRASCHALL_FRAMEWORK_NAME} SHARED
  ${PUBLIC_FRAMEWORK_INCLUDES}
  ${PRIVATE_FRAMEWORK_INCLUDES}
  ${PRIVATE_FRAMEWORK_SOURCES}
)

add_dependencies(${ULTRASCHALL_FRAMEWORK_NAME} ${ULTRASCHALL_MODEL_NAME})
target_link_libraries(${ULTRASCHALL_FRAMEWORK_NAME} ${ULTRASCHALL_MODEL_NAME})

set_target_properties(${ULTRASCHALL_FRAMEWORK_NAME} PROPERTIES LINKER_LANGUAGE CXX)
