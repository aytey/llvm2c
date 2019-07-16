#include <llvm/IR/Instruction.h>

#include "../core/Program.h"
#include "../core/Func.h"
#include "../core/Block.h"

#include "SimplifyingExprVisitor.h"

class ArrowifyVisitor : public SimplifyingExprVisitor {
public:
    std::vector<std::unique_ptr<Expr>> ownership;

    void visit(GepExpr& expr) override;

protected:
    Expr* simplify(Expr* expr) override;
};

void arrowify(const llvm::Module* module, Program& program) {

    ArrowifyVisitor visitor;
    for (const llvm::Function& func : module->functions()) {
        auto* function = program.getFunction(&func);
        for (const auto& block : func) {
            auto* myBlock = function->getBlock(&block);

            for (auto it = myBlock->expressions.begin(); it != myBlock->expressions.end(); ++it) {
                auto expr = *it;
                expr->accept(visitor);

            }

            for (auto&& expr : visitor.ownership) {
                myBlock->addOwnership(std::move(expr));
            }
        }
    }
}

Expr* ArrowifyVisitor::simplify(Expr* expr) {

    if (auto* SE = llvm::dyn_cast_or_null<StructElement>(expr)) {
        if (auto* DE = llvm::dyn_cast_or_null<DerefExpr>(SE->expr)) {
            auto arrow = std::make_unique<ArrowExpr>(SE->strct, SE->expr, SE->element);
            auto* result = arrow.get();
            ownership.push_back(std::move(arrow));
            return result;
        }
    }

    return expr;
}

void ArrowifyVisitor::visit(GepExpr& expr) {
    for (auto it = expr.indices.begin(); it != expr.indices.end(); ++it) {
        (*it)->accept(*this);
        auto uptr = std::move(*it);

        if (auto* SE = llvm::dyn_cast_or_null<StructElement>(uptr.get())) {
            if (auto* DE = llvm::dyn_cast_or_null<DerefExpr>(SE->expr)) {
                auto arrow = std::make_unique<ArrowExpr>(SE->strct, DE->expr, SE->element);
                uptr = std::move(arrow);
            }
        }

        *it = std::move(uptr);
    }
}
