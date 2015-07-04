#include "execution.h"

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/Support/TargetSelect.h>
#include "code.h"
#include "logging.h"

static std::unique_ptr<llvm::ExecutionEngine> Engine;

void prepareExecutionPipeline() {
}

static void prepareExecutionPipeline(llvm::Module* Module) {
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
  std::string Err;
  llvm::ExecutionEngine* ExecutionEngine =
      llvm::EngineBuilder(std::unique_ptr<llvm::Module>(Module))
          .setErrorStr(&Err)
          .setEngineKind(llvm::EngineKind::JIT)
          .create();
  CHECK(ExecutionEngine != nullptr) << Err;
  Engine.reset(ExecutionEngine);
}

void executionPipeline(llvm::Module* Module) {
  if (Engine == nullptr) {
    prepareExecutionPipeline(Module);
  } else {
    Engine->addModule(std::unique_ptr<llvm::Module>(Module));
  }
  llvm::Function* MainFunction = Module->getFunction(ToyMainFunctionName);
  if (MainFunction != nullptr) {
    Engine->finalizeObject();
    void* JITFunction = Engine->getPointerToFunction(MainFunction);
    CHECK(JITFunction != nullptr);
    double Value = reinterpret_cast<double (*)()>(JITFunction)();
    printf("%lf\n", Value);
  }
}

void finishExecutionPipeline() {
  Engine.reset(nullptr);
}

void executionMain(llvm::Module* Module) {
  prepareExecutionPipeline();
  llvm::Function* MainFunction = Module->getFunction(ToyMainFunctionName);
  CHECK(MainFunction != nullptr);
  executionPipeline(Module);
  finishExecutionPipeline();
}
