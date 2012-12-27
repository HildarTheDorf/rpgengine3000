# put common definitions in here
CC              = gcc
CXX             = g++
PRJCFLAGS       = -Wall -Werror -pedantic -std=c99 -g
PRJCXXFLAGS     = `pkg-config --cflags QtGui` -Wall -Werror -pedantic -std=c++11 -g
LD              = gcc
LDFLAGS1        = -lm
LDFLAGS2        = `pkg-config --libs QtGui`
RM              = rm
ECHO            = echo

.SILENT :
