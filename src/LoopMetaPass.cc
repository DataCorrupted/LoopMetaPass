#include "llvm/ADT/SmallSet.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopInfoImpl.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/MDBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils.h"

#include <set>

using namespace llvm;
#define DEBUG_TYPE "loop-meta"
namespace {

class LoopMetaPass : public LoopPass {
public:
  static char ID;

  LoopMetaPass() : LoopPass(ID) {}
  bool doInitialization(Loop *, LPPassManager &LPM) override;
  bool runOnLoop(Loop *, LPPassManager &LPM) override;
  bool doFinalization() override;
  void getAnalysisUsage(AnalysisUsage &AU) const override;

  bool instrumentBackEdge(Loop *);
  bool instrumentInductionVariable(Loop *, ScalarEvolution &);
};
} // namespace

char LoopMetaPass::ID = 0;
bool LoopMetaPass::doInitialization(Loop *L, LPPassManager &LPM) {
  return false;
}

// Get filename:lineno from an instruction.
static inline std::string getMessage(Instruction *I) {
  // dbgs() << *I << "\n";
  // Get line number
  unsigned Line = I->getDebugLoc().getLine();
  // Get file name
  Module *M = I->getParent()->getParent()->getParent();
  const std::string &FileName = M->getSourceFileName();
  std::string Message = FileName + ":" + std::to_string(Line);
  return Message;
}

static inline void setMetadata(Instruction *I, const char *Key,
                               std::string &Message) {
  LLVMContext &C = I->getContext();
  MDNode *N = MDNode::get(C, MDString::get(C, Message));
  I->setMetadata(Key, N);
}

void LoopMetaPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequiredID(LoopSimplifyID);
  AU.addRequired<ScalarEvolutionWrapperPass>();
  AU.addPreserved<ScalarEvolutionWrapperPass>();
}

bool LoopMetaPass::instrumentBackEdge(Loop *L) {
  assert(L->getNumBackEdges() == 1 &&
         "Simple loops should have only one back edge.");

  BasicBlock *BB = L->getLoopLatch();

  // Get terminator block, or back edge.
  Instruction *TermInst = BB->getTerminator();
  auto Message = getMessage(TermInst);
  // Prepare metadata
  dbgs() << "Add metadata to backedge: " << Message << "\n";
  setMetadata(TermInst, "LoopMetaBackEdge", Message);
  return false;

  // Search all blocks for backedge.
  /*
  BasicBlock *Header = L->getHeader();
  std::set<BasicBlock *> preds(pred_begin(Header), pred_end(Header));
  for (BasicBlock *BB : L->getBlocks()) {
    if (preds.count(BB) == 0)
      continue;
    // Get terminator block, or back edge.
    Instruction *TermInst = BB->getTerminator();
    auto Message = getMessage(TermInst);
    // Prepare metadata
    dbgs() << "Add metadata to backedge: " << Message << "\n";
    setMetadata(TermInst, "LoopMetaBackEdge", Message);
  }
  */
}

bool LoopMetaPass::instrumentInductionVariable(Loop *L, ScalarEvolution &SE) {
  PHINode *IV = L->getInductionVariable(SE);
  if (!IV) {
    dbgs() << "No induction variable found\n";
    return false;
  }
  // IV itself doesn't have a location, use the instruction that
  // updates IV as our debug location.
  Instruction *IVAdd = nullptr;
  for (Value *V : IV->incoming_values()) {
    Instruction *I = dyn_cast<Instruction>(V);
    if (!I)
      continue;
    if (IV == I->getOperand(0) || IV == I->getOperand(1)) {
      IVAdd = I;
    }
  }
  assert(IVAdd && "Can't find instruction that changed IV.");
  auto Message = getMessage(IVAdd);
  // Prepare metadata
  dbgs() << "Add metadata to induction variable: " << Message << "\n";
  setMetadata(IV, "LoopMetaInVar", Message);
  return false;
}

bool LoopMetaPass::runOnLoop(Loop *L, LPPassManager &LPM) {
  // Assume we only run on simple loops.
  dbgs() << "Run on Loop: " << L->getName() << "\n";
  bool modified = false;
  modified |= instrumentBackEdge(L);
  auto &SE = getAnalysis<ScalarEvolutionWrapperPass>().getSE();
  modified |= instrumentInductionVariable(L, SE);
  return modified;
}
bool LoopMetaPass::doFinalization() { return false; }

static RegisterPass<LoopMetaPass>
    X("loop_meta_pass", "Pass to add metadata to loops", false, false);
