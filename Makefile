.PHONY: all cpp rust clean run-cpp run-rust benchmark benchmark-helloworld benchmark-addnum benchmark-print0to99

# Targets
all: cpp rust

cpp: brainduck.cpp
	g++ -std=c++17 -O3 -march=native -flto -ffast-math brainduck.cpp -o brainduck-cpp

rust: brainduck.rs
	rustc -C opt-level=3 -C lto=fat -C codegen-units=1 -C target-cpu=native brainduck.rs -o brainduck-rs

run-cpp: cpp
	./brainduck-cpp helloworld.bf

run-rust: rust
	./brainduck-rs helloworld.bf

run: run-cpp run-rust

benchmark: benchmark-helloworld benchmark-addnum benchmark-print0to99

benchmark-helloworld: all
	@echo "=== Benchmarking helloworld.bf ==="
	hyperfine --shell=none --warmup 10 './brainduck-cpp helloworld.bf' './brainduck-rs helloworld.bf'

benchmark-addnum: all
	@echo "=== Benchmarking addnum.bf ==="
	hyperfine --shell=none --warmup 10 './brainduck-cpp addnum.bf' './brainduck-rs addnum.bf'

benchmark-print0to99: all
	@echo "=== Benchmarking print0to99.bf ==="
	hyperfine --shell=none --warmup 10 './brainduck-cpp print0to99.bf' './brainduck-rs print0to99.bf'

clean:
	rm -f brainduck-cpp brainduck-rs

.DEFAULT_GOAL := all
