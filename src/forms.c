#include <stdio.h>

#include "forms.h"
#include "value.h"
#include "plot.h"
#include "types.h"
#include "funcs.h"
#include "eval.h"
#include "hash.h"
#include "env.h"

#define DEBUG 0

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

/* (begin body...)
 */
struct plot_value * plot_form_begin(struct plot_env *env, struct plot_sexpr *sexpr){
    int i;
    plot_value *value;

    for( i=0; i< sexpr->nchildren; ++i ){
        value = plot_eval_expr(env, &(sexpr->subforms[i]));
    }
    return value;
}


/* (define what value)
 */
struct plot_value * plot_form_define(struct plot_env *env, struct plot_sexpr *sexpr){
    int i;
    plot_value *value;
    plot_value *name;

    /* define has 2 forms:
     * (define a <value>)
     * (define (b args) <function body>)
     */
    if( sexpr->nchildren < 3 ){
        #if DEBUG_FORM || DEBUG
        printf("\tDEFINE: incorrect number of children '%d'\n", sexpr->nchildren);
        #endif
        return 0; /* FIXME ERROR */
    }

    if( sexpr->subforms[1].type == plot_expr_sexpr ){
        /* function form */
        if( sexpr->subforms[1].u.sexpr.nchildren < 1){
            puts("DEFINE: incorrect function declaration");
            return 0; /* FIXME error */
        }
        if( sexpr->subforms[1].u.sexpr.subforms[0].type != plot_expr_value ){
            puts("DEFINE: invalid function name");
            return 0; /* FIXME error */
        }
        name = sexpr->subforms[1].u.sexpr.subforms[0].u.value;
        /* FIXME hack to get define function form
         * remove declared function name
         *
         * move down the subforms array in order to remove function name
         */

        for( i=0; i< (sexpr->subforms[1].u.sexpr.nchildren - 1); ++i ){
            sexpr->subforms[1].u.sexpr.subforms[i] = sexpr->subforms[1].u.sexpr.subforms[i+1];
        }

        --sexpr->subforms[1].u.sexpr.nchildren;

        value = plot_new_lambda(env, sexpr);
        plot_env_define(env, &(name->u.symbol), value);
        plot_value_decr(value);

    } else if( sexpr->subforms[1].type == plot_expr_value ){
        /* value form */
        name = sexpr->subforms[1].u.value;
        if( name->type != plot_type_symbol ){
            #if DEBUG_FORM || DEBUG
            puts("\tDEFINE: incorrect 1st arg value type");
            #endif
            return 0; /* FIXME ERROR */
        }

        #if DEBUG_FORM || DEBUG
        puts("\tDEFINE: getting value to store");
        #endif

        /* 2nd subform isnt known to be a value ! */
        value = plot_eval_expr(env, &(sexpr->subforms[2]));
        if( ! value ){
            #if DEBUG_FORM || DEBUG
            puts("\tDEFINE: failed to eval_value");
            #endif
            return 0; /* FIXME ERROR */
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

    } else {
        /* error */
        #if DEBUG_FORM || DEBUG
        puts("\tDEFINE: incorrect 1st arg expr type");
        #endif
        return 0; /* FIXME ERROR */
    }

    return plot_new_unspecified();
}

/* (lambda args body...)
 */
struct plot_value * plot_form_lambda(struct plot_env *env, struct plot_sexpr *sexpr){
    int i;

    if( sexpr->nchildren < 3 ){
        #if DEBUG_FORM || DEBUG
        printf("\tLAMBDA: incorrect number of children, need at least 3, got '%d'\n", sexpr->nchildren);
        #endif
        return 0; /* FIXME error */
    }

    /* check 2nd subform is an sexpr (param list) */
    if( sexpr->subforms[1].type != plot_expr_sexpr ){
        puts("\tLAMBDA: param list expected");
        return 0; /* FIXME error */
    }

    /* check all subforms are symbols */
    for( i=0; i< sexpr->subforms[1].u.sexpr.nchildren; ++i ){
        if( sexpr->subforms[1].u.sexpr.subforms[i].type != plot_expr_value ){
            puts("LAMBDA: invalid param type, expected value");
            return 0; /* FIXME error */
        }
        if( sexpr->subforms[1].u.sexpr.subforms[i].u.value->type != plot_type_symbol ){
            puts("LAMBDA: invalid param type, expected symbol");
            return 0; /* FIXME error */
        }
    }

    return plot_new_lambda(env, sexpr);
}

/* (if cond if-expr else-expr)
 * (if cond if-expr)
 */
struct plot_value * plot_form_if(struct plot_env *env, struct plot_sexpr *sexpr){
    plot_value *value;

    /* scheme if's can have 2 forms
     * (if cond if-expr) ; 'guard'
     * (if cond if-expr else-expr) ; 'branching'
     */
    if( sexpr->nchildren != 3 && sexpr->nchildren != 4 ){
        return 0; /* FIXME ERROR */
    }
    /* decr is handled in plot_truthy */
    value = plot_eval_expr(env, &(sexpr->subforms[1]));
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
        value = plot_eval_expr(env, &(sexpr->subforms[2]));
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
    } else if( sexpr->nchildren == 4){ /* (if cond if-expr else-expr) */
        plot_value_decr(value);
        value = plot_eval_expr(env, &(sexpr->subforms[3]));
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

/* (set! variable value)
 */
struct plot_value * plot_form_set(struct plot_env *env, struct plot_sexpr *sexpr){
    plot_value *value;

    if( sexpr->nchildren != 3 ){
        puts("\tset! had incorrect number of arguments");
        return 0; /* FIXME ERROR */
    }

    if( sexpr->subforms[1].type != plot_expr_value || sexpr->subforms[1].u.value->type != plot_type_symbol ){
        puts("\tset! first argument is wrong type (either not value or not symbol)");
        return 0; /* FIXME ERROR */
    }

    value = plot_eval_expr(env, &(sexpr->subforms[2]));
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

    if( ! plot_env_set(env, &(sexpr->subforms[1].u.value->u.symbol), value) ){
        puts("\tset! call to plot_env_set failed");
        return 0; /* FIXME ERROR */
    }

    return plot_new_unspecified();
}



