termtest:
	g++ termtest.cpp terminal.c -o termtest

o4test:
	g++ terminal.c geometry.cpp renderer.cpp objectRegistry.cpp o4test.cpp -o o4test

debug:
	g++ terminal.c geometry.cpp renderer.cpp objectRegistry.cpp o4test.cpp -o debugo4 -g

geotest:
	g++ geometry.cpp geotest.cpp -o geotest

o5test:
	g++ terminal.c geometry.cpp renderer.cpp objectRegistry.cpp ppmReader.cpp o5test.cpp -o o5test

ppmtest:
	g++ ppmReader.cpp renderer.cpp geometry.cpp terminal.c ppmtest.cpp -o ppmtest

o6test:
	g++ terminal.c geometry.cpp renderer.cpp objectRegistry.cpp ppmReader.cpp o6test.cpp -o o6test -g

optim:
	g++ terminal.c geometry.cpp renderer.cpp objectRegistry.cpp ppmReader.cpp o6test.cpp -o o6test -O3

pep:
	g++ terminal.c geometry.cpp renderer.cpp objectRegistry.cpp ppmReader.cpp pep.cpp -o pep -O3
