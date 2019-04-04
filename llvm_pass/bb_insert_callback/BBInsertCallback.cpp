#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/Instructions.h"
#include <iostream>
using namespace llvm;

namespace {
  struct BBInsertCallback : public FunctionPass {
    static char ID;
    BBInsertCallback() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      // Get the function to call from our runtime library.
      LLVMContext &Ctx = F.getContext();
      std::vector<Type*> paramTypes = {Type::getInt32Ty(Ctx)};
      Type *retType = Type::getVoidTy(Ctx);
      FunctionType *logFuncType = FunctionType::get(retType, paramTypes, false);
      //Constant *logFunc = F.getParent()->getOrInsertFunction("logop", logFuncType);

      for (auto &B : F) { //For each basic block in the function...
        if (F.getName() != "") { //Get function name
          BasicBlock::iterator IP = B.getFirstInsertionPt(); //Start at begining of basic block
          IRBuilder<> IRB(&(*IP));

          Value * args[] = {ConstantInt::get(IntegerType::get(Ctx, 64), 3)}; //Pass a constant of 3

          FunctionType *fType = FunctionType::get(Type::getVoidTy(Ctx), false);
          InlineAsm *IA_enter = InlineAsm::get(fType, "movq $$0xb, %rax; vmcall", "", true, false, InlineAsm::AsmDialect::AD_ATT);
          IRB.CreateCall(IA_enter, NULL);

	
          for (auto &I : B) {
		if (auto *op = dyn_cast<ReturnInst>(&I)) {
			IRBuilder<> builder(op);
			InlineAsm *IA_exit = InlineAsm::get(fType, "movq $$0xc, %rax; vmcall", "", true, false, InlineAsm::AsmDialect::AD_ATT);

              		builder.SetInsertPoint(&B, builder.GetInsertPoint());
			builder.CreateCall(IA_exit, NULL);
		}
	  }

          return true;
          /*
          for (auto &I : B) {
            if (auto *op = dyn_cast<BinaryOperator>(&I)) {
              // Insert *after* `op`.
              IRBuilder<> builder(op);
              builder.SetInsertPoint(&B, ++builder.GetInsertPoint());

              // Insert a call to our function.
              Value* args[] = {op};
              builder.CreateCall(logFunc, args);

              return true;
            }
          }
          */
        }
      }

      return false;
    }
  };
}

char BBInsertCallback::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerBBInsertCallback(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new BBInsertCallback());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerBBInsertCallback);
