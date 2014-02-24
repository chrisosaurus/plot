#include <stdio.h>
#include <string.h> /* strcmp */

#include "forms.h"
#include "value.h"
#include "plot.h"
#include "funcs.h"
#include "eval.h"
#include "hash.h"
#include "env.h"

#define DEBUG 0

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

/* (define-library (library name) body...) -core -syntax
 * define a library
 *
 * library definitions are of the form
 * (define-library <library-name>
 *      <library-declaration> ...)
 *
 * where a <library-declaration> is any of:
 *      (export <export spec> ...)
 *      (import <import set> ...)
 *      (begin <command or defintion> ...)
 *      (include <filename1> <filename2> ...)
 *      (include-ci <filename1> <filename2> ...)
 *      (include-library-declaration <filename1> <filename2> ...)
 *      (cond-expand <ce-clause1> <ce-clause2> ...)
 */
struct plot_value * plot_form_define_library(struct plot_env *env, struct plot_value *sexpr){
    /* library name */
    plot_value *name;
    /* body to iterate through */
    plot_value *body;
    /* current cursor for iteration*/
    plot_value *cur;
    /* current item we are processing in iteration */
    plot_value *item;
    /* library we generate */
    plot_value *lib;

    /* temporary returned when using other functions
     * to assist
     */
    plot_value *ret;

    /* temporary hash value of symbol being inspected
     * comparing against symbol hash is UGLY
     * FIXME TODO
     */
    unsigned long long hash = 0;

    /* FIXME ignore unused variable warnigns */
    #pragma GCC diagnostic ignored "-Wunused-variable"

    /* two new envs, internal and external/exported */
    plot_env *in, *ex;


    /* A library has 3 'parts': */

    /* IMPORTS
     * are processed immediately and modify the internal env.
     */

    /* DEFINITIONS
     * (inside begin exprs)
     * are processed after all imports are performed.
     */
    plot_value *definitions = null;

    /* EXPORTS
     * are processed after all DEFINITIONS have been processed.
     */
    plot_value *exports = null;


    if( !sexpr || sexpr->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected at least 2 args", "plot_form_define_library");
    }

    name = car(sexpr);
    body = cdr(sexpr);

    /* no parents to either as they are both 'clean' (or emtpy) envs */
    ex = plot_alloc_env(0);
    in = plot_alloc_env(0);
    /* construct out library object */
    lib = plot_new_library(in, ex);

    switch( name->type ){
        case plot_type_pair:
            break;
        case plot_type_symbol:
            break;
        default:
            return plot_runtime_error(plot_error_bad_args, "library name was of incorrect type", "plot_form_define_library");
    }

    /* go through body
     * examine each part
     *  if import -> process immediately
     *  if definition -> add to `defintions`
     *  if export -> add to `exports
     */
    for( cur = body; cur->type == plot_type_pair; cur = cdr(cur) ){
        item = car(cur);

        if( item->type != plot_type_pair ){
            return plot_runtime_error(plot_error_bad_args, "library body was malformed, expected sub-sexpr but found expr", "plot_form_define_library");
        }

        if( car(item)->type != plot_type_symbol ){
            return plot_runtime_error(plot_error_bad_args, "library body was malformed, expected symbol", "plot_form_define_library");
        }


        hash = car(item)->u.symbol.hash; /* FIXME checking on hash is ugly */

        /* (export <export spec> ...)
         * add symbols to u.library.exported
         * symbols may have been defined or may later be defined
         * FIXME TODO
         */
        if( hash == 656723401804llu){ /* make hasher && ./hasher export */
            return plot_runtime_error(plot_error_unimplemented, "define-library : export unimplemented", "plot_form_define_library");
        }

        /* (import <import set> ...)
         * should be able to re-use normal import and just specify
         * u.library.internal as the env to eval in
         * FIXME TODO
         */
        else if( hash == 656723400763llu){ /* make hasher && ./hasher import */
            ret = plot_form_import(in, item);
            if( !ret || ret->type == plot_type_error ){
                puts("plot_form_define_library (import)");
                display_error_expr(item);
                return ret;
            }
        }

        /* (begin <command or defintion> ...)
         * normal eval with env specified as u.library.internal
         * FIXME TODO
         */
        else if( hash == 6416384521llu){ /* make hasher && ./hasher begin */
            return plot_runtime_error(plot_error_unimplemented, "define-library : begin unimplemented", "plot_form_define_library");
        }

        /* (include <filename1> <filename2> ...)
         * (include-ci <filename1> <filename2> ...)
         * (include-library-declaration <filename1> <filename2> ...)
         * various include forms
         * FIXME TODO
         */
        else if( hash == 51254500566408llu ){ /* make hasher && ./hasher include */
            return plot_runtime_error(plot_error_unimplemented, "define-library : include unimplemented", "plot_form_define_library");
        }

        else if( hash == 9560169544426541301llu ){ /* make hasher && ./hasher include-ci */
            return plot_runtime_error(plot_error_unimplemented, "define-library : include-ci unimplemented", "plot_form_define_library");
        }

        else if( hash == 2548278337831442659llu ){ /* make hasher && ./hasher include-library-declaration */
            return plot_runtime_error(plot_error_unimplemented, "define-library : include-library-declaration unimplemented", "plot_form_define_library");
        }

        /* (cond-expand <ce-clause1> <ce-clause2> ...)
         * FIXME TODO
         */
        else if( hash == 7383587544085817029llu){ /* make hasher && ./hasher cond-expand */
            return plot_runtime_error(plot_error_unimplemented, "define-library : cond-expand unimplemented", "plot_form_define_library");
        }

        /* ERROR: unknown subform */
        else {
            printf("hash of offender was '%llu'\n", car(item)->u.symbol.hash );
            puts("error found when processing `define-library` : unknown subform");
            display_error_expr(item);
            return plot_runtime_error(plot_error_runtime, "library body was malformed, found unknown subform", "plot_form_define_library");
        }
    }

