# cmakex - an extension for the `cmake` command

## Contents

- Overview
- Building and Installing
- Tutorial
- Reference

## Overview

`cmakex` is a command-line tool, which extends the functionality of the `cmake`
command. It adds the following interdependent features:

1. Execute multiple `cmake` steps with one command. For example, configure,
   install, test a project for debug & release configs with one, concise
   command, without changing directories.
2. Define common cmake option groups (presets) in a `YAML` file. Such a preset
   can be a set of cmake-generator settings, platform and toolchain options. The
   preset can be added to the current build options with a single switch.
3. Download (git-clone), configure, install a package with a single command
   line. It's the command-line version of the CMake ExternalProject module.
4. Describe the dependencies of your project in a simple cmake-file with
   `ExternalProject_Add`-like parameters. The dependencies will be automatically
   downloaded (git-cloned), configured, built, installed.
5. (to be implemented) Install a local or remote build-server which builds,
   caches and provides installable packages, automatically managing the full
   dependency graph, keeping track of versions and build settings.

And all this is

- written in C++ and provided in a self-contained, easy-to-install project.
- as lightweight as it gets: don't need to abandon your current workflow,
  use only as many features as you need, easy to opt-out.
- open-source, permissive license
- supports a wide range of platforms, UNIX/Mac/Windows

## Building and Installing

Prerequisites: CMake version 3.1 (this could be lowered on demand)

All other dependencies are cloned and built within this project.
For a quick release build:

    ./configure.sh && ./make.sh

## Tutorial

### Multiple CMake Commands
### CMake Presets
### Build Script with Dependencies
### Local Build Server
### Remote Build Server

## Reference

### Multiple CMake Commands
### CMake Option Groups
### Build Script with Dependencies
### Local Build Server
### Remote Build Server
