#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"

using namespace clang;

namespace {

// RecursiveASTVisitor traverses the AST node by node
class ImplicitCastCounterVisitor
    : public RecursiveASTVisitor<ImplicitCastCounterVisitor> {
public:
  explicit ImplicitCastCounterVisitor(ASTContext *Context) : Context(Context) {}

  // This method is called automatically for every ImplicitCastExpr node found
  // in the AST.
  bool VisitImplicitCastExpr(ImplicitCastExpr *cast) {
    cast->dump(); // Dumps the internal structure of the node to stderr

    return true;
  }

private:
  ASTContext *Context;
};

// ASTConsumer is the interface used to consume the AST produced by the Clang
// parser
class ImplicitCastCounterConsumer : public ASTConsumer {
public:
  // Called by the compiler when the entire translation unit has been
  // parsed
  void HandleTranslationUnit(ASTContext &Context) override {
    // Initialize our visitor with the context
    ImplicitCastCounterVisitor Visitor(&Context);

    // Start the traversal from the root of the Translation Unit
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }
};

// PluginASTAction is the entry point for our plugin
class ImplicitCastCounterAction : public PluginASTAction {
protected:
  // Creates the ASTConsumer instance.
  virtual std::unique_ptr<ASTConsumer>
  CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
    return std::make_unique<ImplicitCastCounterConsumer>();
  }

  // Parses command line arguments passed to the plugin (if any)
  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override {
    return true; // We don't use arguments
  }

  // Automatic plugin launch when the -fplugin flag is specified
  PluginASTAction::ActionType getActionType() override {
    return AddBeforeMainAction;
  }
};

} // namespace

// Registering a plugin
static FrontendPluginRegistry::Add<ImplicitCastCounterAction>
    X("implicit-cast-counter", "Counts implicit casts");
