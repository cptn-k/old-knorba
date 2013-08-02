
#include "ast-test.h"

using namespace std;
using namespace clang;
using namespace clang::tooling;

class Visitor : public RecursiveASTVisitor<Visitor> {
public:
  explicit Visitor(ASTContext* context) : context(context) {}
  
  bool VisitStmt(Stmt* s) {
    llvm::errs() << "Statement found:\n";
    s->dumpColor();
    return true;
  }

private:
  ASTContext* context;

};

  
class Consumer : public ASTConsumer {
public:
  explicit Consumer(ASTContext* context) : visitor(context) {}
  
  virtual void HandleTranslationUnit(ASTContext& context) {
    visitor.TraverseDecl(context.getTranslationUnitDecl());
  }

private:
  Visitor visitor;

};

class Action : public ASTFrontendAction {
public:
  virtual ASTConsumer* CreateASTConsumer(CompilerInstance& compiler,
      llvm::StringRef inFile)
  {
    HeaderSearchOptions& hsOptions = compiler.getHeaderSearchOpts();
    hsOptions.AddPath(STD_INCLUDE_FOLDER, clang::frontend::System, true,
                          false);
    hsOptions.AddPath(KNORBA_SYSTEM_INCLUDE_FOLDER, clang::frontend::Angled,
                          false, true);
    return new Consumer(&compiler.getASTContext());
  }
};

class ActionFactory : public FrontendActionFactory {
  virtual FrontendAction* create() {
    return new Action();
  }
};

int main(int argc, char** argv) {
  vector<string> commandLine;
  vector<string> sources;

  commandLine.insert(commandLine.end(), string("-I") + 
    string(KNORBA_SYSTEM_INCLUDE_FOLDER));

  if(argc > 1) {
    sources.insert(sources.end(), argv[1]);
  } else {
    cerr << "No file is specified as input." << endl;
    return -1;
  }

  FixedCompilationDatabase compilationDb(".", commandLine);
  ClangTool thisTool(compilationDb, sources);
  thisTool.run(new ActionFactory());
}
