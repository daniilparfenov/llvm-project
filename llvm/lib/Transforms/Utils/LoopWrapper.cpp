#include "llvm/Transforms/Utils/LoopWrapper.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

PreservedAnalyses LoopWrapperPass::run(Loop &L, LoopAnalysisManager &AM,
                                       LoopStandardAnalysisResults &AR,
                                       LPMUpdater &U) {

  // We expect LoopSimplify pass to have formed a preheader. If it failed to do
  // so (e.g., due to irreducible control flow (goto code or other strangeness)
  // or abnormal exits), we cannot safely insert the loop_start call.
  // Skip this loop.
  BasicBlock *Preheader = L.getLoopPreheader();
  if (!Preheader)
    return PreservedAnalyses::all();

  // Find or declare loop_start/loop_end functions
  Module *M = Preheader->getModule();
  Type *VoidTy = Type::getVoidTy(M->getContext());
  FunctionType *FuncTy = FunctionType::get(VoidTy, false);
  FunctionCallee StartFunc = M->getOrInsertFunction("loop_start", FuncTy);
  FunctionCallee EndFunc = M->getOrInsertFunction("loop_end", FuncTy);

  // Insert loop_start function at the end of preheader
  IRBuilder<> Builder(Preheader->getTerminator());
  Builder.CreateCall(StartFunc);

  // Insert loop_end function corresponding with each exit edge of CFG
  SmallVector<Loop::Edge, 4> ExitEdges;
  L.getExitEdges(ExitEdges);
  for (auto Edge : ExitEdges) {
    // Split exit edge and insert loop_end function into a new basic block
    BasicBlock *NewBB = SplitEdge(Edge.first, Edge.second, &AR.DT, &AR.LI);
    IRBuilder<> ExitBuilder(NewBB, NewBB->getFirstNonPHIIt());
    ExitBuilder.CreateCall(EndFunc);
  }

  // We have carefully updated DominatorTree and LoopInfo using SplitEdge,
  // so we can inform the Pass Manager that these analyses remain valid.
  PreservedAnalyses PA;
  PA.preserve<DominatorTreeAnalysis>();
  PA.preserve<LoopAnalysis>();
  return PA;
}