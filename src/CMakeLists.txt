find_package(LLVM REQUIRED CONFIG)

if (LLVM_FOUND)
    message(STATUS "LLVM_VERSION_MAJOR: ${LLVM_VERSION_MAJOR}")
    message(STATUS "LLVM_VERSION_MINOR: ${LLVM_VERSION_MINOR}")
    message(STATUS "LLVM_VERSION_PATCH: ${LLVM_VERSION_PATCH}")
    # GREATER_EQUAL is not supported in older cmake versions...
    # Say ubuntu 16.04
    if (NOT (${LLVM_VERSION_MAJOR} GREATER 7))
	message(STATUS ${CMAKE_VERSION})
        message(FATAL_ERROR "LLVM version must be at least 8!")
    endif()
else()
    message(FATAL_ERROR "You haven't install LLVM !")
endif()

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(LLVM_LIBS Support Core Analysis)

add_library(LoopMetaPass MODULE LoopMetaPass.cc)
target_link_libraries(LoopMetaPass ${LLVM_LIBS})