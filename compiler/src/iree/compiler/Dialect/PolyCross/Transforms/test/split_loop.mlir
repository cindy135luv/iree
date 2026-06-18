// RUN: iree-opt --polycross-split-loop --split-input-file %s | FileCheck %s

// A simple 0..16 step 1 loop, tiled by 4 (divides evenly).
// CHECK-LABEL: func.func @split_static
func.func @split_static() {
  %c0 = arith.constant 0 : index
  %c16 = arith.constant 16 : index
  %c1 = arith.constant 1 : index
  // CHECK-DAG: %[[C16:.+]] = arith.constant 16 : index
  // CHECK-DAG: %[[C1:.+]] = arith.constant 1 : index
  // CHECK-DAG: %[[TILE:.+]] = arith.constant 4 : index
  // CHECK: scf.for %[[IO:.+]] = %{{.+}} to %[[C16]] step %[[TILE]]
  // CHECK:   %[[T:.+]] = arith.addi %[[IO]], %[[TILE]]
  // CHECK:   %[[UB:.+]] = arith.minui %[[C16]], %[[T]]
  // CHECK:   scf.for %[[II:.+]] = %[[IO]] to %[[UB]] step %[[C1]]
  // CHECK:     arith.addi %[[II]], %[[II]]
  // CHECK-NOT: polycross.split_loop
  scf.for %i = %c0 to %c16 step %c1 {
    %x = arith.addi %i, %i : index
    polycross.split_loop {tile_size = 4 : i64}
  }
  return
}

// -----

// An unmarked loop must be left untouched.
// CHECK-LABEL: func.func @no_marker
func.func @no_marker(%lb: index, %ub: index, %step: index) {
  // CHECK: scf.for
  // CHECK-NOT: scf.for
  scf.for %i = %lb to %ub step %step {
    %x = arith.addi %i, %i : index
  }
  return
}
