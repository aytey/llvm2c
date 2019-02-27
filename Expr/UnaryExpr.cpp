#include "UnaryExpr.h"

#include "llvm/Support/raw_ostream.h"

/*
 * UnaryExpr classes
 */

UnaryExpr::UnaryExpr(Expr *expr) {
    this->expr = expr;
    setType(expr->getType()->clone());
}

RefExpr::RefExpr(Expr* expr) :
    UnaryExpr(expr) {
    setType(std::make_unique<PointerType>(expr->getType()->clone()));
}

void RefExpr::print() const {
    llvm::outs() << toString();
}

std::string RefExpr::toString() const {
    return "&(" + expr->toString() + ")";
}

DerefExpr::DerefExpr(Expr* expr) :
    UnaryExpr(expr) {
    auto PT = static_cast<PointerType*>(expr->getType());
    setType(PT->type->clone());
}

void DerefExpr::print() const {
    llvm::outs() << toString();
}

std::string DerefExpr::toString() const {
    RefExpr* refExpr = nullptr;
    if ((refExpr = dynamic_cast<RefExpr*>(expr)) != nullptr) {
        return refExpr->expr->toString();
    } else {
        return "*(" + expr->toString() + ")";
    }
}

RetExpr::RetExpr(Expr* ret)
    : UnaryExpr(ret) { }

RetExpr::RetExpr()
    : UnaryExpr(nullptr) { }

void RetExpr::print() const {
    llvm::outs() << toString();
}

std::string RetExpr::toString() const {
    std::string ret;

    ret += "return";
    if (expr != nullptr) {
        ret += " " + expr->toString();
    }

    return ret + ";";
}

CastExpr::CastExpr(Expr* expr, std::unique_ptr<Type> type)
    : UnaryExpr(expr) {
    setType(std::move(type));
}

void CastExpr::print() const {
    llvm::outs() << toString();
}

std::string CastExpr::toString() const {
    std::string ret;

    ret += "(" + getType()->toString() + ")";
    if (expr != nullptr) {
        ret += expr->toString();
    }

    return ret;
}
