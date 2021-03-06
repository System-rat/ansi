# ANSI Compatible text styling

![License badge](https://img.shields.io/github/license/System-rat/ansi.svg?style=for-the-badge)
![Issues badge](https://img.shields.io/github/issues/System-rat/ansi.svg?style=for-the-badge)
![Pull requests badge](https://img.shields.io/github/issues-pr/System-rat/ansi.svg?style=for-the-badge)
![Docs build badge](https://img.shields.io/github/workflow/status/System-rat/ansi/Docs/master?label=docs&style=for-the-badge)
![Tests build badge](https://img.shields.io/github/workflow/status/System-rat/ansi/Tests/master?label=tests&style=for-the-badge)

A small C++ library for adding ANSI modifiers to text

 - [Requirements](#requirements)
 - [Building](#building)
 - [Installing](#installing)
 - [Testing](#testing)
 - [Documentation](#documentation)
 - [Current features](#current-features)
 - [Namespaces](#namespaces)
 - [Contributing](#contributing)

# Requirements
This library is built using `C++11` so it should work with most projects.
It uses no special compiler extensions.

# Building
This project is built using `cmake`.

To use the project in another `cmake` project simply clone it and add the sub-directory:
```cmake
add_subdirectory(ansi)

target_link_libraries(<target> ansi)
```

# Installing
This library can also be installed globally:

Configure and run using `ninja` or another generator of your choice:
```sh
$ cmake -DCMAKE_BUILD_TYPE=Release -B build -S . -G Ninja -DINSTALL_ANSI=ON -DBUILD_TESTING=OFF
$ cd build
$ sudo ninja install
```

# Testing
Testing should be enabled out of the box if you configure with
no parameters. This project uses GoogleTest and should integrate
with CTest

Configure for testing:
```sh
$ cmake -B build -S . -G Ninja
$ cd build
$ ninja tests
$ ctest
```

# Documentation
The documentation is available on [GitHub Pages](https://system-rat.github.io/ansi/)

The docs can also be built locally using the following steps:

Building the documentation requires a few dependencies:
 - doxygen
 - sphinx-doc

And some pip packages:
 - sphinx-rtd-theme
 - sphinx-sitemap
 - breathe

 After those are installed run the following:

```sh
$ cd docs/ && make html && firefox _build/html/index.html
```

# Current features

Currently the library implements the basic ANSI features:
 - [Bold, Italic, etc](#turning-text-bold-italic-etc)
 - [Color](#coloring-text)
 - [Erasing and moving](#erasing-lines-and-moving-the-cursor)
 - [Format strings](#format-strings)
 - [Other](#other-stuff)

**NOTE:** All of the features will automatically disable themselves
if the process is not attached to a terminal (Unless the library
is compiled with -DFORCE_TERM) or if the OS is Windows (for now)

## Turning text bold, italic, etc.
```cpp
#include <Ansi>
#include <iostream>

int main() {
    std::cout << ansi::manip::bold
    << ansi::manip::underline
    << "Bold and underlined text"
    << ansi::manip::reset << std::endl;
}
```

## Coloring text
```cpp
#include <Ansi>
#include <iostream>

int main() {
    std::cout << ansi::manip::color(ansi::Red)
        << "Red text"
        << ansi::manip::reset << std::endl;
}
```

## Erasing lines and moving the cursor
```cpp
#include <Ansi>
#include <iostream>

int main() {
    std::cout << "This will be erased"
        << ansi::manip::erase_line
        << "This will be written over" << std::endl;

    std::cout << ansi::manip::move_cursor(ansi::Up, 1)
        << "This is some other text" << std::endl;
}
```

## Creating stylings and applying them to text
```cpp
#include <Ansi>
#include <iostream>

int main() {
    auto styling = ansi::styling::Style(
        ansi::Red,
        {ansi::Bold, ansi::Underline}
    );

    std::cout << styling.apply("Styled text") << std::endl;
    // Text is automatically reset to it's normal style afterwards
}
```

## Format strings
The library contains a small format language for generating and caching
a `printf` compatible string from a format string. The grammar is pretty simple:

```cpp
#include <Ansi>
#include <cstdio>

int main() {
    // Within a `#[]` block the text color can be defined along with modifiers like Bold, Underline, etc.
    // `#$` denotes a variable (which will just be replaced with a `%s`)
    // Color can be an ANSI color, an 8 bit integer for Term256 colors, or 3 8 bit integers for RGB
    // separated by a colon, ie. 1:2:3
    // An empty `#[]` style block resets the text styling
    // An `ansi::manip::reset` is automatically inserted at the end of the text
    printf(ANSI("#[Bold, Underline, Red]##TEXT## #$"), "variable text");
}
```

## Other stuff
This library also contains two helper functions for enabling
RAW terminal output (UNIX only for now)

***WARNING***: This may or may not ruin some things

```cpp
#include <Ansi>
#include <iostream>

int main() {
    ansi::set_raw_tty(0);

    std::cout << ansi::styling::Style(
        ansi::Red, {ansi::Bold}).apply("Text") << "\n\r";
    // Since the terminal is in raw mode we have to
    // return the cursor to the start

    // Reset the terminal

    ansi::reset_tty(0);
}
```

# Namespaces
This library contains one main namespace and two namespaces
for different styles of printing:

## ansi
This contains the top-level types like `Color`, `TextModifier`, etc.
Along with some utilities.

### manip
This namespace holds various `ostream` manipulators for
manipulating text via function manipulators.

**NOTE**: Some of the functions take parameters to configure
the manipulator so the function returns a wrapper around an
`std::function` so it can have an overloaded `operator<<`. This
is so we don't get undesired overloading of just _any_ 
`std::function` that satisfies the type.

An example:
```cpp
#include <Ansi>
#include <iostream>

int main() {
    std::cout << ansi::manip::bold << "text";
    // Reset only bolding
    std::cout << ansi::manip::nobold << std::endl;
}
```

### styling
This namespace contains the `Style` class that describes some
style to apply to a piece of text that ensures the style is
reset after it's applied.

The style class can take multiple text modifiers and one color.

An example:

```cpp
#include <Ansi>
#include <iostream>

int main() {
    // NOTE: The first parameter is a StyleColor which can be
    // One of 3 formats: RGB, ANSI, or Term256
    // We can pass the ANSI color directly since the StyleColor
    // struct has an implicit constructor that takes an ANSI
    // color
    auto st = ansi::styling::Style(ansi::Red, {
        ansi::Underline
    });

    std::cout << st.apply("Text1")
        << " and "
        << st.apply("Text2") << std::endl;

    // Uses one of the 256 terminal colors by implicitly using
    // A StyleColor of type Term256
    auto st256 = ansi::styling::Style(220, {
        ansi::Bold
    });

    std::cout << st256.apply("Text1")
        << " and "
        << st256.apply("Text2") << std::endl;

    // Since the RGB StyleColor takes 3 parameters it has to
    // be manually instantiated
    auto strgb = ansi::styling::Style(
        ansi::StyleColor(30, 177, 249), {
        ansi::Bold
    });


    std::cout << strgb.apply("Text1")
        << " and "
        << strgb.apply("Text2") << std::endl;
}
```

# Contributing
Send a PR and I _might_ look into it, this is just a small
learning project. Be civil and all that jazz.