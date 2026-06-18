################################################################################
# PolyCross dialect Transforms library (hand-written).                          #
################################################################################

iree_add_all_subdirs()

iree_cc_library(
  NAME
    Transforms
  HDRS
    "Passes.h"
    "Passes.h.inc"
  SRCS
    "ApplyCondensePass.cpp"
    "AutoTunePatternPass.cpp"
    "GeneratePatternPass.cpp"
    "Passes.cpp"
    "PatternSpaceSearchPass.cpp"
    "SparseMatmulLoweringPass.cpp"
    "SplitLoopPass.cpp"
  DEPS
    ::PassesIncGen
    LLVMSupport
    MLIRArithDialect
    MLIRIR
    MLIRLinalgDialect
    MLIRPass
    MLIRSCFDialect
    MLIRSupport
    MLIRTransforms
    iree::compiler::Dialect::PolyCross::IR
  PUBLIC
)

iree_tablegen_library(
  NAME
    PassesIncGen
  TD_FILE
    "Passes.td"
  OUTS
    --gen-pass-decls Passes.h.inc
)
