#ifndef LLVM_TRANSFORMS_UTILS_LOOPWRAPPER_H
#define LLVM_TRANSFORMS_UTILS_LOOPWRAPPER_H

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"

namespace llvm {

class LoopWrapperPass : public PassInfoMixin<LoopWrapperPass> {
public:
  PreservedAnalyses run(Loop &L, LoopAnalysisManager &AM,
                        LoopStandardAnalysisResults &AR, LPMUpdater &U);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_LOOPWRAPPER_H