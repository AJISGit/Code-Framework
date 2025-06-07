# Code Framework

Code Framework is a simple Lua framework I'm making just for fun.

I'm working on some simple documentation.

##

Has Raylib 5.5 and Lua 5.4.8 as dependencies.

It runs on windows 11. I haven't tested it on any other operating systems.

## Running

When you want to run your software, you do it by typing the command `codeframework <main .lua file>`

For example, if you have a file called "main.lua", you would run it by typing `codeframework main.lua`.

## Command-Line options

If you don't enter a file and just type `codeframework`, you will get this output:
```
CodeFramework: version beta 1.2, "The Unloading Update!"
No copyright at all!
some sort of license would go here.
This is a crappy lua framework made using raylib and C. Generic name on purpose

---------------------------------
You must have forgotten to enter a file, please do that

Or you could type "codeframework --info" for more info
```

And this output for `codeframework --info`:
```
Codeframework:
The new Lua framework in town.
You may know of it as, "That framework that only supports four keys",
Or, "That framework that doesn't test the metatables of userdata."

We, and by we I mean I, don't care about these major flaws. I think of them as features.

You want documentation? I'm working on it! You can see the progress at https://github.com/AJISGit/Code-Framework/wiki.
```
## Compiling From Source

There is a `build.bat` file for building with mingw -w64 on windows. It's assumed that `lua.h`, `lauxlib.h`, and `lualib.h` are all in your include path.

It also assumes that you link the lua library with it (eg. `liblua.a`).
