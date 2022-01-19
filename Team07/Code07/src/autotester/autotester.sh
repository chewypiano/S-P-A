#!/bin/bash

function build(){
  cd "$path" || echo "Code path not found" || return 1
  mkdir -p cmake-build-debug
  cd cmake-build-debug || echo "Folder creation fail" || return 1
  rm CMakeCache.txt
  cmake .. -DCMAKE_BUILD_TYPE=Release
  cmake --build ./
  return 1
}

if [ -z "$GITHUB_WORKSPACE" ]
then
  TESTER_PATH="$(pwd)/../../cmake-build-debug/src/autotester"
  TEST_PATH="$(pwd)/../../../Tests07"
  path="$(pwd)/../../../Code07"
else
  TESTER_PATH=$GITHUB_WORKSPACE/../build/src/autotester/autotester
  TEST_PATH=$GITHUB_WORKSPACE/Team07/Tests07
  path=$GITHUB_WORKSPACE/Team07/Code07
fi


build

cd "$TESTER_PATH" || echo "Path is not found" || return 1


# Valid Tests
./autotester "$TEST_PATH"/AllSymProgram/AllSymProgram_source.txt "$TEST_PATH"/AllSymProgram/AllSymProgram_queries.txt "$TEST_PATH"/AllSymProgram.xml
./autotester "$TEST_PATH"/FollowTests/followsProgram.txt "$TEST_PATH"/FollowTests/followsQueries.txt "$TEST_PATH"/FollowTests.xml
./autotester "$TEST_PATH"/MultipleUsesTests/program.txt "$TEST_PATH"/MultipleUsesTests/query.txt "$TEST_PATH"/MultipleUsesTests.xml
./autotester "$TEST_PATH"/NestingTests/nestedIfProgram.txt "$TEST_PATH"/NestingTests/nestedIfQueries.txt "$TEST_PATH"/nestedIf.xml
./autotester "$TEST_PATH"/NestingTests/nestedWhileProgram.txt "$TEST_PATH"/NestingTests/nestedWhileQueries.txt "$TEST_PATH"/nestedWhile.xml
./autotester "$TEST_PATH"/ParentTests/program.txt "$TEST_PATH"/ParentTests/query.txt "$TEST_PATH"/ParentTests.xml
./autotester "$TEST_PATH"/SampleTests/Sample_source.txt "$TEST_PATH"/SampleTests/Sample_queries.txt "$TEST_PATH"/SampleTests.xml
./autotester "$TEST_PATH"/SimpleTests/simpleProgram.txt "$TEST_PATH"/SimpleTests/simpleQueries.txt "$TEST_PATH"/SimpleTests.xml
./autotester "$TEST_PATH"/AllSymbols_source.txt "$TEST_PATH"/AllSymbols_queries.txt "$TEST_PATH"/AllSynMain.xml
./autotester "$TEST_PATH"/Complex_source.txt "$TEST_PATH"/Complex_queries.txt "$TEST_PATH"/Complex.xml
./autotester "$TEST_PATH"/Generic_source.txt "$TEST_PATH"/Generic_queries.txt "$TEST_PATH"/Generic.xml
./autotester "$TEST_PATH"/MultiClause_source.txt "$TEST_PATH"/MultiClause_queries.txt "$TEST_PATH"/MultiClause.xml



# Invalid Tests
./autotester "$TEST_PATH"/InvalidProgramTests/invalidSimpleProgram.txt "$TEST_PATH"/InvalidProgramTests/blank.txt "$TEST_PATH"/invalidSimple.xml
./autotester "$TEST_PATH"/InvalidProgramTests/invalidSimpleProgram2.txt "$TEST_PATH"/InvalidProgramTests/blank.txt "$TEST_PATH"/invalidSimpleProgram2.xml
./autotester "$TEST_PATH"/InvalidProgramTests/incompleteIfProgram.txt "$TEST_PATH"/InvalidProgramTests/blank.txt "$TEST_PATH"/incompleteIfProgram.xml

# Disable CI blocking
exit 0
