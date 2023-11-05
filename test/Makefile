BINARIES=bin

all: init unit_pure unit_verbose unit_dump stress fail_canary fail_integrity fail_hash fail

init:
	mkdir -p $(BINARIES)

unit_pure: unit.c
	gcc  -I../ unit.c -o $(BINARIES)/$@

unit_verbose: unit_verbose.c
	gcc -fsanitize=address -I../ -D VERBOSE $^ -o $(BINARIES)/$@

unit_dump: unit_verbose.c
	gcc -fsanitize=address -I../ -D VERBOSE -D USEDUMP $^ -o $(BINARIES)/$@

stress: stress.c
	gcc -I../ -D NDEBUG $^ -o $(BINARIES)/$@

fail_canary: fail_canary.c
	gcc -I../ -D VERBOSE -D USEDUMP -D CANARY -D NDEBUG $^ -o $(BINARIES)/$@

fail_integrity: fail_integrity.c
	gcc -I../ -D VERBOSE -D USEDUMP -D CANARY -D NDEBUG $^ -o $(BINARIES)/$@

fail_hash: fail_hash.c
	gcc -I../ -D VERBOSE -D CANARY -D USEDUMP -D NDEBUG $^ -o $(BINARIES)/$@

fail: fail.c
	gcc -fsanitize=address -I../ -D CANARY -D USEDUMP -D VERBOSE $^ -o $(BINARIES)/$@
