// Copyright 2026 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "iree/compiler/Dialect/PolyCross/Transforms/Passes.h"

namespace mlir::iree_compiler::IREE::PolyCross {

#define GEN_PASS_DEF_AUTOTUNEPATTERNPASS
#include "iree/compiler/Dialect/PolyCross/Transforms/Passes.h.inc"

namespace {

// SCAFFOLD: no-op for now.
//
// Next milestone: drive PatternSpaceSearch + a cost model to choose the best
// pruning pattern for each weight, then apply GeneratePattern / ApplyCondense
// with the chosen (unit-size, keep) configuration.
struct AutoTunePatternPass
    : impl::AutoTunePatternPassBase<AutoTunePatternPass> {
  using Base::Base;

  void runOnOperation() override {
    // TODO(polycross): search + cost model + apply best pattern.
  }
};

} // namespace
} // namespace mlir::iree_compiler::IREE::PolyCross
