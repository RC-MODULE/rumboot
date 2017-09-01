#!/usr/bin/perl -w

if (scalar(@ARGV)== 2) {
  $hex64_file=$ARGV[0];
  $hex32_file=$ARGV[1];
  print "Running Boot Image Scatter.\n";
  print "Source File: $hex64_file.\n";
  print "Destination File: $hex32_file.\n";
} else {
  die "Usage: ./scatter_hex.pl <hex64_file> <hex64_file>  \n";
}


open(SRC,"<", "$hex64_file") or die "Can't open Source File $hex64_file \n";
open(DST,">", "$hex32_file") or die "Can't open Destination File $hex32_file \n";

while ($cline = <SRC>)
{
  chomp($cline);
  if($cline =~  m/([0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F])([0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F])/)   {
    print DST "$2\n";
    print DST "$1\n";
  }
  else
  {
    print DST "$cline\n";
  }
}

close(SRC);
close(DST);
