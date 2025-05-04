# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\PDF_to_AudioBook_App_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\PDF_to_AudioBook_App_autogen.dir\\ParseCache.txt"
  "PDF_to_AudioBook_App_autogen"
  )
endif()
