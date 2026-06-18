################################################################################
# PolyCross dialect IR library (hand-written; project builds via CMake).        #
################################################################################

iree_add_all_subdirs()

iree_cc_library(
  NAME
    IR
  HDRS
    "PolyCrossDialect.h"
    "PolyCrossDialect.h.inc"
    "PolyCrossOps.h"
    "PolyCrossOps.h.inc"
  SRCS
    "PolyCrossDialect.cpp"
    "PolyCrossDialect.cpp.inc"
    "PolyCrossOps.cpp"
    "PolyCrossOps.cpp.inc"
  DEPS
    ::PolyCrossOpsGen
    LLVMSupport
    MLIRIR
    MLIRSupport
  PUBLIC
)

iree_tablegen_library(
  NAME
    PolyCrossOpsGen
  TD_FILE
    "PolyCrossOps.td"
  OUTS
    --gen-op-decls PolyCrossOps.h.inc
    --gen-op-defs PolyCrossOps.cpp.inc
    --dialect=polycross --gen-dialect-decls PolyCrossDialect.h.inc
    --dialect=polycross --gen-dialect-defs PolyCrossDialect.cpp.inc
)
