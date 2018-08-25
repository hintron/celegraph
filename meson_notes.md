Decided on Meson because it looks relatively simple and fast.
Better than Gradle because it doesn't require Java.

This blog has good reasoning as to why Meson is the way to go:
https://carlosvin.github.io/es/posts/choosing-modern-cpp-stack/

Also, replace Catch with Doctest.

Plus, Meson does Rust!

Why Make can't be made faster (and why Ninja is faster)

Notes for building with Meson:
===================================
Install the Meson build syntax package for Sublime.
Follow this guide: http://mesonbuild.com/Quick-guide.html

First, get into a linux environment:
    bash
    cd /mnt/c/Users/micro/me/code/celegraph

Install meson and ninja:
    sudo apt-get install python3 ninja-build
    pip3 install meson

Port makefile to meson.build

I needed to delete the build/ folder and rerun `meson build/` in order for my addition of `default_options: ['cpp_std=c++11']` to work under `project()` in top level meson.build file.

All variables need to be defined before running subdir() if you want to use them.

