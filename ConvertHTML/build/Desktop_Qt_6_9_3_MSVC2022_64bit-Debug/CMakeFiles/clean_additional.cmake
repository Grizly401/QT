# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\ConvertHTML_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ConvertHTML_autogen.dir\\ParseCache.txt"
  "ConvertHTML_autogen"
  )
endif()
