// RUN: iree-opt --polycross-auto-tune-pattern %s | FileCheck %s

// Scaffold pass: no-op for now.
// CHECK-LABEL: func.func @noop
// CHECK: return
func.func @noop(%x: index) -> index {
  return %x : index
}
