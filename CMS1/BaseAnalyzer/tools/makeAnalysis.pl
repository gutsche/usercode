#!/usr/bin/env perl
use strict;
use warnings;
sub help{
        system("perldoc $0");
        exit;
    }
help() if (@ARGV==0);
my $path = 'CMS1';
my $sourcePackage = "SampleAnalyzer";
die 'CMSSW environment is not set. Please do eval `scramv1 runtime -csh`\n'if (! defined $ENV{CMSSW_BASE} );
my $name = $ARGV[0];

# making new package

# create directories skipping CVS directories
foreach  my $dir(`find $ENV{CMSSW_BASE}/src/$path/$sourcePackage/ -type d | grep -v CVS`){
    $dir =~ s/\n//;
    $dir =~ s/$sourcePackage/$name/;
    system("mkdir -vp $dir");
}

# create files
foreach  my $file(`find $ENV{CMSSW_BASE}/src/$path/$sourcePackage/ -type f | grep -v CVS|grep -v '~'`){
    $file =~ s/\n//;
    my $new_file = $file;
    $new_file =~ s/$sourcePackage/$name/g;
    print "$file -> $new_file\n";
    open(IN,$file) || die "Cannot open source file: $file\n$!\n";
    open(OUT,">$new_file") || die "Cannot creat new file: $new_file\n$!\n";
    while(my $line = <IN>){
	if ($line !~ /usercode/i){
	    $line =~ s/$sourcePackage/$name/g;
	    print OUT $line;
	}
    }
    close OUT;
    close IN;
}

print "Done.\n";
exit;

=head1 NAME

makeAnalysis.pl - create a new package in the analysis framework
  
=head1 SYNOPSYS 

makeAnalysis.pl <name>

=head1 DESCRIPTION

makeAnalysis.pl is used to make a new analysis in the analysis framework. 
It uses the SampleAnalyzer package as a template, stripping off all text
lines that contain "usercode". Just provide a name of the new package and
all files will be created for you. All you need to make get your analysis
running is to implement processEvent() method and if you need 
configure(const edm::ParameterSet&).

=head1 AUTHOR

Dmytro Kovalskyi, Unversity of California, Santa Barbara

dmytro@physics.ucsb.edu

=cut

