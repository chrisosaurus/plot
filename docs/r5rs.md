List of r5rs sections and notes on implementation progress and any deviations from standard.

2.2 comments
    `;` comments are implemented

3.5 Proper tail recursion
    tail recursion is not implemented but is planned.

4.1.2 Literal expressions
    literals are implemented for all implemented types:
        character
        string
        boolean
        number (exact positive integer)
        symbol (although there is not yet quoting)
        function (lambda form)
4.1.4 Procedures
    `lambda` form is implemented

4.1.5 Conditionals
    both forms of `if` are implemented
    `(if cond if-expr)`
    `(if cond if-expr else-expr)`

4.1.6 Assignments
    `set!` form is implemented.


4.2.1 Derived conditionals
    the following procedures are implemented:
        `and`
        `or`

4.2.2 Binding constructs
    no derived binding construct are currently implemented.

4.3 Macros
    plot does not yet have a macro system.

5.2 Definitions
    `define` form is implemented.

5.3 Syntax definitions
    plot does not yet have syntax definitions.

6.1 Equivalence predicates
    `eq?` is not implemented.
    `eqv?` is not implemented.
    `equal?` is implemented.

6.2 Numbers
    exact integers are implemented although
    there is not yet support for literal negative integers
    so you must use `(- 0 1)` in lieu of `-1`.

    currently in plot `integer` is synonymous with `number`
    `number?` is implemented, all other number-type-testing
    procedures are not currently implemented.

    complex, real and rational are not currently implemented.

    the following numerical comparison procedures are implemented:
        `=`
        `<`
        `>`
        `<=`
        `>=`

    the following numerical arithmetic procedures are implemented:
        `+`
        `-`
        `*`
        `/`
        `remainder`

6.3.1 booleans
    this section is fully implemented.

    booleans are implemented, literals `#t` and `#f`.
    plot has the boolean-type testing procedures `boolean?`
    `not` is implemented.

6.3.2 pairs and lists
    pairs and lists are implemented.
    plot currently lacks quoting, so literal pairs and lists are not yet possible.

    implemented procedures from this section:
        `pair?`
        `cons`
        `car`
        `cdr`
        `set-car!`
        `set-cdr!`

    pending implementation:
        `list`
        `null?`
        `list?`
        `length`
        `append`
        `reverse`

6.3.3 symbols
    symbols are implemented along with the testing procedures `symbol?`,
    note however that quoting is not yet implemented and thus symbols
    are currently always resolved via the env so `symbol?` can never return true.

6.3.4 characters
    this section is completely implemented.

    plot deviates mildly from the standard as 'special' literals `#\space` and
    `#\newline` are case sensitive, whereas 6.3.4 defines them to be case insensitive;
    this means that plow will not behave correctly for `#\NEWLINE`.

    character literals are implemented
        `#\a` through to `#\Z`
        `#\0` through to `#\9`
        symbols such as `#\(`
        'special' literals `#\space` and `#\newline`
        space can also be written as `#\ `

    the following character procedures are implemented:
        `char?`
        `char=?`
        `char-ci=?`
        `char<?`
        `char>?`
        `char<=?`
        `char>=?`
        `char-ci<?`
        `char-ci>?`
        `char-ci<=?`
        `char-ci>=?`
        `char-alphabetic?`
        `char-numeric?`
        `char-whitespace?`
        `char-upper-case?`
        `char-lower-case?`
        `char->integer`
        `integer->char`
        `char-upcase`
        `char-downcase`

6.3.5 Strings
    string literals are implemented
        `"the word recursion has many meanings"`

    escaping is partially implemented but still requires work.

    the following string procedures are implemented:
        `string?`
        `string-length`
        `substring`
        `string-append`
        `string-copy`
        `string=?`
        `string-ci=?`
        `make-string`
        `string`
        `string-ref`
        `string-set!`
        `string-fill!`
        `string<?`
        `string>?`
        `string<=?`
        `string>=?`

    the following procedures are pending implementation
    (exposed to runtime but not yet callable):
        `string-ci<?`
        `string-ci>?`
        `string-ci<=?`
        `string-ci>=?`
        `string->list`
        `list->string`

6.3.6 Vectors
    vectors are not currently implemented.

6.4 Control features
    `procedure?` is implemented.
    all other procedures from this section are not implemented.

6.5 Eval
    eval is not currently exposed to plot programs.

6.6 Input and output
    ports are not currently implemented.

6.6.3 Output
    the following output procedures are implemented:
        `display`
        `newline`

