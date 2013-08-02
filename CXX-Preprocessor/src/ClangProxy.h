/*---[ClangProxy.h]-------------------------------------------m(._.)m--------*\
|
|  Proxy object for Clang parser.
|
|  Copyright (c) 2013, RIKEN (The Institute of Physical and Chemial Research)
|  All rights reserved.
|
|  Author: Hamed KHANDAN (hkhandan@ieee.org)
|
|  This file is distributed under the KnoRBA Free Public License. See
|  LICENSE.TXT for details.
|
*//////////////////////////////////////////////////////////////////////////////

#ifndef CLANG_PROXY_H
#define CLANG_PROXY_H

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include "llvm/Support/raw_ostream.h" /* Defines llvm::errs() */
#include "llvm/Support/Host.h"        /* Define getDefaultTargetTriple() */

#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/Diagnostic.h"   /* Contains class DiagnosticsEngine
                                              and class DiagnosticConsumer */
#include "clang/Basic/FileManager.h"  /* Also defines class FileEntry */
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/TargetInfo.h"

#include "clang/Lex/HeaderSearchOptions.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/Pragma.h"         /* Defines class PragmaHandler
                                             and class PragmaNamespace */

#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"

#include "clang/Sema/SemaConsumer.h"
#include "clang/Sema/Sema.h"

#include "clang/Parse/Parser.h"
#include "clang/Parse/ParseDiagnostic.h"
#include "clang/Parse/ParseAST.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/FrontendAction.h"

#include "clang/Rewrite/Core/Rewriter.h"

#include "FixedSettings.h"
#include "PragmaParticle.h"
#include "SynchronizedBlock.h"
#include "ParticleScope.h"
#include "SharedVariable.h"

using namespace std;
using namespace llvm;
using namespace clang;

namespace org {
namespace knorba {
namespace cxxpreprocessor {

typedef map<DeclContext*, ParticleScope*> ScopeMap;
typedef pair<DeclContext*, ParticleScope*> ScopeMapPair;

const StringRef KNORBA_PRAGMA_NAME          = "knorba";
const StringRef KNORBA_PRAGMA_PARTICLE_NAME = "particle";
const StringRef SYNCHRONIZED_MACRO_NAME     = "__synchronized";

class KnorbaASTVisitor : public RecursiveASTVisitor<KnorbaASTVisitor> {
private:
  ASTContext& _context;
  ScopeMap*   _scopeMap;

public:
  explicit KnorbaASTVisitor(ASTContext& context, ScopeMap* scopeMap) 
    : _context(context), _scopeMap(scopeMap)
  {
    // Nothing
  }

  bool VisitVarDecl(VarDecl* d) {
    if(!d->hasAttrs())
      return true;

    AttrVec& attrs = d->getAttrs();
    const int s = attrs.size();
    bool found = false;
    for(int i = 0; i < s; i++) {
      Attr* attr = attrs[i];
      if(attr->getKind() == attr::Annotate) {
        AnnotateAttr* annotateAttr = (AnnotateAttr*) attr;
        //TODO check annotation text
        found = true;
        break;
      }
    }

    if(!found) {
      return true;
    }

    FunctionDecl* container = (FunctionDecl*)d->getParentFunctionOrMethod();

    if(container == 0) {
      cout << "Something is wrong" << endl;
      return true;
    }

    ParticleScope* scope = 0;
    ScopeMap::iterator it = _scopeMap->find(container);

    if(it == _scopeMap->end()) {
      scope = new ParticleScope(& _context.getSourceManager());
      scope->setContext(container);
      _scopeMap->insert(ScopeMapPair(container, scope));
    } else {
      scope = it->second;
    }
    
    SharedVariable* svar = new SharedVariable(& _context.getSourceManager());
    svar->setDecleration(d);

    scope->addSharedVariable(svar);

    return true;
  }
};

class KnorbaAgentPragmaHandler : public PragmaHandler, public PPCallbacks {
private:
  SourceManager*              _sm;
  IdentifierInfo*             _syncMacroIdentifierInfo;
  bool                        _isInsideSyncBlock;
  vector<PragmaParticle*>*    _particles;
  vector<SynchronizedBlock*>* _syncBlocks;

  void lexSynchronizedBlock(Preprocessor& pp, Token& lastToken,
                            PragmaParticle* pragma);

public:
  explicit KnorbaAgentPragmaHandler(SourceManager* sm);
  virtual ~KnorbaAgentPragmaHandler();

  virtual void HandlePragma(Preprocessor&        pp,
                            PragmaIntroducerKind introducer,
                            Token&               firstToken);
  virtual void MacroDefined(const Token& tok, const MacroDirective* md);
  virtual void MacroExpands(const Token& tok, const MacroDirective* md, 
                            SourceRange range, const MacroArgs* args);

  vector<PragmaParticle*>* getParticles() {
    return _particles;
  }

  vector<SynchronizedBlock*>* getSyncBlocks() {
    return _syncBlocks;
  }
};

class ParserProxy {
private:
  CompilerInstance*         compilerInstance;
  DiagnosticOptions*        diagOptions;
  TextDiagnosticPrinter*    diagClient;
  DiagnosticsEngine*        diagsEngine;
  CompilerInvocation*       compilerInvocation;
  TargetOptions*            targetOptions;
  bool                      isCPP;
  KnorbaAgentPragmaHandler* agentPragmaHandler;
  ASTConsumer*              astConsumer;
  Sema*                     sema;
  Parser*                   parser;

public:
  ParserProxy(const string& fileName);
  ~ParserProxy();
  void rewrite(const string& fileName);
};

// FIXME move this from here to a class
static const string tokenValue(const Token& t, SourceManager* sm)
{
  const char* loc = sm->getCharacterData(t.getLocation());
  int len = t.getLength();
  string s(loc, len);
  return s;
}

} // namespace cxxpreprocessor
} // namespace knorba
} // namespace org

#endif
