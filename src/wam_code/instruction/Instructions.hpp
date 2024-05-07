#include "indexing/BacktrackInstruction.hpp"
#include "indexing/MarkInstruction.hpp"
#include "indexing/RetryMeElseInstruction.hpp"

#include "procedural/AllocateInstruction.hpp"
#include "procedural/CallInstruction.hpp"
#include "procedural/ReturnInstruction.hpp"

#include "get/GetConstantInstruction.hpp"
#include "get/GetListInstruction.hpp"
#include "get/GetStructureInstruction.hpp"
#include "get/GetVariableInstruction.hpp"

#include "put/PutConstantInstruction.hpp"
#include "put/PutVariableInstruction.hpp"
#include "put/PutListInstruction.hpp"
#include "put/PutStructureInstruction.hpp"

#include "CutInstruction.hpp"
#include "Instruction.hpp"