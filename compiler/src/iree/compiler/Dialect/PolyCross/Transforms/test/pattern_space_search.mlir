// RUN: iree-opt --polycross-pattern-space-search %s | FileCheck %s

// Scaffold pass: builds the candidate space internally, leaves IR unchanged.
// CHECK-LABEL: func.func @noop
// CHECK: return
func.func @noop(%x: index) -> index {
  return %x : index
}
