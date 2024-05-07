#include "indexing/Backtrack.hpp"
#include "indexing/Mark.hpp"
#include "indexing/RetryMeElse.hpp"

#include "procedural/Allocate.hpp"
#include "procedural/Call.hpp"
#include "procedural/Return.hpp"

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