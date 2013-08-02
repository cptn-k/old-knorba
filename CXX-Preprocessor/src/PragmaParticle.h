#ifndef ORG_KNORBA_CXXPREPROCESSOR_PRAGMA_PARTICLE_H
#define ORG_KNORBA_CXXPREPROCESSOR_PRAGMA_PARTICLE_H

#include <vector>
#include <string>
#include <sstream>

#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"

#include "SourceRangeEx.h"
#include "SynchronizedBlock.h"

using namespace std;
using namespace clang;

namespace org {
namespace knorba {
namespace cxxpreprocessor {

namespace particle {
  typedef enum e_particle_type {
    GRID
  } type;
}

class SynchronizedBlock;

class PragmaParticle : public SourceRangeEx {
private:
  particle::type type;
  vector<string>* dimensions;
  vector<SynchronizedBlock*>* _syncBlocks;
  static int _counter;
  int _id;
  string _threadName;

public:
  PragmaParticle(SourceManager* sm);
  ~PragmaParticle();

  void setType(particle::type type);
  particle::type getType();
  
  void   addDimension(string label);
  int    getNumberOfDimensions();
  string getDimensionName(int index);

  void   addSynchronizedBlock(SynchronizedBlock* syncBlock);
  int    getNumberOfSynchronizedBlocks();
  SynchronizedBlock* getSynchronizedBlock(int index);

  string getThreadName();

  virtual void printToStringStream(stringstream& stream);
};

} // namespace cxxpreprocessor
} // namespace knorba
} // namepsace org

#endif
