# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\fimozzz_antivir_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\fimozzz_antivir_autogen.dir\\ParseCache.txt"
  "fimozzz_antivir_autogen"
  )
endif()
