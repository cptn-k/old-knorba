#ifndef ORG_KNORBA_CXXPREPROCESSOR_PARTICLESCOPE_H
#define ORG_KNORBA_CXXPREPROCESSOR_PARTICLESCOPE_H
 
#include <vector>

#include "clang/Basic/SourceManager.h"
#include "clang/AST/DeclBase.h"
#include "llvm/ADT/StringRef.h"

#include "SourceRangeEx.h"
#include "SharedVariable.h"
#include "PragmaParticle.h"

using namespace std;
using namespace clang;
using namespace llvm;

namespace org {
namespace knorba {
namespace cxxpreprocessor {

class ParticleScope : public SourceRangeEx {
private:
  string                   _name;
  DeclContext*             _context;
  vector<SharedVariable*>* _sharedVariables;
  vector<PragmaParticle*>* _particles;

public:
  ParticleScope(SourceManager* sm) : SourceRangeEx(sm) {
    _sharedVariables = new vector<SharedVariable*>();
    _particles = new vector<PragmaParticle*>();
  }

  virtual ~ParticleScope() {
    for(int i = _sharedVariables->size() - 1; i >= 0; i--) {
      SharedVariable* var = _sharedVariables->at(i);
      delete var;
    }

    for(int i = _particles->size() - 1; i >= 0; i--) {
      PragmaParticle* p = _particles->at(i);
      delete p;
    }

    delete _sharedVariables;
    delete _particles;
  }

  inline void setName(string name) {
    _name = name;
  }

  inline string getName() {
    return _name;
  }

  inline void setContext(FunctionDecl* context) {
    _context = context;
    setBegin(context->getSourceRange().getBegin());
    setEnd(context->getSourceRange().getEnd());
    if(context->isFunctionOrMethod()) {
      _name = ((FunctionDecl*)context)->getNameInfo().getAsString();
    }
  }

  inline void addSharedVariable(SharedVariable* var) {
    _sharedVariables->insert(_sharedVariables->end(), var);
  }

  inline int getNumberOfSharedVariables() {
    return _sharedVariables->size();
  }

  inline SharedVariable* getSharedVariable(int index) {
    return (*_sharedVariables)[index];
  }

  inline void addParticle(PragmaParticle* particle) {
    _particles->insert(_particles->end(), particle);
  }

  inline int getNumberOfParticles() {
    return _particles->size();
  }

  inline PragmaParticle* getParticle(int index) {
    return (*_particles)[index];
  }

  virtual void printToStringStream(stringstream& stream) {
    stream << "ParticleScope name: " << getName();
    stream << " range: [";
    SourceRangeEx::printToStringStream(stream);
    stream << "] sharedVariables: [";
    
    int s = _sharedVariables->size();
    for(int i = 0; i < s; i++) {
      (*_sharedVariables)[i]->printToStringStream(stream);
      if(i < s - 1)
        stream << ", ";
    }
    
    stream << "] particles: [";

    s = _particles->size();
    for(int i = 0; i < s; i++) {
      (*_particles)[i]->printToStringStream(stream);
      if(i < s - 1)
        stream << ", ";
    }

    stream << "]";
  }

};

} // cxxpreprocessor
} // knorba
} // org

#endif
