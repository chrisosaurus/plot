#!/usr/bin/env perl
use strict;
use warnings;

use File::Slurp;

# we step through each of the specified functions and capture all the c functions that
# have a correctly formatted comments declaring their scheme bindings
#
# example of a builtin:
#
#    /* (display obj)
#    ...
#    */
#    struct plot_value * plot_func_display(struct plot_env *env, struct plot_value **args, int argc);
#
# from this we extract the 3 useful pieces of information
#   'display'
#   'plot_func_display'
#   'struct plot_value **args'
#
#
# example of a syntactic form:
#
#    /* (if cond if-expr else-expr)
#     * (if cond if-expr
#    ...
#    */
#    struct plot_value * plot_form_if(struct plot_env *env, struct plot_expr **args, int argc);
#
# from this we extract the 3 useful pieces of information
#   'if'
#   'plot_form_if'
#   'struct plot_expr **args'
#
#
# the first line of a comment may also end with the string " - library form" to indicate the following
#   definition should be loaded into the initial env
#
# using this information we are able to generate the list of bound functions pre-runtime

# headers to interrogate
my @headers = qw{funcs.h number.h string.h pair.h character.h forms.h};
# header file to generate
my $binding_header = "src/bindings.h";

# NB: 'len' argument to macro PB has to include null terminator (perl's length does not include this)
my $boilder_plate = "
/* functions to bind */
struct plot_binding {
    plot_symbol sym;
    plot_value func;
};

/* Plot Builtin */
#define PB(str, len, func) {{str, len,  len, 0}, {{-1, 0}, plot_type_builtin, {.builtin = {func}}}}
/* Plot Syntactic */
#define PS(str, len, func) {{str, len,  len, 0}, {{-1, 0}, plot_type_syntactic, {.syntactic = {func}}}}

";


open( my $output, ">", "$binding_header" ) or die "Failed to open '$binding_header' : $!\n";
print $output "/* This file is managed automatically by build/generate_bindings.pl */\n";
print $output join "\n", map {"#include \"$_\""} @headers;

print $output "\n", $boilder_plate;

my (@bindings, @library_forms);

for my $header (@headers){
    # NB: we prepend 'src/'
    my $contents = read_file "src/$header"; # File::Slurp read_file
    while ($contents =~ m#^\/ \* \s+ \( (?<scheme> \S+ ) [^\)]* \) \s* (?<library_form> \- \s* library \s* form )? \s+
                      (?: ^ \s* \* [^\/]* \s+ )*
                      (?: ^ \s* \* \/ \s* ) \s+
                          ^ struct \s plot_value \s* \* \s* (?<cfunc> [^\(]+ ) \( \s* struct \s plot_env \s* \* \s* env, \s* struct \s plot_(?<type> sexpr | value) .*   $ #xmg){

        my $len = length($+{scheme}) + 1;

        if( $+{type} eq 'sexpr' ){
            #syntactic form
            if( exists $+{library_form} ){
                push @library_forms, "\tPS(\"$+{scheme}\", $len, $+{cfunc})";
            } else {
                push @bindings, "\tPS(\"$+{scheme}\", $len, $+{cfunc})";
            }
        } else {
            #builtin
            if( exists $+{library_form} ){
                push @library_forms, "\tPB(\"$+{scheme}\", $len, $+{cfunc})";
            } else {
                push @bindings, "\tPB(\"$+{scheme}\", $len, $+{cfunc})";
            }
        }
    }
}

# print initial bound set ('library forms')
print $output "struct plot_binding library_forms[] = {\n";
print $output join ",\n", @library_forms;
# close library_forms[]
print $output "\n};\n\n";

# print other bindings
print $output "struct plot_binding bindings[] = {\n";
print $output join ",\n", @bindings;
# close bindings[]
print $output "\n};\n\n";

close $output;

