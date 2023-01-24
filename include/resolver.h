//
// Created by lijin on 23-1-24.
//

#ifndef CCLOX_RESOLVER_H
#define CCLOX_RESOLVER_H

#include "expr.h"
#include "stmt.h"

namespace cclox {
class Resolver: public ExprVisitor, public StmtVisitor {

};
} //namespace cclox

#endif  // CCLOX_RESOLVER_H
