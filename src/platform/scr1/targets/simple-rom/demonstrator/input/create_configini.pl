
# config.ini parameters
my $H   = 0 ;
my $W   = 0 ;
my $C   = 0 ;
my $Tp  = 0 ;
my $Bp  = 0 ;
my $Lp  = 0 ;
my $Rp  = 0 ;

my $R   = 0 ;
my $S   = 0 ;
my $Ds  = 1 ;
my $Dr  = 1 ;
my $Sw  = 1 ;
my $Sh  = 1 ;
my $K   = 16;           # $ARGV[3]

my $sn      = 7 ;       # $ARGV[0]

my $LINES   = 16;
my $COLUMNS = 64;
my $BUF_NMB = 1 ;

my $fn  = "hwc";
my $fnb = "rsc_";

# config.ini parameters end

my $src_vectors = 32;   # $ARGV[1]
my $RSC         = 64;   # $ARGV[2]

my $Wo = 0;
my $Ho = 0;

my $fnci = "config.ini";
my $fncbi= "config_base.ini";

# create $fn file and fill it with input cube data values
sub hwc_init {

    my $fn = $_[0];
    my $H  = $_[1];
    my $W  = $_[2];
    my $C  = $_[3];

    #print "hwc_init $fn $H $W $C\n";

    open ($fp, ">$fn");
 
    for ($i=0; $i<$H; $i++) {
        for ($j=0; $j<$W; $j++) {
            for ($k=0; $k<$C; $k++) {

                $randl = (0xFFFFFFFF >> 32-$sn-1)+1;

                $val = int(rand($randl));
                $val = sprintf("0x%X", $val);

                print $fp "$val";

                if (($j<$W-1) || ($k<$C-1)) {print $fp ", "};
            }
        }
        print $fp "\n";
    }

    close ($fp);
}

# create $fnb<nmb> files and fill them with kernel<nmb> values
sub rsc_init {
    my $fnb= $_[0];
    my $R  = $_[1];
    my $S  = $_[2];
    my $C  = $_[3];

    for ($f=0; $f<$K; $f++) {

        my $fn = "$fnb$f";
        hwc_init($fn, $R, $S, $C);    
    }
}

sub parse_argv {
    if ($ARGV[0]) {$sn = $ARGV[0];}
    else {die "can't set sn $!\n";}

    if ($ARGV[1]) {$src_vectors = $ARGV[1];}
    else {die "can't set src_vectors $!\n";}

    if ($ARGV[2]) {$RSC = $ARGV[2];}
    else {die "can't set RSC $!\n";}

    if ($ARGV[3]) {$K = $ARGV[3];}
    else {die "can't set K $!\n";}

    #print "sn $sn\n";
    #print "src_vectors $src_vectors\n";
    #print "RSC $RSC\n";
    #print "K $K\n";
}

sub iter_hwcrs {
    $i = 0;

    my $RSC        = $_[0];
    my $src_vectors= $_[1];

    do {
        $R = int(rand(int($RSC/2)));
        $S = int(rand(int($RSC/2)));
        $C = int(rand(int($RSC/4)));

        $i++;
    } while ($R*$S*$C != $RSC);
    #print "$R $S $C $i\n";

    do {
        $Ho = int(rand(int($src_vectors/2)));
        $Wo = int(rand(int($src_vectors/2)));

        $i++;
    } while ($Ho*$Wo != $src_vectors);
    #print "$Ho $Wo $i\n";

    $H = $Ho + $R - 1;
    $W = $Wo + $S - 1;

    #print "$H $W\n";
    print "H $H W $W Ho $Ho Wo $Wo R $R S $S C $C\n";
}

sub random_configs {
    $Hp = $H + $Tp + $Bp;
    $Wp = $W + $Lp + $Rp;
    $Rd = ($R - 1)*$Dr + 1;
    $Sd = ($S - 1)*$Ds + 1;
    $Ho = ($Hp - $Rd)/$Sh + 1;
    $Wo = ($Wp - $Sd)/$Sw + 1;

    $Hoi= int(($Hp - $Rd)/$Sh + 1);
    $Woi= int(($Wp - $Sd)/$Sw + 1);

    if (($Ho-$Hoi) || ($Wo-$Woi)) {die "Ho or Wo isn't integer\n";}
}

sub configini_fillin {

    my $fnci   = $_[0];
    my $fncbi  = $_[1];

    open ($fpr, "<$fncbi");
    open ($fpw, ">$fnci");

    while ($line = <$fpr>) {

        if ($line =~ m/^H\s+=\s+\d/) {$line =~ s/\d+/$H/;}
        elsif ($line =~ m/^W\s+=\s+\d/) {$line =~ s/\d+/$W/;}
        elsif ($line =~ m/^C\s+=\s+\d/) {$line =~ s/\d+/$C/;}
        elsif ($line =~ m/^Tp\s+=\s+\d/) {$line =~ s/\d+/$Tp/;}
        elsif ($line =~ m/^Bp\s+=\s+\d/) {$line =~ s/\d+/$Bp/;}
        elsif ($line =~ m/^Lp\s+=\s+\d/) {$line =~ s/\d+/$Lp/;}
        elsif ($line =~ m/^Rp\s+=\s+\d/) {$line =~ s/\d+/$Rp/;}
        elsif ($line =~ m/^R\s+=\s+\d/) {$line =~ s/\d+/$R/;} 
        elsif ($line =~ m/^S\s+=\s+\d/) {$line =~ s/\d+/$S/;} 
        elsif ($line =~ m/^Ds\s+=\s+\d/) {$line =~ s/\d+/${Ds}/;}
        elsif ($line =~ m/^Dr\s+=\s+\d/) {$line =~ s/\d+/${Dr}/;}
        elsif ($line =~ m/^Sw\s+=\s+\d/) {$line =~ s/\d+/${Sw}/;}
        elsif ($line =~ m/^Sh\s+=\s+\d/) {$line =~ s/\d+/${Sh}/;}
        elsif ($line =~ m/^K\s+=\s+\d/) {$line =~ s/\d+/$K/;} 
        elsif ($line =~ m/^sn\s+=\s+\d/) {$line =~ s/\d+/${sn}/;}
        elsif ($line =~ m/^LINES\s+=\s+\d/) {$line =~ s/\d+/${LINES}/;}
        elsif ($line =~ m/^COLUMNS\s+=\s+\d/) {$line =~ s/\d+/${COLUMNS}/;}
        elsif ($line =~ m/^BUF_NMB\s+=\s+\d/) {$line =~ s/\d+/${BUF_NMB}/;}

        elsif ($line =~ m/^user_X_data_file_name\s+=/) {
            $line =~ s/=.+/= ${fn}/;
        }
        elsif ($line =~ m/^user_W_data_base_file_name\s+=/) {
            $line =~ s/=.+/= ${fnb}/;
        }

        print $fpw $line;
    }

    close ($fpr);
    close ($fpw);    
}

sub set_configs {
    if ($ARGV[0] == "random") {random_configs;}
    else {
        parse_argv;
        iter_hwcrs($RSC, $src_vectors);
    }
}

set_configs;

hwc_init($fn, $H, $W, $C);
rsc_init($fnb, $R, $S, $C);

configini_fillin($fnci, $fncbi);

exit 0;