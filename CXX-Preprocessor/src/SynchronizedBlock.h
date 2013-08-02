#ifndef ORG_KNORBA_CXXPREPROCESSOR_SYNCHRONIZEDBLOCK_H
#define ORG_KNORBA_CXXPREPROCESSOR_SYNCHRONIZEDBLOCK_H

#include <string>
#include <sstream>

#include "common.h"
#include "SourceRangeEx.h"
#include "PragmaParticle.h"

using namespace std;

namespace org {
namespace knorba {
namespace cxxpreprocessor {

class PragmaParticle;

class SynchronizedBlock : public SourceRangeEx {
private:
  bool            _isOneLine;
  PragmaParticle* _enclosingParticle;

public:
  SynchronizedBlock(SourceManager* sm);

  void            setIsOneLine(bool isOneLine);
  bool            isOneLine();

  void            setEnclosingParticle(PragmaParticle* enParticle);
  PragmaParticle* getEnclosingParticle();

  virtual void    printToStringStream(stringstream& stream);
};

} // namespace cxxpreprocessor
} // namespace knorba
} // namespace org

#endif
