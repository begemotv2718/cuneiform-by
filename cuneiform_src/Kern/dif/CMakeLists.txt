include_directories(BEFORE src)

add_library(dif32 ${LIBTYPE}
src/dif.c
src/diffr.c
src/diffrb.c
src/sticchar.c
src/sticdif.c
src/stictool.c
)
set_property(TARGET dif32 PROPERTY COMPILE_DEFINITIONS __DIF__)

library_hook(dif32)
