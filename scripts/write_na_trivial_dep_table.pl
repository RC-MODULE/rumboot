#!/usr/bin/perl

if(scalar(@ARGV) != 3) {
  die "Usage: $0 <dep_table_kind> <path_to_num_iterations.bin> <path_to_dep_table>\n  <dep_table_kind> is one of -mpe_vpe, -mpe_vpe_ppe, -vpe_ppe, -vpe, -ppe\n";
}

$kind = "$ARGV[0]";
$filepath = "$ARGV[1]";
$dep_table_file = "$ARGV[2]";

open (NUM_ITERATIONS_FILE, '<:raw', $filepath) or die "Cannot open file $filepath.\n";

$num_bytes_read = read(NUM_ITERATIONS_FILE,$bytes,4);
if($num_bytes_read != 4) {
  die "Cannot read file $filepath.\n";
}

$num_iterations = unpack ("l", $bytes);

close(NUM_ITERATIONS_FILE);

print "num_iterations = $num_iterations\n";


@dep_table = ();

if   ($kind eq "-mpe_vpe") {
  for($i=0; $i<$num_iterations; $i++) {
    push( @dep_table, (
      -1, -1, $i, -1, -1, -1, -2,
      -2, -2, -2, -1, -1, -1, -2
    ));
  }
}
elsif($kind eq "-mpe_vpe_ppe") {
  for($i=0; $i<$num_iterations; $i++) {
    push( @dep_table, (
      -1, -1, $i, -1, -1, -1, $i,
      -2, -2, -2, -1, -1, -1, -2
    ));
  }
}
elsif($kind eq "-vpe_ppe") {
  for($i=0; $i<$num_iterations; $i++) {
    push( @dep_table, (
      -2, -2, -1, -1, -1, -1, $i,
      -2, -2, -1, -1, -1, -1, -2
    ));
  }
}
elsif($kind eq "-vpe") {
  for($i=0; $i<$num_iterations; $i++) {
    push( @dep_table, (
      -2, -2, -1, -1, -1, -1, -2,
      -2, -2, -1, -1, -1, -1, -2
    ));
  }
}
elsif($kind eq "-ppe") {
  for($i=0; $i<$num_iterations; $i++) {
    push( @dep_table, (
      -2, -2, -2, -2, -2, -2, -1,
      -2, -2, -2, -2, -2, -2, -1
    ));
  }
}
else {
  die "Unknown dep_table_kind $kind.\n";
}


$n=0;
for($i=0; $i<$num_iterations; $i++) {
  print "{\n";
  for($k=0; $k<2; $k++) {
    for($j=0; $j<7; $j++) {
      print "  $dep_table[$n]";
      $n++;
    }
    print "\n";
  }
  print "}\n";
}

$arraysize = $num_iterations * 14;
$write_bytes = pack ("s[$arraysize]", @dep_table);

open (DEP_TABLE_FILE, ">:raw", $dep_table_file) or die "Cannot open $dep_table_file for writing.\n";
print DEP_TABLE_FILE  $write_bytes;
close (DEP_TABLE_FILE);


@dep_table_metrics = ($arraysize*2, $num_iterations, 2, 7, 1);
$write_bytes_metrics = pack ("l[5]", @dep_table_metrics);
$dep_table_metrics_file = "$dep_table_file" . ".metrics";
open (DEP_TABLE_METRICS_FILE, ">:raw", $dep_table_metrics_file) or die "Cannot open $dep_table_metrics_file for writing.\n";
print DEP_TABLE_METRICS_FILE $write_bytes_metrics;
close (DEP_TABLE_METRICS_FILE);

