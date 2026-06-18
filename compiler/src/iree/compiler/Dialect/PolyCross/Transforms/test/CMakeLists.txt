iree_add_all_subdirs()

iree_lit_test_suite(
  NAME
    lit
  SRCS
    "apply_condense.mlir"
    "auto_tune_pattern.mlir"
    "generate_pattern.mlir"
    "pattern_space_search.mlir"
    "sparse_matmul_lowering.mlir"
    "split_loop.mlir"
  TOOLS
    FileCheck
    iree-opt
)
