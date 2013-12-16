#!/usr/bin/env perl
use File::Basename;

# generates compliance report stats

my $bindings_file = "src/bindings.h";
my $symbol_dir = "docs/symbols/*";

# process bindings file
open( my $bfile, "<", "$bindings_file" ) or die "Failed to open file '$bindings_file' : $!\n";

my %bindings; # binding => 1
while( <$bfile> ){
    if( m/^ \s* P [FS] \( \" (?<binding> [^"]+ ) \"/x ){
        $bindings{ $+{binding} } = 1;
    }
}

close $bfile;


# process symbol files
my %symbols; # file => binding => 1
for my $file ( glob $symbol_dir ){
    $symbols{ $file } = {};

    open( my $sfile, "<", "$file" ) or die "Failed to open file '$file' : $!\n";

    while( <$sfile> ){
        chomp;
        $symbols{ $file }->{ $_ } = 1;
    }

    close $sfile;
}


# merge to produce stats
my %stats; # file => {}, implemented => #, total => #
for my $file ( keys %symbols ){
    $stats{$file}->{"implemented"} = 0;
    $stats{$file}->{"total"} = 0;

    for my $binding ( keys %{ $symbols{$file} } ){
        if( exists $bindings{ $binding } ){
            ++$stats{$file}->{"implemented"};
        }
        ++$stats{$file}->{"total"};
    }
}


my ($total, $implemented);

# output stats
for my $file ( keys %stats ){
    print basename $file ;
    print ": ";

    $implemented += $stats{$file}->{"implemented"};
    print $stats{$file}->{"implemented"};

    print " / ";

    $total += $stats{$file}->{"total"};
    print $stats{$file}->{"total"};
    print "\n";
}

print "\ntotal: $implemented / $total\n";
