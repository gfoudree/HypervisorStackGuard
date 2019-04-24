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
#include "llvm/IR/Constants.h"

#include <iostream>
#include <set>

using namespace llvm;

namespace {
  struct BBInsertCallback : public FunctionPass {
    static char ID;
    BBInsertCallback() : FunctionPass(ID) {}

    std::set<Function *> taggedFunctions;

    virtual bool doInitialization(Module &M) override {
	    findTaggedFunctions(&M);
	    return false;
    }

    bool hookFunctionCheck(Function &F) {
	return taggedFunctions.find(&F) != taggedFunctions.end();	
    }

    void findTaggedFunctions(Module *M) {
	//Have to go through the metadata unforunately to find the annotations...
	for (Module::global_iterator it = M->global_begin(), E = M->global_end(); it != E; it++) {
		if (it->getName() == "llvm.global.annotations") {
			ConstantArray *CA = dyn_cast<ConstantArray>(it->getOperand(0));
			for (auto OI = CA->op_begin(); OI != CA->op_end(); ++OI) {
			    ConstantStruct *CS = dyn_cast<ConstantStruct>(OI->get());
			    Function *FUNC = dyn_cast<Function>(CS->getOperand(0)->getOperand(0));
			    GlobalVariable *AnnotationGL = dyn_cast<GlobalVariable>(CS->getOperand(1)->getOperand(0));
			    StringRef annotation = dyn_cast<ConstantDataArray>(AnnotationGL->getInitializer())->getAsCString();
			    if (annotation.compare("StackGuard") == 0) {
			      taggedFunctions.insert(FUNC);
			    }
			}
		}
	}
    }

    virtual bool runOnFunction(Function &F) {
      if (hookFunctionCheck(F)) {
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

		  //Value * args[] = {ConstantInt::get(IntegerType::get(Ctx, 64), 3)}; //Pass a constant of 3

		  FunctionType *fType = FunctionType::get(Type::getVoidTy(Ctx), false);
		  InlineAsm *IA_enter = InlineAsm::get(fType, "movl $$0xb, %eax; movl $$0x1, %ebx; vmcall", "", true, false, InlineAsm::AsmDialect::AD_ATT);
		  IRB.CreateCall(IA_enter, NULL); //Insert in vmcall at begining of function


		  for (auto &I : B) {
				if (auto *op = dyn_cast<ReturnInst>(&I)) {
					//std::cout << I.getOpcodeName() << std::endl;
					std::cout << "Inserting function trap in " << F.getName().str() << "()" << std::endl;
					IRBuilder<> builder(op);
					InlineAsm *IA_exit = InlineAsm::get(fType, "movl $$0xb, %eax; movl $$0x2, %ebx; vmcall", "", true, false, InlineAsm::AsmDialect::AD_ATT);

					//builder.SetInsertPoint(&B, builder.GetInsertPoint());
					builder.SetInsertPoint(&I);
					builder.CreateCall(IA_exit, NULL); //Insert in vmcall at end of function
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
