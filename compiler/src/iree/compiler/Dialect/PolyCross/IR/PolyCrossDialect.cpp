// Copyright 2026 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "iree/compiler/Dialect/PolyCross/IR/PolyCrossDialect.h"

#include "iree/compiler/Dialect/PolyCross/IR/PolyCrossOps.h"

namespace mlir::iree_compiler::IREE::PolyCross {

void PolyCrossDialect::initialize() {
  addOperations<
#define GET_OP_LIST
#include "iree/compiler/Dialect/PolyCross/IR/PolyCrossOps.cpp.inc" // IWYU pragma: keep
      >();
}

} // namespace mlir::iree_compiler::IREE::PolyCross

#include "iree/compiler/Dialect/PolyCross/IR/PolyCrossDialect.cpp.inc"
