#include "indexing/BacktrackInstruction.hpp"
#include "indexing/MarkInstruction.hpp"
#include "indexing/RetryMeElseInstruction.hpp"

#include "procedural/AllocateInstruction.hpp"
#include "procedural/CallInstruction.hpp"
#include "procedural/ReturnInstruction.hpp"

#include "get/GetConstant.hpp"
#include "get/GetList.hpp"
#include "get/GetStructure.hpp"
#include "get/GetVariable.hpp"

#include "put/PutConstantInstruction.hpp"
#include "put/PutVariableInstruction.hpp"
#include "put/PutListInstruction.hpp"
#include "put/PutStructureInstruction.hpp"

#include "unify/UnifyConstantInstruction.hpp"
#include "unify/UnifyVariableInstruction.hpp"

#include "Cut.hpp"
#include "Instruction.hpp"