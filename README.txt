tcl-fribidi
===========

(simple) FriBidi bindings for tcl, in order to support LTR text


OBJECTIVE:
for now i'm only interested in fixing the text-direction for short texts
(less than a paragraph)
i intend to use that mainly with tk's canvas' TEXT ITEMS

LATER:
think about a full wrapper around FriBidi

EVEN LATER:
bidi-support for tk-widgets
(but this might be an entirely different project)


building from git:
$ autoconf
$ ./configure
$ make
$ make install
