message(STATUS "Available architectures (default = X64):\n")

message("   ARCH=          No specific architecture (disable some features).")
message("   ARCH=AVR       Atmel AVR ATMega128 8-bit architecture.")
message("   ARCH=MSP       TI MSP430 16-bit architecture.")
message("   ARCH=ARM       ARM 32-bit architecture.")
message("   ARCH=X86       Intel x86-compatible 32-bit architecture.")
message("   ARCH=X64       AMD x86_64-compatible 64-bit architecture.\n")

message(STATUS "Available word sizes (default = 64):\n")

message("   WORD=8         Build a 8-bit library.")
message("   WORD=16        Build a 16-bit library.")
message("   WORD=32        Build a 32-bit library.")
message("   WORD=64        Build a 64-bit library.\n")

message(STATUS "Byte boundary to align digit vectors (default = 1):\n")

message("   ALIGN=1        Do not align digit vectors.")
message("   ALIGN=2        Align digit vectors into 16-bit boundaries.")
message("   ALIGN=8        Align digit vectors into 64-bit boundaries.")
message("   ALIGN=16       Align digit vectors into 128-bit boundaries.\n")

# Memory layout.
if(NOT ALIGN)
	set(ALIGN 1)
endif(NOT ALIGN)
set(ALIGN ${ALIGN} CACHE INTEGER "Boundary to align digit vectors")
