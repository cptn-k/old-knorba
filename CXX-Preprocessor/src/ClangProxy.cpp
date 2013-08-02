/*---[ClangProxy.cpp]-----------------------------------------m(._.)m--------*\
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

#include "ClangProxy.h"

namespace org {
namespace knorba {
namespace cxxpreprocessor {

KnorbaAgentPragmaHandler::KnorbaAgentPragmaHandler(SourceManager* sm)
  : PragmaHandler(KNORBA_PRAGMA_PARTICLE_NAME), 
    _sm(sm), 
    _syncMacroIdentifierInfo(0), 
    _isInsideSyncBlock(false)
{
  _particles = new vector<PragmaParticle*>();
  _syncBlocks = new vector<SynchronizedBlock*>();
}

KnorbaAgentPragmaHandler::~KnorbaAgentPragmaHandler() {
  delete _particles;
  delete _syncBlocks;
}


void 
KnorbaAgentPragmaHandler::MacroDefined(const Token& tok,
  const MacroDirective* md) 
{
  if(_syncMacroIdentifierInfo != 0)
    return;

  if(tok.getIdentifierInfo()->getName().equals(SYNCHRONIZED_MACRO_NAME)) {
    _syncMacroIdentifierInfo = tok.getIdentifierInfo();
  } else {
    return;
  }
}

void
KnorbaAgentPragmaHandler::MacroExpands(
  const Token&          tok, 
  const MacroDirective* md, 
        SourceRange     range, 
  const MacroArgs*      args)
{
  if(_syncMacroIdentifierInfo == 0)
    return;

  if(tok.getIdentifierInfo() == _syncMacroIdentifierInfo) {
    if(_isInsideSyncBlock) {
      // TODO Generate diagnostics
      cout << "Ignoring nested synchronized block" << endl;
      return;
    }
    _isInsideSyncBlock = true;
  } 
}

void 
KnorbaAgentPragmaHandler::HandlePragma(
        Preprocessor& pp, PragmaIntroducerKind introducer, Token& firstToken)
{
  if(_syncMacroIdentifierInfo == 0) {
    // TODO Generate diagnostics
    errs() << "__synchronized macro is not defined, forgot to include "
              "<knorba.h>?\n";
  }

  Token& token = firstToken;

  if(token.getKind() != tok::identifier) {
    pp.Diag(token, diag::err_expected_ident);
    return;
  }

  if(strcmp("particle", token.getIdentifierInfo()->getName().data()) != 0) {
    // TODO generate diagnostics
    llvm::errs() << "\"particle\" keyword expected\n";
    return;
  }

  pp.Lex(token);

  if(token.getKind() != tok::identifier) {
    pp.Diag(token, diag::err_expected_ident);
    return;
  }

  PragmaParticle* pragma = new PragmaParticle(_sm);

  StringRef typeStr = token.getIdentifierInfo()->getName();
  particle::type ptype;
  if(typeStr == "grid")
    pragma->setType(particle::GRID);
  else {
    // TODO generate diagnostics
    llvm::errs() << "Unknown particle type \"" << typeStr << "\"\n";
    return;
  }

  pp.Lex(token);
  if(token.isNot(tok::l_paren)) {
    pp.Diag(token, diag::err_expected_lparen);
    return;
  }

  while(token.isNot(tok::r_paren)) {
    pp.Lex(token);
    if(token.isNot(tok::identifier)) {
      pp.Diag(token, diag::err_expected_ident);
      return;
    }
    pragma->addDimension(token.getIdentifierInfo()->getName().str());
    pp.Lex(token);
    if(token.isNot(tok::comma))
      break;
  }

  if(token.isNot(tok::r_paren)) {
    pp.Diag(token.getLocation(), diag::err_expected_rparen);
    return;
  }

  pp.Lex(token);
  if(token.isNot(tok::eod)) {
    // TODO generate diagnostics
    errs() << "This is unexpected here: " << token.getName() << "\n";
    return;
  }

  pp.EnableBacktrackAtThisPos();
  pp.Lex(token);
  int braceCount = 0;
  if(token.is(tok::l_brace)) {
    braceCount = 1;
    pragma->setBegin(token.getLocation());
  }

  while(token.isNot(tok::eof) && braceCount > 0) {
    pp.Lex(token);
    if(_isInsideSyncBlock) {
      lexSynchronizedBlock(pp, token, pragma);
      _isInsideSyncBlock = false;
    }
    if(token.is(tok::l_brace))
      braceCount++;
    if(token.is(tok::r_brace))
      braceCount--;
  }

  if(token.is(tok::r_brace)) {
    pragma->setEnd(token.getLocation());
  }

  _particles->insert(_particles->end(), pragma);

  pp.Backtrack();
}

void KnorbaAgentPragmaHandler::lexSynchronizedBlock(
  Preprocessor&   pp,
  Token&          lastToken,
  PragmaParticle* pragma) 
{
  Token& token = lastToken;
  SynchronizedBlock* block = new SynchronizedBlock(_sm);
  block->setBegin(token.getLocation());
  if(token.is(tok::l_brace)) {
    int braceCount = 1;
    while(token.isNot(tok::eof) && braceCount > 0) {
      pp.Lex(token);
      if(token.is(tok::l_brace))
        braceCount++;
      if(token.is(tok::r_brace))
        braceCount--;
    }
    block->setEnd(token.getLocation());
    block->setIsOneLine(false);
  } else {
    while(token.isNot(tok::eof) && token.isNot(tok::semi)) {
      pp.Lex(token);
    }
    block->setEnd(token.getLocation());
    block->setIsOneLine(true);
  }
  _syncBlocks->insert(_syncBlocks->end(), block);
  pragma->addSynchronizedBlock(block);
}

ParserProxy::ParserProxy(const string& fileName) {
  string extension = fileName.substr(fileName.rfind('.') + 1, string::npos);
  isCPP = (extension == "C")   || (extension == "cpp") || (extension == "cxx")
       || (extension == "hpp") || (extension == "hxx");

  compilerInstance = new CompilerInstance();

  diagOptions = new DiagnosticOptions();
  diagOptions->ShowLocation = 1;
  diagOptions->ShowCarets   = 1;
  diagOptions->ShowColors   = 1;
  diagOptions->TabStop      = 4;

  diagClient  = new TextDiagnosticPrinter(llvm::errs(), diagOptions);
  diagsEngine = new DiagnosticsEngine(
                      IntrusiveRefCntPtr<DiagnosticIDs>(new DiagnosticIDs),
                      diagOptions, diagClient, true);

  compilerInstance->setDiagnostics(diagsEngine);

  compilerInstance->createFileManager();
  compilerInstance->createSourceManager(compilerInstance->getFileManager());

  compilerInvocation = new CompilerInvocation();
  CompilerInvocation::CreateFromArgs(*compilerInvocation, 0, 0, *diagsEngine);
  compilerInstance->setInvocation(compilerInvocation);

  HeaderSearchOptions& hSearchOpts = compilerInstance->getHeaderSearchOpts();
  hSearchOpts.UseBuiltinIncludes        = 0;
  hSearchOpts.UseStandardSystemIncludes = 1;
  hSearchOpts.UseStandardCXXIncludes    = 1;
  /// FIXME set something that makes sense
  hSearchOpts.AddPath(STD_INCLUDE_FOLDER, clang::frontend::System, true,
                        false);
  hSearchOpts.AddPath(STD_EXT_INCLUDE_FOLDER, clang::frontend::System, false,
                        true && true);
  hSearchOpts.AddPath(KNORBA_SYSTEM_INCLUDE_FOLDER, clang::frontend::Angled,
                        false, true);

  targetOptions = new TargetOptions();
  targetOptions->Triple = llvm::sys::getDefaultTargetTriple();

  compilerInstance->setTarget(
    TargetInfo::CreateTargetInfo(*diagsEngine, targetOptions));

  LangOptions& langOptions = compilerInstance->getLangOpts();
  langOptions.GNUMode      = 1;
  langOptions.Bool         = 1;
  langOptions.POSIXThreads = 1;
  if(isCPP) {
    langOptions.CPlusPlus        = 1;
    langOptions.CXXOperatorNames = 1;
    langOptions.RTTI             = 1;
    langOptions.Exceptions       = 1;
    langOptions.CXXExceptions    = 1;
  }

  compilerInstance->createPreprocessor();
  compilerInstance->createASTContext();

  SourceManager& sm = compilerInstance->getSourceManager();

  Preprocessor& prepro = compilerInstance->getPreprocessor();
  PragmaNamespace* knorbaNamespace = new PragmaNamespace(KNORBA_PRAGMA_NAME);
  prepro.AddPragmaHandler(knorbaNamespace);

  agentPragmaHandler = new KnorbaAgentPragmaHandler(&sm);
  knorbaNamespace->AddPragma(agentPragmaHandler);
  prepro.getBuiltinInfo().InitializeBuiltins(
    prepro.getIdentifierTable(), langOptions);
  prepro.addPPCallbacks(agentPragmaHandler);
  const FileEntry* fileEntry = compilerInstance->getFileManager()
                                  .getFile(fileName, true);
  if(!fileEntry) {
    printf("File not found.\n");
    exit(1);
  }

  Rewriter rw;
  rw.setSourceMgr(sm, langOptions);

  FileID mainFileId = sm.createMainFileID(fileEntry);

  compilerInstance->getDiagnosticClient().BeginSourceFile(langOptions, &prepro);
  
  ASTContext& astContext = compilerInstance->getASTContext();
  astConsumer = new ASTConsumer();

  ParseAST(prepro, astConsumer, astContext);

  /*
  sema = new Sema(prepro, astContext, *astConsumer, TU_Complete);
  parser = new Parser(prepro, *sema, false);
  prepro.EnterMainSourceFile();
  parser->Initialize();
  astConsumer->Initialize(astContext);
  Parser::DeclGroupPtrTy aDecl;
  while(!parser->ParseTopLevelDecl(aDecl))
    if(aDecl)
      astConsumer->HandleTopLevelDecl(aDecl.getAsVal<DeclGroupRef>());
  */

  ScopeMap* scopeMap = new ScopeMap();
  KnorbaASTVisitor visitor(astContext, scopeMap);
  visitor.TraverseDecl(astContext.getTranslationUnitDecl());

  vector<PragmaParticle*>* particles = agentPragmaHandler->getParticles();
  vector<SynchronizedBlock*>* syncBlocks = agentPragmaHandler->getSyncBlocks();

  for(ScopeMap::iterator i = scopeMap->begin(); i != scopeMap->end(); i++) {
    ParticleScope* scope = i->second;
    for(vector<PragmaParticle*>::iterator j = particles->begin(); 
        j != particles->end(); j++) 
    {
      PragmaParticle* p = *j;
      if(scope->contains(*p)) {
        scope->addParticle(p);
      }
    }
  }

  for(ScopeMap::iterator it = scopeMap->begin(); it != scopeMap->end(); it++) {
    cout << endl;
    cout << it->second->toString() << endl;
  }

  for(ScopeMap::iterator it = scopeMap->begin(); it != scopeMap->end(); it++) {
    ParticleScope* scope = it->second;
    int s = scope->getNumberOfSharedVariables();
    SourceLocation lastEnd;
    for(int i = 0; i < s; i++) {
      SharedVariable* svar = scope->getSharedVariable(i);
      SourceLocation begin = svar->getBegin();

      // TODO getLocWithOffset is a temporary fix for earasing semicolons
      SourceLocation end = svar->getEnd().getLocWithOffset(2);
      rw.RemoveText(SourceRange(begin, end));
      lastEnd = end;
    }
    
    string sharedTypeName = "__k_" + scope->getName() + "_shared";
    const static string sharedVarName = "__k_shared";
    string text;
    text += "\n";
    text += sharedTypeName + "* " + sharedVarName + " = malloc(sizeof("
            + sharedTypeName + "));\n";

    for(int i = 0; i < s; i++) {
      SharedVariable* svar = scope->getSharedVariable(i);
      text += "#define " + svar->getName() + " (" + sharedVarName 
              + "->" + svar->getName() + ")\n";
    }

    if(s > 0)
      text += "\n";
    
    for(int i = 0; i < s; i++) {
      SharedVariable* svar = scope->getSharedVariable(i);
      Expr* init = svar->getInitializer();
      if(init == 0)
        continue;

      text += svar->getName() + " = "
              + rw.ConvertToString(svar->getInitializer()) + ";\n";
    }

    rw.InsertTextAfter(lastEnd, StringRef(text));

    int np = scope->getNumberOfParticles();
    for(int i = 0; i < np; i++) {
      PragmaParticle* p = scope->getParticle(i);
      rw.RemoveText(*p);
      text = "__k_system_setActiveParticle(&" + p->getThreadName() + ", "
           + sharedVarName + ");\n";
      rw.InsertTextAfter(p->getEnd(), StringRef(text));
    }

    text = "\n";
    for(int i = 0; i < s; i++) {
      SharedVariable* var = scope->getSharedVariable(i);
      text += "#undef " + var->getName() + "\n";
    }

    rw.InsertTextBefore(scope->getEnd(), StringRef(text));

    text = "typedef struct {\n";
    for(int i = 0; i < s; i++) {
      SharedVariable* var = scope->getSharedVariable(i);
      text += "  " + var->getType().getAsString() + " " + var->getName() 
            + ";\n";
    }
    text += "} " + sharedTypeName + ";\n\n";
    
    for(int i = 0; i < np; i++) {
      PragmaParticle* p = scope->getParticle(i);
      text += "void* " + p->getThreadName() + "(void* arg) {\n";
      text += "  __k_GridParticleParam_t* param = " 
              "(__k_GridParticleParam_t*)arg;\n";
      text += "\n";
      text += "  " + sharedTypeName + "* __k_shared = (" + sharedTypeName
            + "*)param->shared;\n\n";
      for(int j = 0; j < s; j++) {
        SharedVariable* svar = scope->getSharedVariable(j);
        text += "  #define " + svar->getName() + "(__k_shared->" 
              + svar->getName() +")\n";
      }
      text += "\n";

      text += "  pthread_mutex_t __k_mutex = PTHREAD_MUTEXT_INITIALIZER;\n";
      text += "\n";
      text += "  while(ture) {\n";
      
      if(p->getNumberOfDimensions() != 1) {
        text += "    // For this demonstation, only 1-d grid is supported";
      } else {

        string counterVarName = "__k_counter_" + p->getDimensionName(0);
      
        text += "    for(VERY_LONG_UNIT " + counterVarName + " = params->begin; "
              + counterVarName + " <= params->end; " + counterVarName 
              + "++) \n";

        const char* ubBegin = sm.getCharacterData(p->getBegin());
        const char* ubEnd   = sm.getCharacterData(p->getEnd());
        string userBlock = string(ubBegin, ubEnd);
        
        int nsb = p->getNumberOfSynchronizedBlocks();
        for(int j = 0; j < nsb; j++) {
          SynchronizedBlock* block = p->getSynchronizedBlock(j);
          const char* sbBegin = sm.getCharacterData(block->getBegin());
          const char* sbEnd = sm.getCharacterData(block->getEnd());
          int beginOffset = sbBegin - ubBegin;
          int endOffset = sbEnd - ubBegin + 1;
          string beginStr = "pthread_mutex_lock(__k_mutex);\n";
          string endStr = "\npthread_mutex_unlock(__k_mutex);\n";
          userBlock.insert(beginOffset, beginStr);
          userBlock.insert(endOffset + beginStr.size(), endStr);
          int pos = 0;
          const static string SYNC_STR = "__synchronized";
          while((pos = userBlock.find(SYNC_STR)) != -1)
          {
            userBlock.replace(pos, SYNC_STR.size(), "");
          }
        }

        text += userBlock;
        text += "    \n";
      }
      text += "  }\n";
      for(int j = 0; j < s; j++) {
        SharedVariable* svar = scope->getSharedVariable(j);
        text += "  #undef " + svar->getName() + "\n";
      }
      text += "  __k_GridParticleParam_delete(params)\n";
      text += "}\n\n";
    }
    rw.InsertTextBefore(scope->getBegin(), StringRef(text));
  }

  const RewriteBuffer* rwbuffer = rw.getRewriteBufferFor(sm.getMainFileID());
  if(rwbuffer != 0) {
    cout << string(rwbuffer->begin(), rwbuffer->end()) << endl;
  } else {
    cout << "--empty buffer--" << endl;
  }

  delete scopeMap;
}

void 
ParserProxy::rewrite(const string& fileName) {
}


ParserProxy::~ParserProxy() {
  delete compilerInstance;
  /* The following are deleted by compilerInstance */
  // delete diagOptions;
  // delete diagClient
  // delete diagsEngine;
  // delete compilerInvocation
  // delete targetOptions;
  // delete agentPragmaHandler;
  delete astConsumer;
  //  delete parser;
  delete sema;
}

} // namespace cxxpreprocessor
} // namespace knorba
} // namespace org

