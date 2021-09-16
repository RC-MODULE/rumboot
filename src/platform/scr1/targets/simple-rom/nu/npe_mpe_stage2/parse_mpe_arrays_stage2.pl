#!/usr/bin/perl -w

sub parse_and_write_hex_word {
  $word_hex = $_[0];
  $fh = $_[1];
  $line_length = $_[2];
  $skip_odd_hwords = $_[3];
  
  $word_hex =~ s/_//g;
  
    # Append Zeroes Until We Got The Target Line Length
  while(length($word_hex) < $line_length*2) {
    $word_hex = "0" . $word_hex; 
  }
  
  $word_len = length($word_hex);
  $enable = 1;
  if ("$ARGV[4]" eq "TRUNC16") { $cnt = 0; }
  else                       { $cnt = 2; }
  for($pos=$word_len-2;$pos>=0;$pos=$pos-2) {  # From The Last Byte Of String To The First Byte
    $byte_hex = substr($word_hex,$pos,2);      # Select Each Two Digits (Byte)
    $byte = hex($byte_hex);                    # Interpret It As A Byte In Hex
    # Write The Byte To File
    if ($skip_odd_hwords) { $enable=0x1&($cnt>>1); $cnt=0x3&($cnt+1); if ($enable) { print $fh pack("C", $byte); } }
    else { print $fh pack("C", $byte); }
  }
  
}

sub bin2hex {
  my $digit = $_[0];
  if    ($digit eq "0000") { return "0"; }
  elsif ($digit eq "0001") { return "1"; }
  elsif ($digit eq "0010") { return "2"; }
  elsif ($digit eq "0011") { return "3"; }
  elsif ($digit eq "0100") { return "4"; }
  elsif ($digit eq "0101") { return "5"; }
  elsif ($digit eq "0110") { return "6"; }
  elsif ($digit eq "0111") { return "7"; }
  elsif ($digit eq "1000") { return "8"; }
  elsif ($digit eq "1001") { return "9"; }
  elsif ($digit eq "1010") { return "A"; }
  elsif ($digit eq "1011") { return "B"; }
  elsif ($digit eq "1100") { return "C"; }
  elsif ($digit eq "1101") { return "D"; }
  elsif ($digit eq "1110") { return "E"; }
  elsif ($digit eq "1111") { return "F"; }
  else                     { return "0"; }
}

sub write_metrics_file {
  $size = $_[0];
  $filename = $_[1];
  $metrics_filename = $filename . ".metrics";
  open(my $fh, ">:raw", $metrics_filename) or die "Cannot open $metrics_filename for writing\n";
  print $fh pack("L", $size);
  close($fh);
}



  

if (scalar(@ARGV)!= 5) {
  print "Usage:\n";
  print "  $0 <cmd_file> <ft_binary_file> <wt_binary_file> <etalon_file> <TRUNC_MODE\n";
  die();
}

$ft_process=0;
$wt_process=0;
$cmd_process=0;
$et_process=0;

$ft_size=0;
$wt_size=0;
$cmd_size=0;
$et_size=0;

$cmd_file = "$ARGV[0]";
$ft_file  = "$ARGV[1]";
$wt_file  = "$ARGV[2]";
$et_file  = "$ARGV[3]";


while(defined($cline = <STDIN>)) {
  chomp($cline);
  $cline =~ s/\x0d//;    # Delete Windows Carriage Return
  $cline =~ s/\/\/.*//;  # Delete C-style line comments
  
  print STDOUT "$cline\n";
  
  if($cline eq "FT") {
    open($ft_fh, ">:raw", $ft_file) or die("Cannot open $ft_file for writing\n");
    $ft_process=1;
  }
  
  if($cline eq "WT") {
    open($wt_fh, ">:raw", $wt_file) or die("Cannot open $wt_file for writing\n");
    $ft_process=0;
    $wt_process=1;
  }
  
  if($cline eq "RES") {
    open($et_fh, ">:raw", $et_file) or die("Cannot open $et_file for writing\n");
    $wt_process=0;
    $et_process=1;
  }
  
  if($cline eq "CMD") {
    open($cmd_fh, ">:raw", $cmd_file) or die("Cannot open $cmd_file for writing\n");
    $et_process=0;
    $cmd_process=1;
  }
  
  if($ft_process == 1) {
    if($cline =~ m/0x([\w_]+)/) {
      $word_hex = $1;
      parse_and_write_hex_word($word_hex, $ft_fh, 1024/8, 0);
      $ft_size += 1024/8;
    }
  }
  elsif($wt_process == 1) {
    if($cline =~ m/0x([\w_]+)/) {
      $word_hex = $1;
      parse_and_write_hex_word($word_hex, $wt_fh, 1024/8, 0);
      $wt_size += 1024/8;
    }
  }
  elsif($et_process == 1) {
    if($cline =~ m/0x([\w_]+)/) {
      $word_hex = $1;
      parse_and_write_hex_word($word_hex, $et_fh, 512/8, 1);
      $et_size += 512/8;
    }
  }
  elsif($cmd_process == 1) {
    if($cline =~ m/0x([\w_]+)/) {   # Hex Representation
      $word_hex = $1;
      parse_and_write_hex_word($word_hex, $cmd_fh, 64/8, 0);
      $cmd_size += 64/8;
    }
    elsif($cline =~ m/0b([\w_]+)/) { # Binary Representation
      $word_bit = $1;
      $word_bit =~ s/_//g;
      while(length($word_bit) < 64) {
        $word_bit = "0" . $word_bit;
      }
      $word_hex="";
      for($pos=0;$pos<64;$pos=$pos+4) {
        $digit_hex = &bin2hex(substr($word_bit,$pos,4));
        $word_hex = $word_hex . $digit_hex;
      }
      parse_and_write_hex_word($word_hex, $cmd_fh, 64/8, 0);
      $cmd_size += 64/8;
    }
    
  }
}

if($ft_size > 0) { close($ft_fh); write_metrics_file($ft_size,$ft_file); }
else { die("FT instruction is not complete\n"); }
if($wt_size > 0) { close($wt_fh); write_metrics_file($wt_size,$wt_file); }
else { die("WT instruction is not complete\n"); }
if($et_size > 0) { close($et_fh); write_metrics_file($et_size/2,$et_file); }
else { die("RES instruction is not complete\n"); }
if($cmd_size > 0) { close($cmd_fh); write_metrics_file($cmd_size,$cmd_file); }
else { die("CMD instruction is not complete\n"); }

