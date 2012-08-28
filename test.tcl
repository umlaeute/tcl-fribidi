#!/usr/bin/tclsh

lappend auto_path .
package require fribidi
puts [fribidi::log2vis -string "foo"]

