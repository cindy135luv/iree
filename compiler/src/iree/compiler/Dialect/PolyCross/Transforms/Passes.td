// Copyright 2026 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef IREE_DIALECT_POLYCROSS_PASSES
#define IREE_DIALECT_POLYCROSS_PASSES

include "mlir/Pass/PassBase.td"

def SplitLoopPass : Pass<"polycross-split-loop"> {
  let summary = "Tile scf.for loops marked with polycross.split_loop.";
  let description = [{
    Finds every `scf.for` whose body contains a `polycross.split_loop` marker
    and splits it into an outer loop (stepping by `tile_size`) and an inner
    loop (walking within each tile). A `min` guards the inner upper bound so
    the loop is correct even when the trip count is not divisible by the tile
    size. The marker op is removed.

    v1 only handles simple loops: step 1 and no iter_args / results.
  }];
  let dependentDialects = [
    "mlir::scf::SCFDialect",
    "mlir::arith::ArithDialect",
  ];
}

//===----------------------------------------------------------------------===//
// Step 2: GeneratePattern  (REAL logic)
//===----------------------------------------------------------------------===//

def GeneratePatternPass : Pass<"polycross-generate-pattern"> {
  let summary = "Generate a magnitude-based N:M pruning mask for f32 constants.";
  let description = [{
    For every `arith.constant` holding an `f32` tensor, split the flattened
    values into Sorted Units of `unit-size`, keep the `keep` largest-magnitude
    values per unit, and attach the resulting boolean mask as a
    `polycross.mask` attribute. This is the polymorphic N:M idea in its
    simplest form: a whole unit competes by magnitude before pruning.
  }];
  let options = [
    Option<"unitSize", "unit-size", "int64_t", /*default=*/"4",
           "Size of each Sorted Unit (the competition region).">,
    Option<"keep", "keep", "int64_t", /*default=*/"2",
           "How many values to keep per unit (by magnitude).">,
  ];
  let dependentDialects = ["mlir::arith::ArithDialect"];
}

//===----------------------------------------------------------------------===//
// Step 3: ApplyCondense  (REAL logic)
//===----------------------------------------------------------------------===//

def ApplyCondensePass : Pass<"polycross-apply-condense"> {
  let summary = "Apply a polycross.mask to its constant, zeroing pruned values.";
  let description = [{
    For every `arith.constant` carrying a `polycross.mask` attribute (produced
    by `-polycross-generate-pattern`), zero out the values whose mask bit is
    false and drop the mask attribute. This realizes the "remove unimportant
    values" step. A future version can additionally *condense* the survivors
    into a compact representation.
  }];
  let dependentDialects = ["mlir::arith::ArithDialect"];
}

//===----------------------------------------------------------------------===//
// Step 4: SparseMatmulLowering  (SCAFFOLD + TODO)
//===----------------------------------------------------------------------===//

def SparseMatmulLoweringPass : Pass<"polycross-sparse-matmul-lowering"> {
  let summary = "Lower a pruned linalg.matmul into explicit loops (scaffold).";
  let description = [{
    Scaffold only. The intended behavior is to lower a matmul whose weights
    were pruned into `scf`/`affine` loops that skip the pruned (zero) entries.
    For now this pass registers and walks `linalg.matmul` ops without changing
    them, so the structure exists for the next milestone.
  }];
  let dependentDialects = [
    "mlir::scf::SCFDialect",
    "mlir::arith::ArithDialect",
    "mlir::linalg::LinalgDialect",
  ];
}

//===----------------------------------------------------------------------===//
// Step 5: PatternSpaceSearch  (SCAFFOLD + TODO)
//===----------------------------------------------------------------------===//

def PatternSpaceSearchPass : Pass<"polycross-pattern-space-search"> {
  let summary = "Enumerate candidate (unit-size, keep) patterns (scaffold).";
  let description = [{
    Scaffold only. The intended behavior is to enumerate the pruning pattern
    space and score each candidate with a cost/accuracy model. For now it
    builds the candidate list but does not transform the IR.
  }];
}

//===----------------------------------------------------------------------===//
// Step 6: AutoTunePattern  (SCAFFOLD + TODO)
//===----------------------------------------------------------------------===//

def AutoTunePatternPass : Pass<"polycross-auto-tune-pattern"> {
  let summary = "Pick the best pruning pattern per weight (scaffold).";
  let description = [{
    Scaffold only. The intended behavior is to drive PatternSpaceSearch plus a
    cost model to choose the best pattern, then apply GeneratePattern /
    ApplyCondense with the chosen config. No-op for now.
  }];
}

#endif // IREE_DIALECT_POLYCROSS_PASSES
