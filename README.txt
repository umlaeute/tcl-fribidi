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

INSTALLATION
------------

building from git:
$ autoconf
$ ./configure
$ make
$ make install

if not building from git, you should be able to omit the first step (running
'autoconf')


DEPENDENCIES
------------
this extension depends on the GNU libfribidi library to handle bi-directional
text.
you can get it from http://fribidi.org/
if you are using Debian (or a Debian-derivative, like ubuntu), there is already
a package available:
$ sudo aptitude install libfribidi-dev
