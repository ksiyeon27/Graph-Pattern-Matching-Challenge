#!/usr/bin/env perl

use strict;
use warnings;
use integer;

my $no_nodes = 1;
my $no_links = 1;

print join("\n  ",
  "{",
  '"directed": false,',
  '"multigraph": false,',
  '"graph": {},',
  '"nodes": [',
);
print "\n";

while (<>) {
  if (/v (\d+) (\d+)/) {
    if ($no_nodes) {
      $no_nodes = 0;
    } else {
      print ",\n";
    }
    print join "", "    {\"id\": ", $1, ", \"label\": ", $2, "}";
  }

  if (/e (\d+) (\d+) (\d+)/) {
    if ($no_links) {
      $no_links = 0;
      print "\n  ],\n  \"links\": [\n";
    } else {
      print ",\n";
    }
    print join "", "    {\"source\": ", $1, ", \"target\": ", $2, "}";
  }
}

print "\n  ]\n}\n";
