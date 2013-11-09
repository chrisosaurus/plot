#!/usr/bin/env perl
use strict;
use warnings;

use File::Slurp;

# we step through each of the specified functions and capture all the c functions that
# have a correctly formatted comments declaring their scheme bindings
#
#    /* (display obj)
#    ...
#    */
#    struct plot_value * plot_func_display(struct plot_env *env, struct plot_value **args, int argc);
#
# from this we extract the 2 useful pieces of information
#   'display'
#   'plot_func_display'
#
# using this information we are able to generate the list of bound functions at pre-runtime

# headers to interrogate
my @headers = qw{src/funcs.h src/number.h src/string.h src/pair.h src/character.h};

# key is scheme binding, value is c function
my %exposed;

for my $header (@headers){
    #print "matching $header\n";

    my $contents = read_file $header; # File::Slurp read_file
    while ($contents =~ m#^\/ \* \s+ \( (?<scheme> \S+ ) .+ \s+
                      (?: ^ \s* \* [^\/]* \s+ )*
                      (?: ^ \s* \* \/ \s* ) \s+
                          ^ struct \s plot_value \s* \* \s* (?<cfunc> [^\(]+ ) \( .* $ #xmg){
        print "Matched cfunc $+{cfunc} and scheme $+{scheme}\n";
        $exposed{ $+{scheme} } = $+{cfunc};
    }

    #print "\n";
}

