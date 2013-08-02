#include "SynchronizedBlock.h"

namespace org {
namespace knorba {
namespace cxxpreprocessor {

SynchronizedBlock::SynchronizedBlock(SourceManager* sm) 
: SourceRangeEx(sm)
{
  _enclosingParticle = 0;
}

void
SynchronizedBlock::setIsOneLine(bool isOneLine)
{
  _isOneLine = isOneLine;
}

bool
SynchronizedBlock::isOneLine() {
  return _isOneLine;
}

void 
SynchronizedBlock::setEnclosingParticle(PragmaParticle* enParticle) {
  release(_enclosingParticle);
  _enclosingParticle = enParticle;
  retain(_enclosingParticle);
}

PragmaParticle* 
SynchronizedBlock::getEnclosingParticle() {
  return _enclosingParticle;
}

void
SynchronizedBlock::printToStringStream(stringstream& stream) {
  stream << "SynchronizedBlock isOneLine: ";
  if(_isOneLine)
    stream << "yes";
  else
    stream << "no";
  stream << ", range: ";
  SourceRangeEx::printToStringStream(stream);
}

} // namespace cxxpreprocessor
} // namespace knorba
} // namespace org
