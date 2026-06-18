// RUN: iree-opt --polycross-generate-pattern %s | FileCheck %s

// unit-size=4, keep=2 (defaults). In each group of 4, the 2 largest-magnitude
// values are kept:
//   group0 [0.9, 0.8, 0.1, 0.2]  -> keep idx 0,1
//   group1 [0.05, 0.7, 0.3, 0.6] -> keep idx 5,7
// CHECK-LABEL: func.func @gen
// CHECK: polycross.mask = dense<[true, true, false, false, false, true, false, true]>
func.func @gen() -> tensor<8xf32> {
  %w = arith.constant dense<[0.9, 0.8, 0.1, 0.2, 0.05, 0.7, 0.3, 0.6]> : tensor<8xf32>
  return %w : tensor<8xf32>
}
