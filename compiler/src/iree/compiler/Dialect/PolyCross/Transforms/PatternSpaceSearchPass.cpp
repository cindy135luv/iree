// Copyright 2026 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "iree/compiler/Dialect/PolyCross/Transforms/Passes.h"
#include "llvm/ADT/SmallVector.h"

namespace mlir::iree_compiler::IREE::PolyCross {

#define GEN_PASS_DEF_PATTERNSPACESEARCHPASS
#include "iree/compiler/Dialect/PolyCross/Transforms/Passes.h.inc"

namespace {

// One point in the pruning pattern search space.
struct PatternCandidate {
  int64_t unitSize; // size of the Sorted Unit
  int64_t keep;     // values kept per unit
  double score;     // higher is better; filled by a cost model (TODO)
};

// SCAFFOLD: builds the candidate pattern space but does not transform the IR.
//
// Next milestone: score each candidate with a cost/accuracy model and record
// the winner for AutoTunePattern to apply.
struct PatternSpaceSearchPass
    : impl::PatternSpaceSearchPassBase<PatternSpaceSearchPass> {
  using Base::Base;

  void runOnOperation() override {
    SmallVector<PatternCandidate> space;
    for (int64_t unit : {4, 8, 16})
      for (int64_t keep = 1; keep < unit; ++keep)
        space.push_back({unit, keep, /*score=*/0.0});

    // TODO(polycross): score candidates and pick the best.
    (void)space;
  }
};

} // namespace
} // namespace mlir::iree_compiler::IREE::PolyCross
