// Copyright 2026 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef IREE_DIALECT_POLYCROSS_OPS
#define IREE_DIALECT_POLYCROSS_OPS

include "iree/compiler/Dialect/PolyCross/IR/PolyCrossBase.td"
include "mlir/IR/OpBase.td"

//===----------------------------------------------------------------------===//
// polycross.split_loop
//===----------------------------------------------------------------------===//

def PolyCross_SplitLoopOp : PolyCross_Op<"split_loop"> {
  let summary = [{Marks the enclosing scf.for loop to be tiled/split.}];
  let description = [{
    A marker op placed inside an `scf.for` body. It requests the
    `-polycross-split-loop` pass to split the enclosing loop into an outer
    loop (stepping by `tile_size`) and an inner loop (walking inside each
    tile). The marker is removed by the pass.

    Example:
    ```mlir
    scf.for %i = %c0 to %c16 step %c1 {
      polycross.split_loop {tile_size = 4 : i64}
      // ... body using %i ...
    }
    ```
  }];

  let arguments = (ins I64Attr:$tile_size);

  let assemblyFormat = "attr-dict";
}

#endif // IREE_DIALECT_POLYCROSS_OPS
