cmake_minimum_required(VERSION 3.22.1)
project(wam)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -pedantic ${CMAKE_CXX_FLAGS}")

add_library(wam_lib STATIC src/compiler/ast/ClauseNode.cpp
src/compiler/ast/ComplexNode.cpp
src/compiler/ast/ConstNode.cpp
src/compiler/ast/CallNode.cpp
src/compiler/ast/CutNode.cpp
src/compiler/ast/ListNode.cpp
src/compiler/ast/ProgramNode.cpp
src/compiler/ast/StructNode.cpp
src/compiler/ast/TermNode.cpp
src/compiler/ast/UnificationNode.cpp
src/compiler/ast/VarNode.cpp
src/compiler/CompilationContext.cpp
src/compiler/Compiler.cpp
src/compiler/lexer/Lexer.cpp
src/compiler/parser/Parser.cpp
src/desugar/Desugar.cpp
src/interpreter/data_structures/ArgumentRegisters.cpp
src/interpreter/data_structures/ChoicePoint.cpp
src/word/ConstantWord.cpp
src/word/ListWord.cpp
src/word/StructurePointerWord.cpp
src/word/StructureWord.cpp
src/word/VariableWord.cpp
src/word/Word.cpp
src/interpreter/Interpreter.cpp
src/interpreter/WAMState.cpp
src/preprocessor/Preprocessor.cpp
src/Renderer.cpp
src/wam_code/instruction/Instruction.cpp
src/wam_code/instruction/indexing/Mark.cpp
src/wam_code/instruction/BranchInstruction.cpp
src/wam_code/instruction/indexing/RetryMeElse.cpp
src/wam_code/instruction/procedural/Call.cpp
src/wam_code/instruction/indexing/Backtrack.cpp
src/wam_code/instruction/procedural/Allocate.cpp
src/wam_code/instruction/procedural/Return.cpp
src/wam_code/instruction/Cut.cpp
src/wam_code/instruction/get/GetConstant.cpp
src/wam_code/instruction/get/GetList.cpp
src/wam_code/instruction/get/GetStructure.cpp
src/wam_code/instruction/get/GetVariable.cpp
src/wam_code/instruction/put/PutConstant.cpp
src/wam_code/instruction/put/PutVariable.cpp
src/wam_code/instruction/put/PutList.cpp
src/wam_code/instruction/put/PutStructure.cpp
src/wam_code/instruction/unify/UnifyConstant.cpp
src/wam_code/instruction/unify/UnifyVariable.cpp
src/wam_code/WAMCode.cpp)

add_executable(wam src/main.cpp)
target_link_libraries(wam wam_lib)

add_executable(wam_test 
test/main.cpp
test/compiler/lexer/LexerTest.cpp
test/compiler/parser/ParserTest.cpp
test/desugar/DesugarTest.cpp
test/interpreter/InterpreterTest.cpp
test/preprocessor/PreprocessorTest.cpp)

target_link_libraries(wam_test wam_lib)