all: forwarding non_forwarding

forwarding: src/forwarding.cpp src/main.cpp src/utils.cpp src/instructions.cpp
	g++ -o forwarding $^

non_forwarding: src/non_forwarding.cpp src/main.cpp src/utils.cpp src/instructions.cpp
	g++ -o non_forwarding $^

clean:
	rm -f forwarding non_forwarding


run: run_forwarding run_non_forwarding

run_forwarding:
	@echo "Running with forwarding:"
	@for file in $(wildcard ./inputfiles/*); do \
	    echo "Running $$file"; \
	    ./forwarding $$file; \
	done

run_non_forwarding:
	@echo "Running with non-forwarding:"
	@for file in $(wildcard ./inputfiles/*); do \
	    echo "Running $$file"; \
	    ./non_forwarding $$file; \
	done