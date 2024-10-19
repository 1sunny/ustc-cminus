add_library(
    opt_lib STATIC
    Dominators.cpp
    Mem2Reg.cpp
    FuncInfo.cpp
    DeadCode.cpp
    CFGAnalyse.cpp
    LoopInvariant.cpp
)
