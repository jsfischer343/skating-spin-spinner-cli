version = 1.0
all:
	g++ ./src/main.cc ./src/spinspinner.cc ./src/spin.cc ./src/spinposition.cc ./src/spinsegment.cc -o spinspinner
debug:
	g++ -g ./src/main.cc ./src/spinspinner.cc ./src/spin.cc ./src/spinposition.cc ./src/spinsegment.cc -o spinspinner
warnings:
	g++ -g -Wall -Wextra -Wpedantic ./src/main.cc ./src/spinspinner.cc ./src/spin.cc ./src/spinposition.cc ./src/spinsegment.cc -o spinspinner
static:
	g++ ./src/main.cc ./src/spinspinner.cc ./src/spin.cc ./src/spinposition.cc ./src/spinsegment.cc -o spinspinner-linux-x86_64-v$(version).AppImage -static -s
windows32:
	i686-w64-mingw32-g++ ./src/main.cc ./src/spinspinner.cc ./src/spin.cc ./src/spinposition.cc ./src/spinsegment.cc -o spinspinner-w32-v$(version).exe -static -s
windows64:
	x86_64-w64-mingw32-g++ ./src/main.cc ./src/spinspinner.cc ./src/spin.cc ./src/spinposition.cc ./src/spinsegment.cc -o spinspinner-w64-v$(version).exe -static -s
clean:
	rm -f ./spinspinner
