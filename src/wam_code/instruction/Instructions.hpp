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

#include "put/PutConstant.hpp"
#include "put/PutVariable.hpp"
#include "put/PutList.hpp"
#include "put/PutStructure.hpp"

#include "unify/UnifyConstant.hpp"
#include "unify/UnifyVariable.hpp"

#include "Cut.hpp"
#include "Instruction.hpp"