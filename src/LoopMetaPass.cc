#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/MDBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {

class LoopMetaPass : public LoopPass {
public:
  static char ID;

  LoopMetaPass() : LoopPass(ID) {}
  bool doInitialization(Loop *, LPPassManager &LPM) override;
  bool runOnLoop(Loop *, LPPassManager &LPM) override;
  bool doFinalization() override;
};

} // namespace

char LoopMetaPass::ID = 0;
bool LoopMetaPass::doInitialization(Loop *L, LPPassManager &LPM) {
  return false;
}
bool LoopMetaPass::runOnLoop(Loop *L, LPPassManager &LPM) {
  // Assume we only run on simple loops.
  assert(L->getNumBackEdges() == 1 &&
         "Simple loops should have only one back edge.");

  BasicBlock *BB = L->getLoopLatch();

  // Get terminator block, or back edge.
  Instruction *TermInst = BB->getTerminator();
  // Get line number
  unsigned Line = TermInst->getDebugLoc().getLine();
  // Get file name
  Module *M = BB->getParent()->getParent();
  StringRef FileName = M->getName();
  std::string Message = FileName.str() + ":" + std::to_string(Line);
  // dbg(errs() << Message << "\n");
  // Prepare metadata
  LLVMContext &C = TermInst->getContext();

  MDNode *N = MDNode::get(C, MDString::get(C, Message));
  TermInst->setMetadata("LoopMeta", N);
  return true;
}
bool LoopMetaPass::doFinalization() { return false; }

static void registerLoopMetaPass(const PassManagerBuilder &,
                                 legacy::PassManagerBase &PM) {
  PM.add(new LoopMetaPass());
}

static RegisterPass<LoopMetaPass>
    X("loop_meta_pass", "Pass to add metadata to loops", false, false);

static RegisterStandardPasses
    RegisterLoopMetaPass0(PassManagerBuilder::EP_EnabledOnOptLevel0,
                          registerLoopMetaPass);