all:
	g++ ./src/main.cc ./src/spinspinner.cc ./src/spin.cc ./src/spinposition.cc -o spinspinner
debug:
	g++ -g ./src/main.cc ./src/spinspinner.cc ./src/spin.cc ./src/spinposition.cc -o spinspinner
clean:
	rm -f ./spinspinner
