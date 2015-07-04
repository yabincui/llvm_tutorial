#ifndef TOY_EXECUTION_H_
#define TOY_EXECUTION_H_

#include <memory>
#include <llvm/IR/Module.h>

// Used in interactive mode.
void prepareExecutionPipeline();
void executionPipeline(llvm::Module* Module);
void finishExecutionPipeline();

// Used in non-interactive mode.
void executionMain(llvm::Module* Module);

#endif  // TOY_EXECUTION_H_
