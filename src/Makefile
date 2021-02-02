test: Vec3D.o Line.o Ray.o test.cpp
	g++ -std=c++11 Vec3D.o Line.o Ray.o test.cpp -o test.out

vec: Vec3D.hpp Vec3D.cpp
	g++ -std=c++11 -c Vec3D.cpp

line: Line.hpp Line.cpp
	g++ -std=c++11 -c Line.cpp

ray: Ray.hpp Ray.cpp
	g++ -std=c++11 -c Ray.cpp

clean:
	rm *.o *.out

