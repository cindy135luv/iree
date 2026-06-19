// Copyright 2026 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef IREE_COMPILER_DIALECT_POLYCROSS_IR_POLYCROSSOPS_H_
#define IREE_COMPILER_DIALECT_POLYCROSS_IR_POLYCROSSOPS_H_

#include "iree/compiler/Dialect/PolyCross/IR/PolyCrossDialect.h"
#include "mlir/Bytecode/BytecodeOpInterface.h"
#include "mlir/IR/Attributes.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/IR/OpImplementation.h"

// clang-format off
#define GET_OP_CLASSES
#include "iree/compiler/Dialect/PolyCross/IR/PolyCrossOps.h.inc" // IWYU pragma: export
#undef GET_OP_CLASSES
// clang-format on

#endif // IREE_COMPILER_DIALECT_POLYCROSS_IR_POLYCROSSOPS_H_
