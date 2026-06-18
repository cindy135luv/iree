// Copyright 2026 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef IREE_COMPILER_DIALECT_POLYCROSS_TRANSFORMS_PASSES_H_
#define IREE_COMPILER_DIALECT_POLYCROSS_TRANSFORMS_PASSES_H_

#include "mlir/Pass/Pass.h"

namespace mlir::iree_compiler::IREE::PolyCross {

#define GEN_PASS_DECL
#include "iree/compiler/Dialect/PolyCross/Transforms/Passes.h.inc" // IWYU pragma: keep

void registerPasses();

} // namespace mlir::iree_compiler::IREE::PolyCross

#endif // IREE_COMPILER_DIALECT_POLYCROSS_TRANSFORMS_PASSES_H_
