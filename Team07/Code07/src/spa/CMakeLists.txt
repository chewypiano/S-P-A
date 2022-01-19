file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/Tokenizer/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/SourceProcessor/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/SourceProcessor/SourceProcessorUtil/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/CFG/*.hpp")



add_library(spa ${srcs} ${headers} src/PKB/CFGComponent.cpp src/PKB/CFGComponent.h src/PKB/CFGBipComponent.cpp src/PKB/CFGBipComponent.h src/PQL/QueryEvaluator/ValueStore.cpp src/PQL/QueryEvaluator/ValueStore.h src/PQL/QueryEvaluator/SuchEvaluator.cpp src/PQL/QueryEvaluator/SuchEvaluator.h)



# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Tokenizer)
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/SourceProcessor)
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/SourceProcessor/SourceProcessorUtil)
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/PQL)
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/PQL/QueryParser)
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/PQL/QueryProjector)
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/PQL/QueryEvaluator)
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/)
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/CFG/)


