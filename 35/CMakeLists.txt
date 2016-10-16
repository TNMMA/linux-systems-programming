#================================================================================
# @fileOverview Top level build file
# @author Ashish Bajaj, bajaj.ashish@gmail.com
# @copyright Copyright (c) 2016 Ashish Bajaj
# @license To any person (the "Recipient") obtaining a copy of this software and        
# associated documentation files (the "Software"):                             
#                                                                              
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#================================================================================
# Set compilation flags
set(CMAKE_C_FLAGS "-Wall -O2 -pedantic")

# The following file GLOB command will search for all files within the current
# directory that match the specified expressions (*.c and *.h), and assign the
# resulting files to the variable SRCS. SRCS is a list of all C source and
# header files in the current directory.
file(GLOB SRCS *.c *.h)

# Add final executable for this folder
add_executable(prog35 prog35.c)
