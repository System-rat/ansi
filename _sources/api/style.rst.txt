Styles
======

Another way of printing ANSI formatted text is to use a reusable style
object. It pre-computes ANSI formatted text and applies the style to
a string which can then be written to the output stream.

An example:

.. code-block:: c++

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

The styling namespace has the following members:

.. doxygennamespace:: ansi::styling
    :project: ANSI