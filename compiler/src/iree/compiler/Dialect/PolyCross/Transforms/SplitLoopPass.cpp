// Copyright 2026 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "iree/compiler/Dialect/PolyCross/IR/PolyCrossOps.h"
#include "iree/compiler/Dialect/PolyCross/Transforms/Passes.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/SCF/IR/SCF.h"
#include "mlir/IR/Builders.h"
#include "llvm/ADT/SmallVector.h"

namespace mlir::iree_compiler::IREE::PolyCross {

#define GEN_PASS_DEF_SPLITLOOPPASS
#include "iree/compiler/Dialect/PolyCross/Transforms/Passes.h.inc"

namespace {

// Splits one marked `scf.for` into an outer/inner nested loop pair:
//
//   scf.for %io = %lb to %ub step %tile {
//     %ub_inner = min(%ub, %io + %tile)
//     scf.for %ii = %io to %ub_inner step %step { ...original body... }
//   }
//
// v1 assumes the loop has step 1 and no iter_args/results.
static void splitLoop(scf::ForOp forOp, SplitLoopOp marker) {
  int64_t tileSize = marker.getTileSize();
  OpBuilder builder(forOp);
  Location loc = forOp.getLoc();

  Value lb = forOp.getLowerBound();
  Value ub = forOp.getUpperBound();
  Value step = forOp.getStep();

  // Outer loop walks tile-by-tile.
  Value tile = arith::ConstantIndexOp::create(builder, loc, tileSize);
  auto outer = scf::ForOp::create(builder, loc, lb, ub, tile);

  // Inner loop walks inside one tile; `min` keeps the last tile in bounds.
  builder.setInsertionPointToStart(outer.getBody());
  Value io = outer.getInductionVar();
  Value ioPlusTile = arith::AddIOp::create(builder, loc, io, tile);
  Value innerUb = arith::MinUIOp::create(builder, loc, ub, ioPlusTile);
  auto inner = scf::ForOp::create(builder, loc, io, innerUb, step);

  // Move the original body into the inner loop and drop the marker.
  marker.erase();
  forOp.getInductionVar().replaceAllUsesWith(inner.getInductionVar());
  Block *innerBody = inner.getBody();
  Block *origBody = forOp.getBody();
  // Splice every original op except the trailing scf.yield.
  innerBody->getOperations().splice(
      innerBody->getTerminator()->getIterator(), origBody->getOperations(),
      origBody->begin(), std::prev(origBody->end()));

  forOp.erase();
}

struct SplitLoopPass : impl::SplitLoopPassBase<SplitLoopPass> {
  using Base::Base;

  void runOnOperation() override {
    // Collect targets first; we mutate the IR (create/erase loops) afterwards.
    SmallVector<std::pair<scf::ForOp, SplitLoopOp>> targets;
    getOperation()->walk([&](scf::ForOp forOp) {
      // v1: only simple loops without iter_args / results.
      if (forOp.getNumResults() != 0)
        return;
      for (SplitLoopOp marker : forOp.getBody()->getOps<SplitLoopOp>()) {
        targets.push_back({forOp, marker});
        break; // one marker is enough to request a split
      }
    });
    for (auto [forOp, marker] : targets)
      splitLoop(forOp, marker);
  }
};

} // namespace
} // namespace mlir::iree_compiler::IREE::PolyCross
