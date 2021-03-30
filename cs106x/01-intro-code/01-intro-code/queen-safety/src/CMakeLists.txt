include_directories(${CMAKE_SOURCE_DIR}/lib)
aux_source_directory(. SRC)
add_executable(main ${SRC})
#target_link_libraries(main spl)
