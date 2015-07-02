#ifndef TOY_AST_H_
#define TOY_AST_H_

#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>

enum ASTType {
  NUMBER_EXPR_AST,
  VARIABLE_EXPR_AST,
  BINARY_EXPR_AST,
  PROTOTYPE_AST,
  FUNCTION_AST,
  CALL_EXPR_AST,
};

class ExprAST {
 public:
  ExprAST(ASTType Type) : Type_(Type) {
  }

  virtual ~ExprAST() {
  }

  ASTType type() const {
    return Type_;
  }

  virtual void dump(int Indent = 0) const = 0;
  virtual llvm::Value* codegen() = 0;

 private:
  ASTType Type_;
};

class NumberExprAST : public ExprAST {
 public:
  NumberExprAST(double Val) : ExprAST(NUMBER_EXPR_AST), Val_(Val) {
  }

  void dump(int Indent = 0) const override;
  llvm::Value* codegen() override;

 private:
  double Val_;
};

class VariableExprAST : public ExprAST {
 public:
  VariableExprAST(const std::string& Name)
      : ExprAST(VARIABLE_EXPR_AST), Name_(Name) {
  }

  void dump(int Indent = 0) const override;
  llvm::Value* codegen() override;

 private:
  const std::string Name_;
};

class BinaryExprAST : public ExprAST {
 public:
  BinaryExprAST(char Op, ExprAST* Left, ExprAST* Right)
      : ExprAST(BINARY_EXPR_AST), Op_(Op), Left_(Left), Right_(Right) {
  }

  void dump(int Indent = 0) const override;
  llvm::Value* codegen() override;

 private:
  char Op_;
  ExprAST* Left_;
  ExprAST* Right_;
};

class PrototypeAST : public ExprAST {
 public:
  PrototypeAST(const std::string& Name, const std::vector<std::string>& Args)
      : ExprAST(PROTOTYPE_AST), Name_(Name), Args_(Args) {
  }

  void dump(int Indent = 0) const override;
  llvm::Function* codegen() override;

 private:
  const std::string Name_;
  std::vector<std::string> Args_;
};

class FunctionAST : public ExprAST {
 public:
  FunctionAST(PrototypeAST* Prototype, ExprAST* Body)
      : ExprAST(FUNCTION_AST), Prototype_(Prototype), Body_(Body) {
  }

  void dump(int Indent = 0) const override;
  llvm::Function* codegen() override;

 private:
  PrototypeAST* Prototype_;
  ExprAST* Body_;
};

class CallExprAST : public ExprAST {
 public:
  CallExprAST(const std::string& Callee, const std::vector<ExprAST*>& Args)
      : ExprAST(CALL_EXPR_AST), Callee_(Callee), Args_(Args) {
  }

  void dump(int Indent = 0) const override;
  llvm::Value* codegen() override;

 private:
  const std::string Callee_;
  const std::vector<ExprAST*> Args_;
};

std::vector<ExprAST*> parseMain();

#endif  // TOY_AST_H_
