src_files = src\funcs.c src\main.c
executable_name = codeframework.exe

build:
	gcc $(src_files) -o $(executable_name) -Wno-missing-braces -static -I ./include/ -L ./lib/ -lraylib -llua -lopengl32 -lgdi32 -lwinmm