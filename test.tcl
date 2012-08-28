#!/usr/bin/tclsh

lappend auto_path .
package require fribidi

proc print {s} {
 puts "old: $s"
 set S [fribidi::log2vis $s]
 puts "new: $S"

}

print "abcde"
print "שנבגק"

