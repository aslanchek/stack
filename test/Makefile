all: unit_pure unit_verbose unit_dump stress fail_canary

unit_pure: unit.c
	gcc -I../ unit.c -o $@

unit_verbose: unit_verbose.c
	gcc -I../ -D VERBOSE $^ -o $@

unit_dump: unit_verbose.c
	gcc -I../ -D VERBOSE -D USEDUMP $^ -o $@

stress: stress.c
	gcc -I../ -D NDEBUG $^ -o $@

fail_canary: fail_canary.c
	gcc -I../ -D VERBOSE -D CANARY -D NDEBUG $^ -o $@

fail_integrity: fail_integrity.c
	gcc -I../ -D VERBOSE -D CANARY -D NDEBUG $^ -o $@
