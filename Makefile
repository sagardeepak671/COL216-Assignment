all: forwarding non_forwarding

forwarding: src/forwarding.cpp src/main.cpp src/utils.cpp src/instructions.cpp
	g++ -o forwarding $^

non_forwarding: src/non_forwarding.cpp src/main.cpp src/utils.cpp src/instructions.cpp
	g++ -o non_forwarding $^

clean:
	rm -f forwarding non_forwarding
