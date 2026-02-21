#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"

#include <map>
#include <string>

using namespace clang;

namespace {

// RecursiveASTVisitor traverses the AST node by node
class ImplicitCastCounterVisitor
    : public RecursiveASTVisitor<ImplicitCastCounterVisitor> {
public:
  explicit ImplicitCastCounterVisitor(ASTContext *Context) : Context(Context) {}

  const auto &getImplicitCastInfo() const { return ImplicitCastInfo; }

  // This method is called automatically for every ImplicitCastExpr node found
  // in the AST.
  bool VisitImplicitCastExpr(ImplicitCastExpr *Cast) {

    // Skip casts in sys headers
    if (Context->getSourceManager().isInSystemHeader(Cast->getExprLoc())) {
      return true;
    }

    // Casted types definition
    QualType SrcType = Cast->getSubExpr()->getType().getUnqualifiedType();
    QualType DstType = Cast->getType().getUnqualifiedType();

    // Converting to str
    std::string SrcTypeStr = SrcType.getAsString();
    std::string DstTypeStr = DstType.getAsString();

    // Counting found cast
    if (SrcTypeStr != DstTypeStr) {
      std::string CastLoc = getImplicitCastLoc(Cast);
      std::string CastDescription = SrcTypeStr + " -> " + DstTypeStr;
      ImplicitCastInfo[CastLoc][CastDescription]++;
    }

    return true;
  }

private:
  ASTContext *Context;

  // A map to store location, type and quantity of casts
  std::map<std::string, std::map<std::string, int>> ImplicitCastInfo;

  // Explores a location of cast (function or not)
  std::string getImplicitCastLoc(const ImplicitCastExpr *Cast) {
    auto Parents = Context->getParents(*Cast);

    while (!Parents.empty()) {
      const auto &ParentNode = Parents[0];

      if (const FunctionDecl *FD = ParentNode.get<FunctionDecl>()) {
        return "Function `" + FD->getNameAsString() + "`";
      }

      Parents = Context->getParents(ParentNode);
    }

    return "Not a Function";
  }
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

    // Cast info output
    for (const auto &CastInfo : Visitor.getImplicitCastInfo()) {
      llvm::outs() << CastInfo.first << "\n";
      for (const auto &CastMembers : CastInfo.second) {
        llvm::outs() << CastMembers.first << ": " << CastMembers.second << "\n";
      }
      llvm::outs() << "\n";
    }
  }
};

// PluginASTAction is the entry point for our plugin
class ImplicitCastCounterAction : public PluginASTAction {
protected:
  // Creates the ASTConsumer instance.
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
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
