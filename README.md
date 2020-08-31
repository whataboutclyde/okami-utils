# okami-utils
Utilities for Okami HD file manipulation.

Currently supported: Okami HD PC

At the moment, everything assumes you are working with unencrypted files. Okami files are encrypted with Blowfish. There are plans to add an encryption tool at some point but I haven't decided if they'll be integrated into tools.

Note that this is a very very early version and all apis could change.

DO NOT ask me for game files. I will not give them out.

## okami-utils.a

Everything uses the OKAMI_UTILS namespace.

The library currently supports the following file types:
* ak.h/akt.h: Collision files. The AKT is an archive of AK files.
* ans.h:      Information about animals on the map.
* its.h:      Loot objects on the map, e.g. chests, clams, pots with guaranteed contents.
* objtbl.h:   Objects placed around the map.
* sca.h:      Hitboxes for dialogues and some other things. (Includes .SCA, .MEH and .SCI files.)
* scr.h:      Level Model files

In many cases, there are portions of the file that are still unknown.

## Data

The data/ folder contains .yaml files I've mostly used for pretty-printing.

## Tools

The tools/ folder contains the following tools:

convert_collison: Converts an .AK or .AKT file into a Wavefront Object (.obj) files that you can load in blender or wherever else.
print_ans:        Print .ANS file in a human-readable manner.
print_its:        Print .ITS file in a human-readable manner.

Note: Currently print_* commands assume that the yaml files are in ./data/ from your current working directory. I'll fix this, honest.

## Build Environment

I've been doing everything with MSYS2.

```bash
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-make
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-fmt
pacman -S mingw-w64-x86_64-yaml-cpp
```

I hope this is all that was required... However, cmake is meant to be able to make things for other build environments, so hopefully if you want to then you can.