# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Formul_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Formul_autogen.dir\\ParseCache.txt"
  "Formul_autogen"
  )
endif()
