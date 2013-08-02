#ifndef ORG_KNORBA_CXXPREPROCESSOR_SHAREDVARIABLE_H
#define ORG_KNORBA_CXXPREPROCESSOR_SHAREDVARIABLE_H

#include "llvm/ADT/StringRef.h"
#include "clang/AST/Type.h"
#include "clang/AST/Expr.h"

#include "org/knorba/common/StringStreamer.h"
#include "SourceRangeEx.h"

using namespace llvm;
using namespace clang;
using namespace org::knorba::common;

namespace org {
namespace knorba {
namespace cxxpreprocessor {

class SharedVariable : public StringStreamer, public SourceRangeEx {
private:
  VarDecl*       _decleration;
  SourceManager* _sm;
  string         _name;

public:
  SharedVariable(SourceManager* sm) : _sm(sm), SourceRangeEx(sm) {

  }

  inline void setDecleration(VarDecl* decl) {
    _decleration = decl;
    setBegin(_sm->getExpansionLoc(decl->getSourceRange().getBegin()));
    setEnd(_sm->getExpansionLoc(decl->getSourceRange().getEnd()));
    _name = decl->getName().str();
  }

  inline string getName() {
    return _name;
  }

  inline const QualType getType() {
    return _decleration->getType();
  }

  inline Expr* getInitializer() {
    if(!_decleration->hasInit())
      return 0;
    return _decleration->getInit();
  }

  virtual void printToStringStream(stringstream& stream) {
    stream << "SharedVariable name: " << getName();
    stream << " location: " << getBegin().printToString(*_sm);
  }
};

} // org
} // konrba
} // cxxpreprocessor

#endif
