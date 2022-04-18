std::ostream manipulators
=========================

One way of using this library is to use the `ansi::manip` namespace
which provides `std::ostream` manipulators (like `std::endl` and co.).

The manipulators are named based on the effect they have on text, so
the `ansi::manip::bold` manipulator boldens text.

Certain manipulators take parameters and return a wrapper struct with
a lambda so the global namespace isn't polluted with an overload for
lambdas with that kind of type signature.

Some example code:

.. code-block:: c++

    #include <iostream>
    #incldue <Ansi>

    int main() {
        std::cout << ansi::manip::bold << "Bold text" << std::endl;
        std::cout << ansi::manip::color(ansi::Red) << "Red and bold text" << std::endl;
        std::cout << ansi::manip::reset << "Normal text" << std::endl;
    }

The supported text effects are:
    - `bold`
    - `faint`
    - `italic`
    - `underline`
    - `blink` (usually not supported by terminals)
    - `reverse`
    - `hidden`
    - `strikethrough`

Individual text effects can be reset by prefixing the function name with `no`, ie. `ansi::manip::nobold`.
Or all effects (including color) can be reset using `ansi::manip::reset`.

The manip namespace has the following members:

.. doxygennamespace:: ansi::manip
    :project: ANSI