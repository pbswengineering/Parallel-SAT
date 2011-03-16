project(parallel-sat CXX)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_COMPILER mpicxx)
set(CMAKE_CXX_FLAGS "-pipe -O3 -Wall -wd869 -wd981 -DMPICH_IGNORE_CXX_SEEK")

add_executable(parallel-sat
	clause.cpp
	node.cpp
	literal.cpp
	communications.cpp
	dpll-parallel.cpp
	main-parallel.cpp
)

add_executable(serial-sat
	clause.cpp
	node.cpp
	literal.cpp
	dpll-serial.cpp
	main-serial.cpp
)
