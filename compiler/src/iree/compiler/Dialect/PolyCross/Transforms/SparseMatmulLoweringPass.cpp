// Copyright 2026 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "iree/compiler/Dialect/PolyCross/Transforms/Passes.h"
#include "mlir/Dialect/Linalg/IR/Linalg.h"

namespace mlir::iree_compiler::IREE::PolyCross {

#define GEN_PASS_DEF_SPARSEMATMULLOWERINGPASS
#include "iree/compiler/Dialect/PolyCross/Transforms/Passes.h.inc"

namespace {

// SCAFFOLD: walks linalg.matmul ops but does not transform them yet.
//
// Next milestone: for a matmul whose weight operand was pruned (carries a
// polycross.mask / zeroed values), generate explicit scf/affine loops that
// skip the pruned entries instead of multiplying by zero.
struct SparseMatmulLoweringPass
    : impl::SparseMatmulLoweringPassBase<SparseMatmulLoweringPass> {
  using Base::Base;

  void runOnOperation() override {
    getOperation()->walk([&](linalg::MatmulOp matmul) {
      // TODO(polycross): lower this pruned matmul into sparse-aware loops.
      (void)matmul;
    });
  }
};

} // namespace
} // namespace mlir::iree_compiler::IREE::PolyCross
