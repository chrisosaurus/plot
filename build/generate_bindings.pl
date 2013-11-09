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
my @headers = qw{funcs.h number.h string.h pair.h character.h};
# header file to generate
my $binding_header = "src/bindings.h";

# NB: 'len' argument to macro PBB has to include null terminator (perl's length does not include this)
my $boilder_plate = "
/* functions to bind */
struct plot_binding {
    plot_symbol sym;
    plot_value func;
};

#define PBB(str, len, func) {{str, len,  len}, {{-1, 0}, plot_type_builtin, {.builtin = {func}}}}

struct plot_binding bindings[] = {
";


open( my $output, ">", "$binding_header" ) or die "Failed to open '$binding_header' : $!\n";
print $output "/* This file is managed automatically by build/generate_bindings.pl */\n";
print $output join "\n", map {"#include \"$_\""} @headers;

print $output "\n", $boilder_plate;

my @bindings;

for my $header (@headers){
    # NB: we prepend 'src/'
    my $contents = read_file "src/$header"; # File::Slurp read_file
    while ($contents =~ m#^\/ \* \s+ \( (?<scheme> \S+ ) .+ \s+
                      (?: ^ \s* \* [^\/]* \s+ )*
                      (?: ^ \s* \* \/ \s* ) \s+
                          ^ struct \s plot_value \s* \* \s* (?<cfunc> [^\(]+ ) \( .* $ #xmg){

        my $len = length($+{scheme}) + 1;
        push @bindings, "\tPBB(\"$+{scheme}\", $len, $+{cfunc})";
    }
}

print $output join ",\n", @bindings;

# close bindings[]
print $output "\n};\n\n";

close $output;

