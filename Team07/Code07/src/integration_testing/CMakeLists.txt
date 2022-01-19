file(GLOB srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/PQL/PQLEvaluator/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/*.cpp"
        )
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/*/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*/*.hpp")
add_executable(integration_testing ${srcs} src/PQL/PQLEvaluator/TestAffectsT.cpp src/PQL/PQLEvaluator/TestNextBipT.cpp src/PQL/PQLEvaluator/TestAffectsBip.cpp)


target_link_libraries(integration_testing spa)
