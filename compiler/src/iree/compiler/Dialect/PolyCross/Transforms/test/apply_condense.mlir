// RUN: iree-opt --polycross-generate-pattern --polycross-apply-condense %s | FileCheck %s

// After generate (unit 4, keep 2) then condense, the pruned values become 0.0
// and the mask attribute is consumed.
// CHECK-LABEL: func.func @condense
// CHECK: arith.constant
// CHECK-SAME: 0.000000e+00
// CHECK-NOT: polycross.mask
func.func @condense() -> tensor<8xf32> {
  %w = arith.constant dense<[0.9, 0.8, 0.1, 0.2, 0.05, 0.7, 0.3, 0.6]> : tensor<8xf32>
  return %w : tensor<8xf32>
}
