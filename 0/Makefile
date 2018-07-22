# for g++ on linux platforms

# ===============================================================

SRCS = main.cpp \
		matrix.cpp \
		image.cpp \
		ifs.cpp

# ===============================================================

LIBS = -lm 
CC = g++ -std=c++11
INCLUDE = -I/usr/include 
EXE = ifs
OBJS = $(SRCS:.cpp=.o)

# ===============================================================
# targets

.PHONY: all depend clean

all: depend $(EXE) 

depend:
	$(CC) -E -M $(INCLUDE) $(SRCS) > Makefile.depend

clean: 
	rm -f *~ *bak *.o  $(EXE) Makefile.depend

test: all
	./ifs -input sierpinski_triangle.txt -points 10000 -iters 0 -size 200 -output sierpinski_triangle_0.tga
	./ifs -input sierpinski_triangle.txt -points 10000 -iters 1 -size 200 -output sierpinski_triangle_1.tga
	./ifs -input sierpinski_triangle.txt -points 10000 -iters 2 -size 200 -output sierpinski_triangle_2.tga
	./ifs -input sierpinski_triangle.txt -points 10000 -iters 3 -size 200 -output sierpinski_triangle_3.tga
	./ifs -input sierpinski_triangle.txt -points 10000 -iters 4 -size 200 -output sierpinski_triangle_4.tga
	./ifs -input sierpinski_triangle.txt -points 10000 -iters 30 -size 200 -output sierpinski_triangle.tga
	./ifs -input fern.txt -points 50000 -iters 30 -size 400 -output fern.tga

# ===============================================================
# compilation rules

$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

.cpp.o:
	$(CC) $< -c -o $@

# ===============================================================

-include Makefile.depend

# DO NOT DELETE