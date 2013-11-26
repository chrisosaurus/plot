List of r7rs-small (WG1) sections and notes on implementation progress and any deviations from standard.

2.2 comments
    `;` comments are implemented

3.5 Proper tail recursion
    tail recursion is not implemented but is planned.

4.1.2 Literal expressions
    quoting is implemented, both (quote datum) and 'datum

    literals are implemented for the following types:
        character
        string
        boolean
        number (exact positive integer, no negative literals yet)
        symbol
        function (lambda form)
        lists

    literals for the following types are pending implementation:
        pairs (improper lists)

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

    the following forms should be implemented shortly:
        `cond`
        `case`
        `when`
        `unless`

4.2.2 Binding constructs
    no derived binding construct are currently implemented.

4.2.3 Sequencing
    plot supports the second form of begin
        `(begin <expr1> <expr2> ... )`

    more thought has to be given to the first form
        `(begin <expression or definition> ... )`

4.2.5 Delayed evaluation
    Both `delay` and `force` are implemented.

    The following is currently unimplemented
        `delay-force`

4.2.9 Case-lambda
    `case-lambda` support should be implemented soon.

4.3 Macros
    plot does not yet have a macro system.

5.2 Import declarations
    plot has short-term plans to include an import form.

5.3 Variable definitions
    `define` form is implemented.

    `define` has 2 different forms:
        (define a
            (lambda (x)
                (+ x 1)))

        (define (a x)
            (+ x 1))

5.4 Syntax definitions
    plot does not yet have syntax definitions.

5.6 Libraries
    plot has short-term plans to include library support.

6.1 Equivalence predicates
    `eq?`, `eqv?` and `equal?` are implemented.
    All 3 are currently the same function and all test for 'deep equality' (`equal?`).
    This may be changed later to be more inline with the spec, but in it's current form
    this is (as far as I know) standard-compliant.

    All 3 forms should work for all currently implemented types except for 
    pairs and lists.

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

6.3 Booleans
    this section is fully implemented.

    booleans are implemented, literals `#t` and `#f`.
    plot has the boolean-type testing procedures `boolean?`
    `not` is implemented.

6.4 Pairs and lists
    pairs and lists are implemented,
    lists literals are implemented.
    pair (improper list) literals are pending implementation.
    pair and list equality is currently not implemented (`eq` family will throw a plot_runtime_error).

    implemented procedures from this section:
        `pair?`
        `cons`
        `car`
        `cdr`
        `set-car!`
        `set-cdr!`
        `list`
        `list?`
        `null?`
        `length`
        `reverse`
        `make-list`
        `list-copy`
        `append`
        `list-set!`
        `list-ref`

    pending implementation:
        `list-tail`
        `memq`
        `memv`
        `member`
        `assq`
        `assv`
        `assoc`

6.5 Symbols
    symbols are implemented along with the testing procedures `symbol?`,
    literal symbols are supported via quoting `'symbol`.

6.6 Characters
    The subset of this section that is in r5rs is implemented.

    Plot deviates mildly from the standard as 'special' literals `#\space` and
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

6.7 Strings
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
        `string->list`

    the following procedures are pending implementation
    (exposed to runtime but not yet callable):
        `string-ci<?`
        `string-ci>?`
        `string-ci<=?`
        `string-ci>=?`
        `list->string`

6.8 Vectors
    Vectors are not currently implemented.

6.9 Bytevectors
    Bytevectors are not currently implemented.

6.10 Control features
    `procedure?` is implemented.
    all other procedures from this section are not implemented.
    `apply` is implemented but is not yet fully r7rs-compliant.

    the following are pending implementation:
        `map
        `for-each`

6.11 Exceptions
    Plot has the beginnings of an exception system, however it is not
    currently exposed to plot programs.

6.12 Environment and evaluation
    `eval` is not currently exposed to plot programs.

6.13 Input and output
    Plot's current support for I/O is rather lousy.

6.13.1 Ports
    Ports are not currently implemented.

6.13.2 Input
    Plot does not yet have any input procedures.

6.13.3 Output
    the following output procedures are implemented:
        `display`
        `newline`

6.14 System interface
    plot has both `exit` and `emergency-exit`, but as plot currently
    lacks dynamic-wind they are both the same.

    `(exit)`,`(exit #t)` and `(exit 0)` will exit 0 (success)
    any other value will exit 1 (failure)


