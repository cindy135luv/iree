iree_add_all_subdirs()

iree_lit_test_suite(
  NAME
    lit
  SRCS
    "split_loop_roundtrip.mlir"
  TOOLS
    FileCheck
    iree-opt
)