    if( cur->type != plot_type_null ){
        return plot_runtime_error(plot_error_runtime, "library body was malformed, expected pair or null but found neither", "plot_form_define_library");
    }

    /* process DEFINITIONS
     * FIXME TODO
     */

    /* process EXPORTS
     * FIXME TODO
     */

    return lib;
}

/* (import (library name) ...) -core -syntax
 * import some libraries
 */
struct plot_value * plot_form_import(struct plot_env *env, struct plot_value *sexpr){

    /* 5.2 Import Declarations, page 25
     * an import declaration takes the following form:
     * (import <import-set> ...)
     *
     * <import-set> takes one of the following forms:
     *      <library-name>
     *      (only <import-set> <identifier> ...)
     *      (except <import-set> <identifier> ...)
     *      (prefix <import-set> <identifier> ...)
     *      (rename <import-set> (<identifier1> <identifier2>) ...)
     *
     */

    return plot_runtime_error(plot_error_unimplemented, "not yet implemented", "plot_form_import");
}

/* (plot-bind identifier ... ) -syntax
 * looks through plot's internal bindings and binds
 * identifiers to current scope
 *
 * eventually exported by (plot internal)
 */
struct plot_value * plot_form_plot_bind(struct plot_env *env, struct plot_value *sexpr){
    return plot_runtime_error(plot_error_unimplemented, "not yet implemented", "plot_form_plot_bind");
}

/* (begin body...)
 */
struct plot_value * plot_form_begin(struct plot_env *env, struct plot_value *sexpr){
    plot_value *value = 0;
    plot_value *cur;

    for( cur=sexpr; cur->type != plot_type_null; cur = cdr(cur) ){
        if( value ){
            plot_value_decr(value);
        }
        value = plot_eval_expr(env, car(cur));
    }

    if( ! value ){
        value = plot_new_unspecified();
    }

    return value;
}


/* (define what value) -syntax
 */
struct plot_value * plot_form_define(struct plot_env *env, struct plot_value *sexpr){
    plot_value *value;
    plot_value *name;
    plot_value *body;
    plot_value *args;

    /* define has 2 forms:
     * (define a <value>)
     * (define (b args) <function body>)
     */

    name = car(sexpr);
    body = cdr(sexpr);

    #if DEBUG
    puts("define with name:");
    display_error_expr(name);
    puts("with body:");
    display_error_expr(body);
    #endif

    if( name->type == plot_type_pair ){
        /* function form */

        args = cdr(name);
        name = car(name);

        value = plot_new_lambda(env, plot_new_pair( args, body ));
        plot_env_define(env, &(name->u.symbol), value);
        plot_value_decr(value);

    } else {
        /* value form */

        if( name->type != plot_type_symbol ){
            return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_symbol", "plot_form_define");
        }

        #if DEBUG_FORM || DEBUG
        puts("\tDEFINE: getting value to store");
        #endif

        if( cdr(body)->type != plot_type_null ){
            return plot_runtime_error(plot_error_bad_args, "trailing argument to define", "plot_form_define");
        }
        /* 2nd subform isnt known to be a value ! */
        value = plot_eval_expr(env, car(body));
        if( ! value ){
            return plot_runtime_error(plot_error_internal, "call to eval expr returned null", "plot_form_define");
        }

        if( value->type == plot_type_error ){
            puts("plot_eval_form (define)");
            return value;
        }

        #if DEBUG_FORM || DEBUG
        puts("\tDEFINE: value fetched");
        #endif

        #if DEBUG_FORM || DEBUG
        puts("\tDEFINE: success!");
        printf("\tStoring value type '%d', under name '%s'\n'", value->type, name->u.symbol.val);
        #endif
        plot_env_define(env, &(name->u.symbol), value);
        /* decrement value as eval and define will both increment it and we are not keeping a reference around */
        plot_value_decr(value);
    }

