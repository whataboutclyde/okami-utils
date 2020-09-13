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
* archive.h:  Generic archive format.
* common.h:   Common structures and functions I should start using throughout.
* ect.h:      Information about fight spawns around the map.
* its.h:      Loot objects on the map, e.g. chests, clams, pots with guaranteed contents.
* model.h:    Level Model files (Need to add .md support) - submesh.h and mesh_division.h are used by this.
* objtbl.h:   Objects placed around the map.
* zone.h:     Hitboxes for dialogues and some other things. (Includes .SCA, .MEH and .SCI files.)

In many cases, there are portions of the file that are still unknown.

## Data

The data/ folder contains .yaml files I've mostly used for pretty-printing.

## GLTF

The gltf/ folder contains the following:

* generate_gltf:      Executable taking a .json config file to determine what gltf files to generate.
* gltf_config.json:   Probably don't try to mess with the format too much for now... Materials need to be in the same order as the MATERIAL_TYPE enum in generate_gltf.cpp, in the files section the type matches the FILE_TYPE enum in generate_gltf.cpp.
* gltf.h/.cpp:        Basically just using rapidjson to build the document.
* wrappers/           Someday I want to actually make real wrappers but admittedly for some of it I just gave up for now. I'd rather not have gltf code in the library.

## Tools

The tools/ folder contains the following tools:

* pack_akt:         Takes all .AK files in a folder to create an .AKT file.
* unpack_akt:       Unpacks an .AKT file into an .AKT_dir folder of .AK files.
* unpack_event:     This is ugly right now because I don't know how things associate yet but it separates the files with the right extensions.
* print_ans:        Print .ANS file in a human-readable manner.
* print_its:        Print .ITS file in a human-readable manner.
* parse_\*:          I mostly use these in order to print out all values for a variable and compare. Not sure they're useful to anyone else and will probably disappear.

Note: Currently print_* commands assume that the yaml files are in ./data/ from your current working directory. I'll fix this, honest.

## Build Environment

I've been doing everything with MSYS2.

```bash
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-make
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-fmt
pacman -S mingw-w64-x86_64-yaml-cpp
pacman -S mingw-w64-x86_64-rapidjson
```

I hope this is all that was required... However, cmake is meant to be able to make things for other build environments, so hopefully if you want to then you can.
