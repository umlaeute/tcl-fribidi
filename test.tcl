#!/usr/bin/tclsh

lappend auto_path .
package require fribidi
puts [sha1 -string "foo"]

