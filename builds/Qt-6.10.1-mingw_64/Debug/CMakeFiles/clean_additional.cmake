# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\smartHouse_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\smartHouse_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\smartHouse_gui_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\smartHouse_gui_autogen.dir\\ParseCache.txt"
  "smartHouse_autogen"
  "smartHouse_gui_autogen"
  )
endif()
