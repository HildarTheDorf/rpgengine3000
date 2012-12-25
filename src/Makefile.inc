# put common definitions in here
CC              = gcc
CXX             = g++
PRJCFLAGS       = -Wall -Werror -Wextra -pedantic -std=c99 -g
PRJCXXFLAGS     = `pkg-config --cflags QtGui` -Wall -Werror -Wextra -pedantic -std=c++11 -g
LD              = gcc
LDFLAGS         = `pkg-config --libs QtGui` -lm
RM              = rm
ECHO            = echo

.SILENT :
