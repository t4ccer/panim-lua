lua-5.4.7/src/liblua.a:
	env -C lua-5.4.7/src make liblua.a

panim/build/panim:
	env -C panim cc -o nob nob.c
	env -C panim ./nob -f

build/libpluglua.so: src/harness.c src/plug.lua lua-5.4.7/src/liblua.a
	mkdir -p build
	cc -o ./build/libpluglua.so -Wall -Wextra -ggdb -I./panim/raylib/raylib-5.0_linux_amd64/include -I./lua-5.4.7/src -I./panim/panim/ -I./src -fPIC -shared -Wl,--no-undefined -Wl,-rpath=./panim/raylib/raylib-5.0_linux_amd64/lib/ -Wl,-rpath=./panim/panim/ -L./panim/raylib/raylib-5.0_linux_amd64/lib -l:libraylib.so -lm -ldl -lpthread ./src/harness.c ./lua-5.4.7/src/liblua.a

.PHONY: all
all: lua-5.4.7/src/liblua.a panim/build/panim build/libpluglua.so
