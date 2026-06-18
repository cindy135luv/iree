// Copyright 2026 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef IREE_DIALECT_POLYCROSS_BASE
#define IREE_DIALECT_POLYCROSS_BASE

include "mlir/IR/OpBase.td"

//===----------------------------------------------------------------------===//
// Dialect definition
//===----------------------------------------------------------------------===//

def PolyCross_Dialect : Dialect {
  let name = "polycross";
  let cppNamespace = "::mlir::iree_compiler::IREE::PolyCross";
  let summary = [{A dialect for compiler-side polymorphic N:M pruning.}];
  let description = [{
    The PolyCross dialect carries pruning intent (e.g. how a weight region is
    tiled and which units compete by importance) as explicit ops, so that
    PolyCross transformation passes can lower it down to plain scf/affine/memref
    loops for the IREE CPU/GPU backends.

    This is the first, intentionally small milestone: a single marker op
    (`polycross.split_loop`) plus one pass that tiles a marked `scf.for`.
  }];
}

//===----------------------------------------------------------------------===//
// Base op class
//===----------------------------------------------------------------------===//

class PolyCross_Op<string mnemonic, list<Trait> traits = []> :
    Op<PolyCross_Dialect, mnemonic, traits>;

#endif // IREE_DIALECT_POLYCROSS_BASE
