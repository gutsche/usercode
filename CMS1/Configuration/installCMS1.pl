#!/usr/bin/env perl
use strict;
use warnings;

die "Usage: $0 <cms1 tag>\n" if (@ARGV != 1);
die "Please setup proper CMSSW environment\n" if ( ! defined $ENV{CMSSW_VERSION} || ! defined $ENV{CMSSW_BASE});

if (-e "$ENV{CMSSW_BASE}/src/CMS1"){
    print "Found existing CMS1 installation. Use it and ignore requested tag. Please update by hand if it's necessary\n";
}else{
    system("cd $ENV{CMSSW_BASE}/src; cvs -d \$CVSROOT co -r $ARGV[0] -d CMS1 UserCode/Gutsche/CMS1");
}

my $required_packages = "$ENV{CMSSW_BASE}/src/CMS1/Configuration/required_packages.txt";
open(IN, $required_packages) || die "Cannot access list of required packages:\n$required_packages\n$!\n";
while (my $line = <IN>){
    $line =~ s/\n//;
    next if ($line =~ /^\s*\#/ || $line !~ /\S/);
    my @fields = split(/\s+/,$line);
    if (@fields != 3 || $fields[0] !~ /^CMSSW_[\d\*]+_[\d\*]+_[\d\*]+$/){
	print "incorrect format of required package information:\n$line\nSkipped\n";
	next;
    }
    $fields[0] =~ s/\*+/\*/g;
    $fields[0] =~ s/\*/\\d\+/g;
    if ($ENV{CMSSW_VERSION} =~ $fields[0]){
	system("cd $ENV{CMSSW_BASE}/src; cvs -d \$CVSROOT co -r $fields[2] $fields[1]");
    }
}
print "Compiling and building code ...\n";
system("cd $ENV{CMSSW_BASE}/src; scramv1 b distclean; scramv1 b");
  
exit
