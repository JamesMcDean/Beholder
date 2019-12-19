project(Exceptions)

add_library(Exceptions connectionexceptions.hpp)

set_target_properties(Exceptions PROPERTIES LINKER_LANGUAGE CXX)