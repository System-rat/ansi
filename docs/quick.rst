.. _quick:

Quickstart
==========

This library is developed to be small, dependency-free, and easy to install/use. Simply clone the project
with `git` and install using `CMake`.

Installation
############

The library can be used as a CMake submodule by including it as a
sub-project:

.. code-block:: cmake

    add_subdirectory(ansi)

    target_link_libraries(<target> ansi)

The library can also be installed system-wide:

.. code-block:: shell

    $ cmake --config Release -B build -S . -G Ninja -DINSTALL_ANSI=ON -DBUILD_TESTING=OFF
    $ cd build
    $ sudo ninja install

There are a few options within the CMakeLists.txt file that can be configured:

 - `INSTALL_ANSI` adds an install step to the build that installs the ansi lib system-wide
 - `BUILD_TESTING` enables tests for the library, probably not something you want enabled if
   you're using the library as a sub-project
 - By default the library is built and linked with sanitizers, switching to the `Release` configuration
   does not build or link with sanitizers

Using the library
#################

The library contains functions and classes for different codding styles, the `ansi::manip` namespace
contains manipulator functions to be used with streaming outputs the same way `std::endl` and co.
are used. The `ansi::styling` namespace is for a more "stylesheet" oriented workflow, styles can be
instantiated and the text is pre-computed and can then be applied and reused. And for more `printf`
oriented workflows the `ansi::format_str` (and subsequently the `ANSI` macro) can be used to create
a format string from a small custom language.

An example of all 3 styles:

.. code-block:: c++

    #include <Ansi>
    #include <iostream>
    #include <cstdio>

    int main() {
        std::cout << ansi::manip::bold << ansi::manip::color(ansi::Red) << "Red text" << std::endl;

        ansi::styling::Style style1(ansi::StyleColor(240, 150, 30), { ansi::Bold, ansi::Underline });

        std::cout << style1.apply("Text") << std::endl;

        printf(ANSI("#[Underline, Strikethrough, 150:30:240]Text with a #$\n"), "variable");
    }

For more details on the various functions and types, see :ref:`api` 

