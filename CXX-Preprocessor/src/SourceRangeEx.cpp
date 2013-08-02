#include "SourceRangeEx.h"

namespace org {
namespace knorba {
namespace cxxpreprocessor {

SourceRangeEx::SourceRangeEx(SourceManager* sm)
: sm(sm) 
{ 
  retain(sm); 
}

SourceRangeEx::~SourceRangeEx() {
  release(sm);
}

bool SourceRangeEx::contains(SourceLocation loc) {
  return sm->isBeforeInTranslationUnit(getBegin(), loc)
         && sm->isBeforeInTranslationUnit(loc, getEnd());
}

bool SourceRangeEx::contains(const SourceRange& range) {
  return contains(range.getBegin()) && contains(range.getEnd());
}

void SourceRangeEx::printToStringStream(stringstream& stream) {
  stream << getBegin().printToString(*sm) << ", "
    << getEnd().printToString(*sm);
}

} // namespace cxxpreprocessor
} // namespace knorba
} // namespace org
