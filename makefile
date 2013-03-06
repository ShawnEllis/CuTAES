#
# Makefile for cuTAES
# COMP2404 Project
#
# Zack Dawson 100851846
#

OUT = bin/cuTAES.out
CFLAGS = -g
SRCDIR = src
OBJDIR = obj
OBJ:=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.cpp))
LIB = -lncurses -lform -lpanel

$(shell [ -d "bin" ] || mkdir -p bin)
$(shell [ -d "obj" ] || mkdir -p obj)

$(OUT): $(OBJ)
	g++ $^ $(LIB) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/*.h
	g++ $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OUT) $(OBJDIR)/*.o

