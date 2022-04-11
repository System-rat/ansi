Top-level types and functions
=============================

An ANSI terminal library. This library contains various utilities for
outputting colors, styles and moving the cursor in an ANSI compatible terminal.
The top level `ansi` namespace contains some frequently used types and functions.

.. doxygennamespace:: ansi
    :desc-only:
    :project: ANSI

.. doxygenenum:: ansi::Color
    :project: ANSI

.. doxygenenum:: ansi::TextModifier
    :project: ANSI

.. doxygenclass:: ansi::ManipulatorFunc
    :project: ANSI

.. doxygenstruct:: ansi::StyleColor
    :project: ANSI

.. doxygenfunction:: ansi::set_raw_tty
    :project: ANSI

.. doxygenfunction:: ansi::reset_tty
    :project: ANSI

.. doxygenfunction:: ansi::format_str
    :project: ANSI