    return plot_new_unspecified();
}

/* (lambda args body...) -syntax
 */
struct plot_value * plot_form_lambda(struct plot_env *env, struct plot_value *sexpr){
    plot_value *args, *arg;
    plot_value *body;

    /* our sexpr will be of the form (args body... )
     */

    args = car(sexpr);
    body = cdr(sexpr);

    if( body->type == plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "no body found", "plot_form_lambda");
    }

    /* check all subforms are symbols */
    for( arg=args; arg->type != plot_type_null; arg = cdr(arg) ){
        if( car(arg)->type != plot_type_symbol ){
            return plot_runtime_error(plot_error_internal, "invalid parameter, not of type plot_type_symbol", "plot_form_define");
        }
    }

    return plot_new_lambda(env, sexpr);
}

/* (if cond if-expr else-expr) -syntax
 * (if cond if-expr)
 */
struct plot_value * plot_form_if(struct plot_env *env, struct plot_value *sexpr){
    plot_value *value;
    plot_value *cond;
    plot_value *if_expr;
    plot_value *else_expr = 0;

    /* scheme if's can have 2 forms
     * (if cond if-expr) ; 'guard'
     * (if cond if-expr else-expr) ; 'branching'
     *
     * when we receive it will be either of
     *  (cond if-expr)
     *  (cond if-expr else-expr)
     *
     */

    cond = car(sexpr);
    if_expr = car(cdr(sexpr));
    else_expr = cdr(cdr(sexpr));
    if( else_expr->type == plot_type_pair ) {
        else_expr = car(else_expr);
    } else {
        /* null */
        else_expr = 0;
    }

    /* decr is handled in plot_truthy */
    value = plot_eval_expr(env, cond);
    if( ! value ){
        #if DEBUG_FORM || DEBUG
        puts("\teval of if condition returned NULL");
        #endif
        return 0; /* FIXME ERROR */
    }
    if( value->type == plot_type_error ){
        puts("plot_eval_form (if cond)");
        return value;
    }
    if( plot_truthy(value) ){
        plot_value_decr(value);
        value = plot_eval_expr(env, if_expr);
        if( ! value ){
            #if DEBUG_FORM || DEBUG
            puts("\teval of if true branch returned NULL");
            #endif
            return 0; /* FIXME ERROR */
        }
        if( value->type == plot_type_error ){
            puts("plot_eval_form (if if-expr)");
            return value;
        }
    } else if( else_expr ){ /* (if cond if-expr else-expr) */
        plot_value_decr(value);
        value = plot_eval_expr(env, else_expr);
        if( ! value ){
            #if DEBUG_FORM || DEBUG
            puts("\teval of if false branch returned NULL");
            #endif
            return 0; /* FIXME ERROR */
        }
        if( value->type == plot_type_error ){
            puts("plot_eval_form (if else-expr)");
            return value;
        }
    } else {
        plot_value_decr(value);
        /* (display (if #f "hello")) ;; => unspecified
         * in csi this is 'unspecified'
         * in racket (lang scheme) there is no output
         *
         * r5rs says: (4.1.5 page 10)
         *  "if the <test> yields a false value and no <alternate>
         *  is spcified, then the result of the expression is
         *  unspecified"
         *
         * r6rs says: (11.4.3 page 33)
         *  "if the <test> yields #f and no <alternate> is
         *  specified, then the result of the expression is
         *  unspecified"
         *
         * r7rs says: (4.1.5 page 13)
         *  "if the <test> yields a false value and no <alternative>
         *  if specified, then the result of teh expression is
         *  unspecified"
         */
        return plot_new_unspecified(); /* success */
    }

    return value;
}

/* (cond (<test> <expression>)...) -syntax
 *
 * (cond ((> 3 2) 'greater)
 *       ((< 3 2) => 'less)
 *       (else 'equal))
 *
 * a cond is a set of <test> <expression> pairs which are tried in order:
 *   if a <test> results in #t (or a truthy value) then expression is eval-ed
 *     and the result of the cond expression is the result of this expression.
 *
 *   else is always true.
 *
 *   if all <test>s are tried and found to be false then the value of the cond
 *     is unspecified.
 *
 *   possible forms:
 *      ((< 3 2) 'greater)
 *      (else 'equal)
 *      ((> 3 2) => 'less)
 *
 */
