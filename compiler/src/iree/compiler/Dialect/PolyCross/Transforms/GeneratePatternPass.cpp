// Copyright 2026 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "iree/compiler/Dialect/PolyCross/Transforms/Passes.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/IR/BuiltinTypes.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"
#include <cmath>

namespace mlir::iree_compiler::IREE::PolyCross {

#define GEN_PASS_DEF_GENERATEPATTERNPASS
#include "iree/compiler/Dialect/PolyCross/Transforms/Passes.h.inc"

namespace {

// Returns the keep-mask for one flat array of weights, using the polymorphic
// N:M rule: split into units of `unit`, keep the `keep` largest-magnitude
// values inside each unit.
static SmallVector<bool> magnitudePruneMask(ArrayRef<float> vals, int64_t unit,
                                            int64_t keep) {
  int64_t n = vals.size();
  SmallVector<bool> mask(n, false);
  for (int64_t base = 0; base < n; base += unit) {
    int64_t end = std::min(base + unit, n);
    // Order this unit's indices by descending magnitude (stable for ties).
    SmallVector<int64_t> idx;
    for (int64_t i = base; i < end; ++i)
      idx.push_back(i);
    llvm::stable_sort(idx, [&](int64_t a, int64_t b) {
      return std::abs(vals[a]) > std::abs(vals[b]);
    });
    for (int64_t j = 0; j < keep && j < static_cast<int64_t>(idx.size()); ++j)
      mask[idx[j]] = true;
  }
  return mask;
}

struct GeneratePatternPass
    : impl::GeneratePatternPassBase<GeneratePatternPass> {
  using Base::Base;

  void runOnOperation() override {
    int64_t unit = unitSize > 0 ? unitSize : 1;
    int64_t k = keep >= 0 ? keep : 0;

    getOperation()->walk([&](arith::ConstantOp constOp) {
      if (constOp->hasAttr("polycross.mask"))
        return; // already generated
      auto dense = dyn_cast<DenseElementsAttr>(constOp.getValue());
      if (!dense)
        return;
      auto type = dyn_cast<RankedTensorType>(dense.getType());
      if (!type || !type.getElementType().isF32())
        return;

      SmallVector<float> vals(dense.getValues<float>().begin(),
                              dense.getValues<float>().end());
      SmallVector<bool> mask = magnitudePruneMask(vals, unit, k);

      auto maskType = RankedTensorType::get(
          type.getShape(), IntegerType::get(type.getContext(), 1));
      auto maskAttr = DenseElementsAttr::get(maskType, ArrayRef<bool>(mask));
      constOp->setAttr("polycross.mask", maskAttr);
    });
  }
};

} // namespace
} // namespace mlir::iree_compiler::IREE::PolyCross
