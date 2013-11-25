#!/usr/bin/env perl
use strict;
use warnings;

use File::Slurp;

# we step through each of the specified functions and capture all the c functions that
# have a correctly formatted comments declaring their scheme bindings
#
# example of a legacy builtin:
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
my @headers = qw{funcs.h number.h string.h pair.h character.h forms.h control.h};
# header file to generate
my $binding_header = "src/bindings.h";

# NB: 'len' argument to macro PB has to include null terminator (perl's length does not include this)
my $boilder_plate = "
/* functions to bind */
struct plot_binding {
    plot_symbol sym;
    plot_value func;
};

/* Plot Legacy Builtin */
#define PLB(str, len, func) {{str, len,  len, 0}, {{-1, 0}, plot_type_legacy, {.legacy = {func}}}}
/* Plot Syntactic */
#define PS(str, len, func) {{str, len,  len, 0}, {{-1, 0}, plot_type_form, {.form = {func, 1}}}}

";


open( my $output, ">", "$binding_header" ) or die "Failed to open '$binding_header' : $!\n";
print $output "/* This file is managed automatically by build/generate_bindings.pl */\n";
print $output join "\n", map {"#include \"$_\""} @headers;

print $output "\n", $boilder_plate;

my (@library_forms, @core_forms);

for my $header (@headers){
    # NB: we prepend 'src/'
    my $contents = read_file "src/$header"; # File::Slurp read_file
    while ($contents =~ m#^\/ \* \s+ \( (?<scheme> \S+ ) [^\)]* \) \s* (?<args> (?:\s*\-\w+\s*)*  )? \s+
                      (?: ^ \s* \* [^\/]* \s+ )*
                      (?: ^ \s* \* \/ \s* ) \s+
                          ^ struct \s plot_value \s* \* \s* (?<cfunc> [^\(]+ ) \( .*   $ #xmg){

        my $len = length($+{scheme}) + 1;

        my @args = split '\s+', $+{args};
        my $contents;

        if( grep "-syntax", @args ){
            $contents = "\tPS(\"$+{scheme}\", $len, $+{cfunc})";
        } else {
            # legacy builtin is default
            $contents = "\tPLB(\"$+{scheme}\", $len, $+{cfunc})";
        }

        if( grep "-core", @args ){
            push @core_forms, $contents;
        } else {
            # library form is default
            push @library_forms, $contents;
        }

    }
}

# print initial bound set ('library forms')
print $output "struct plot_binding core_forms[] = {\n";
# FIXME temprarily moving all bindings into library_forms (until we can actually bind libraries)
print $output join ",\n", (@core_forms, @library_forms);
# close library_forms[]
print $output "\n};\n\n";

# print other bindings
print $output "struct plot_binding library_forms[] = {\n";
print $output join ",\n", @library_forms;
# close bindings[]
print $output "\n};\n\n";

close $output;

