#!/usr/bin/perl -w


#-I [Start,End,Stride,test#,test#|Test file]

$iamfirst=1; #SHEISS ctest



# print "$strings[0]";
# print "$strings[1]";
# print "$strings[2]";
# print "$strings[3]";


# Prepeare Arrays With Reference Test Numbers
@strings=`ctest -N`;  # Get The Reference Test List
@ctestnums=();
@ctestnames=();
$i_last_=$#strings;
$tests_num=0;
for($i=0; $i<=$i_last_; $i++)
{
  if( $strings[$i] =~ m/#(\d+):\s*([-\w]+)/ )
  {
    $ctestnums[$tests_num] = $1;
    $ctestnames[$tests_num] = $2;
    $tests_num++;
  }
}
  
# for($i=0; $i<$tests_num; $i++)
# {
#   print "#$ctestnums[$i] : $ctestnames[$i] $i\n";
# }
# 

while(defined($cline=<STDIN>))
{
  chomp($cline);
  if( $cline =~ m/#(\d+):\s*([-\w]+)/ )
  {
    $mytestnum = $1;
    $mytestname = $2;
    
    $exit=0;
    for($i=0; $i<$tests_num and not $exit; $i++)
    {
      
#       print "Searching $mytestname vs $ctestnames[$i]\n";
    
      if($mytestname eq $ctestnames[$i])
      {
	$mytestnum = $ctestnums[$i];
	$exit = 1;
	if($iamfirst)
	{
	  print "$mytestnum,$mytestnum,1";   # Special Magic For First Index Of ctest -I Key
	  $iamfirst=0;
	}
	else
	{
	  print ",$mytestnum";
	}
      }
    }
  }
}
print "\n";

