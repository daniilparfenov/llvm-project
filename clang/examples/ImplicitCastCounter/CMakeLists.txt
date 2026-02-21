add_llvm_library(ImplicitCastCounter MODULE ImplicitCastCounter.cpp PLUGIN_TOOL clang)

if(WIN32 OR CYGWIN)
  set(LLVM_LINK_COMPONENTS
    Support
  )
  clang_target_link_libraries(ImplicitCastCounter PRIVATE
    clangAST
    clangBasic
    clangFrontend
    )
endif()
