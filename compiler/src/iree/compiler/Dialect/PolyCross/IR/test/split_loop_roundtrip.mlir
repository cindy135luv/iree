// RUN: iree-opt --split-input-file %s | iree-opt --split-input-file | FileCheck %s

// CHECK-LABEL: func.func @marker
func.func @marker(%lb: index, %ub: index, %step: index) {
  scf.for %i = %lb to %ub step %step {
    // CHECK: polycross.split_loop {tile_size = 4 : i64}
    polycross.split_loop {tile_size = 4 : i64}
  }
  return
}
