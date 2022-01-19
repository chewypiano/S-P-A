file(GLOB srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pkb/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pql/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/tokenizer/*.cpp"
        )
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(unit_testing ${srcs}
        src/pkb/PKBTestCall.cpp
        src/pkb/PKBTestCtrlVar.cpp
        src/pkb/PKBTestFollow.cpp
        src/pkb/PKBTestGeneral.cpp
        src/pkb/PKBTestModify.cpp
        src/pkb/PKBTestParent.cpp
        src/pkb/PKBTestUse.cpp
        src/pkb/PKBTestPattern.cpp
        src/pkb/PKBTestNext.cpp
        src/pql/TestRow.cpp
        src/pql/TestQuery.cpp
        src/pql/TestIrrelevantClauseProcessor.cpp
        src/pql/TestQueryParser.cpp
        src/pql/TestClauseResult.cpp
        src/pql/TestQueryEvaluator.cpp
        src/tokenizer/TestTokenType.cpp
        src/tokenizer/TestToken.cpp
        src/tokenizer/TestRuleEngine.cpp
        src/tokenizer/TestReMatch.cpp
        src/TestSimpleParser.cpp
        src/Utils/TestSorting.cpp
        )

target_link_libraries(unit_testing spa)

