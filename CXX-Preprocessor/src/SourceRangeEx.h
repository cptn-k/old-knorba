#ifndef ORG_KNORBA_CXXPREPROCESSOR_SOURCERANGEEX_H
#define ORG_KNORBA_CXXPREPROCESSOR_SOURCERANGEEX_H

#include <string>
#include <sstream>
#include <iostream>

#include "clang/Basic/SourceManager.h"
#include "clang/Basic/SourceLocation.h" // Also defines class SourceRange

#include "common.h"
#include "org/knorba/common/StringStreamer.h"

using namespace std;
using namespace clang;
using namespace org::knorba::common;

namespace org {
namespace knorba {
namespace cxxpreprocessor {

class SourceRangeEx : public SourceRange, public StringStreamer {
private:
  SourceManager* sm;
  
public:
  SourceRangeEx(SourceManager* sm);
  ~SourceRangeEx();

  bool         contains(SourceLocation loc);
  bool         contains(const SourceRange& range);

  virtual void printToStringStream(stringstream& stream);
};

} // namespace cxxpreprocessor
} // namespace knorba
} // namespace org

#endif
