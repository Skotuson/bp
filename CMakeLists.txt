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
src/wam_code/instruction/indexing/MarkInstruction.cpp
src/wam_code/instruction/BranchInstruction.cpp
src/wam_code/instruction/indexing/RetryMeElseInstruction.cpp
src/wam_code/instruction/procedural/CallInstruction.cpp
src/wam_code/instruction/indexing/BacktrackInstruction.cpp
src/wam_code/instruction/procedural/AllocateInstruction.cpp
src/wam_code/instruction/procedural/ReturnInstruction.cpp
src/wam_code/instruction/CutInstruction.cpp
src/wam_code/instruction/get/GetConstantInstruction.cpp
src/wam_code/instruction/get/GetListInstruction.cpp
src/wam_code/instruction/get/GetStructureInstruction.cpp
src/wam_code/instruction/get/GetVariableInstruction.cpp
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