struct plot_value * plot_form_cond(struct plot_env *env, struct plot_value *sexpr){
    /* current cursor into args */
    plot_value *cur;
    /* result from an eval call */
    plot_value *res;
    /* temporary value used in testing and eval */
    plot_value *tmp;

    if( sexpr->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected at least 1 cond clause", "plot_form_cond");
    }

    for( cur = sexpr; cur->type == plot_type_pair; cur = cdr(cur) ){
        if( cur->type != plot_type_pair ){
            return plot_runtime_error(plot_error_bad_args, "expected cond clause", "plot_form_cond");
        }

        if( car(cur)->type != plot_type_pair ){
            return plot_runtime_error(plot_error_bad_args, "expected cond clause", "plot_form_cond");
        }

        /* assign test position to tmp */
        tmp = car(car(cur));

        /* if tmp is 'else' then it is considered true */
        if( tmp->type == plot_type_symbol && !strcmp("else", tmp->u.symbol.val) ){
            /* NB: plot_eval_truthy will handle our decr for us */
            res = plot_new_boolean(1);
        } else {
            /* otherwise we eval */
            /* NB: plot_eval_truthy will handle our decr for us */
            res = plot_eval_expr(env, tmp);
        }

        /* test for truthyness
         * plot_eval_truthy will handle our decr for us
         */
        if( plot_truthy(res) ){
            if( cdr(car(cur))->type != plot_type_pair ){
                return plot_runtime_error(plot_error_bad_args, "expected cond expression, none found", "plot_form_cond");
            }

            /* assign expression to tmp */
            tmp = car(cdr(car(cur)));

            /* if this is `=>` then the next element is the expression */
            if( tmp->type == plot_type_symbol && !strcmp("=>", tmp->u.symbol.val) ){
                if( cdr(cdr(car(cur)))->type != plot_type_pair ){
                    return plot_runtime_error(plot_error_bad_args, "expected cond expression, none found in => cond clause", "plot_form_cond");
                }
                tmp = car(cdr(cdr(car(cur))));
            }

            res = plot_eval_expr(env, tmp);
            return res;
        }
    }

    return plot_new_unspecified();
}


/* (set! variable value) -syntax(set! variable value)
 */
struct plot_value * plot_form_set(struct plot_env *env, struct plot_value *sexpr){
    plot_value *value;
    plot_value *name, *expr;

    /* (set! variable expr)
     * car = symbol set!
     * cdr =
     *  car = symbol <variable>
     *  cdr =
     *      car = <expr>
     *      cdr = null
     */

    name = car(sexpr);
    expr = car(cdr(sexpr));

    if( name->type != plot_type_symbol ||
        cdr(cdr(sexpr))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "malformed set! form", "plot_form_set");
    }

    value = plot_eval_expr(env, expr);
    if( ! value ){
        #if DEBUG_FORM || DEBUG
        puts("\teval of set value returned NULL");
        #endif
        return 0; /* FIXME ERROR */
    }
    if( value->type == plot_type_error ){
        puts("plot_eval_form (set!");
        return value;
    }

    if( ! plot_env_set(env, &(name->u.symbol), value) ){
        puts("\tset! call to plot_env_set failed");
        return 0; /* FIXME ERROR */
    }

    return plot_new_unspecified();
}

struct plot_value * plot_form_quote(struct plot_env *env, struct plot_value *sexpr){
    /* current position in input */
    plot_value *in;
    /* output */
    plot_value *out, **cur;


    #if DEBUG_FORM || DEBUG
    puts("quote of sexpr:");
    display_error_expr(sexpr);
    #endif

    out = 0;
    cur = &out;

    switch( car(sexpr)->type ){
        case plot_type_pair:
            /* return map quote cdr(sexpr);
             * '(a b c) => (list 'a 'b 'c)
             */
            for( in=car(sexpr); in->type == plot_type_pair; in = cdr(in) ){
                *cur = plot_new_pair( 0, plot_new_null() );
                car(*cur) = plot_form_quote( env,  plot_new_pair( car(in), plot_new_null() ) );
                cur = &cdr(*cur);
            }
            return out;
            break;
        default:
            return car(sexpr);
            break;
    }

    plot_fatal_error("Impossible expression type given to plot_form_quote");
    return 0;
}

/* (delay expr) -syntax
 */
struct plot_value * plot_form_delay(struct plot_env *env, struct plot_value *sexpr){
    plot_value *val;
    /* delay will have
     * car = symbol delay
     * cdr =
     *  car = expr
     *  cdr = null
     */

    if( cdr(sexpr)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "malformed delay expression", "plot_form_delay");
    }

    val = plot_new_promise(env, car(sexpr));
    return val;
}


