// RUN: iree-opt --polycross-sparse-matmul-lowering %s | FileCheck %s

// Scaffold pass: the matmul is left unchanged for now.
// CHECK-LABEL: func.func @mm
// CHECK: linalg.matmul
func.func @mm(%a: tensor<4x4xf32>, %b: tensor<4x4xf32>, %c: tensor<4x4xf32>)
    -> tensor<4x4xf32> {
  %0 = linalg.matmul
      ins(%a, %b : tensor<4x4xf32>, tensor<4x4xf32>)
      outs(%c : tensor<4x4xf32>) -> tensor<4x4xf32>
  return %0 : tensor<4x4xf32>
}
