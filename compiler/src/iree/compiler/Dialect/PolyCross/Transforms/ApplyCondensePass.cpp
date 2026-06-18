// Copyright 2026 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "iree/compiler/Dialect/PolyCross/Transforms/Passes.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/IR/BuiltinTypes.h"
#include "llvm/ADT/SmallVector.h"

namespace mlir::iree_compiler::IREE::PolyCross {

#define GEN_PASS_DEF_APPLYCONDENSEPASS
#include "iree/compiler/Dialect/PolyCross/Transforms/Passes.h.inc"

namespace {

struct ApplyCondensePass : impl::ApplyCondensePassBase<ApplyCondensePass> {
  using Base::Base;

  void runOnOperation() override {
    getOperation()->walk([&](arith::ConstantOp constOp) {
      auto maskAttr =
          dyn_cast_or_null<DenseElementsAttr>(constOp->getAttr("polycross.mask"));
      if (!maskAttr)
        return;
      auto dense = dyn_cast<DenseElementsAttr>(constOp.getValue());
      if (!dense)
        return;
      auto type = dyn_cast<RankedTensorType>(dense.getType());
      if (!type || !type.getElementType().isF32())
        return;

      SmallVector<float> vals(dense.getValues<float>().begin(),
                              dense.getValues<float>().end());
      SmallVector<bool> mask(maskAttr.getValues<bool>().begin(),
                             maskAttr.getValues<bool>().end());
      if (mask.size() != vals.size())
        return;

      // Zero out the pruned (mask == false) values.
      for (size_t i = 0; i < vals.size(); ++i)
        if (!mask[i])
          vals[i] = 0.0f;

      auto newDense = DenseElementsAttr::get(type, ArrayRef<float>(vals));
      constOp.setValueAttr(newDense);
      constOp->removeAttr("polycross.mask");
    });
  }
};

} // namespace
} // namespace mlir::iree_compiler::IREE::PolyCross
