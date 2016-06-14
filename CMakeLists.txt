cmake_minimum_required (VERSION 2.6)
project (backlite)

add_executable(backlite backlite.c)
install (TARGETS backlite DESTINATION bin)
