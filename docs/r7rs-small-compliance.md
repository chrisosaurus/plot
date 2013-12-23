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
        `cond`

    the following forms should be implemented shortly:
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
    The following forms and procedures are implemented:
        `delay`
        `force`
        `promise?`
        `make-promise`

    The following are currently unimplemented
        `delay-force`

4.2.9 Case-lambda
    `case-lambda` support should be implemented soon.

4.3 Macros
    plot does not yet have a macro system.

5.2 Import declarations
    library support is currently pending implementation.

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
    library support is currently pending implementation.

6.1 Equivalence predicates
    `eq?`, `eqv?` and `equal?` are implemented.
    All 3 are currently the same function and all test for 'deep equality' (`equal?`).
    This may be changed later to be more inline with the spec, but in it's current form
    this is (as far as I know) standard-compliant.

    All 3 forms should work for all currently implemented types.

6.2 Numbers
    exact integers are implemented although
    there is not yet support for literal negative integers
    so you must use `(- 0 1)` in lieu of `-1`.

    currently in plot `integer` is synonymous with `number`
    `number?` and `exact?` are implemented, all other number-type-testing
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
    pair and list equality is implemented.

    All procedures from this section are implemented:
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
    `symbol=?` is implemented.

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
        `list->string`

    the following procedures are pending implementation
    (exposed to runtime but not yet callable):
        `string-ci<?`
        `string-ci>?`
        `string-ci<=?`
        `string-ci>=?`

6.8 Vectors
    Vectors are not currently implemented.

6.9 Bytevectors
    Bytevectors are not currently implemented.

6.10 Control features
    The following procedures are implemented:
        `procedure?`
        `apply`
        `map`
        `for-each`

6.11 Exceptions
    Plot has the beginnings of an exception system, however it is not
    currently exposed to plot programs.

6.12 Environment and evaluation
    `eval` is not currently exposed to plot programs.

6.13 Input and output
    Plot's current support for I/O is rather lousy.

6.13.1 Ports
    Textual ports are currently being implemented.

    The following procedures are implemented:
        `open-output-port`
        `open-input-file`
        `close-port`
        `input-port?`
        `output-port?`
        `port?`
        `textual-port?`
        `binary-port?`
        `input-port-open?`
        `output-port-open?`

    the following procedures are not yet implemented:
        `call-with-port`
        `call-with-input-file`
        `call-with-output-file`
        `current-input-port`
        `current-output-port`
        `current-error-port`
        `with-input-from-file`
        `with-output-to-file`
        `open-binary-input-file`
        `open-binary-output-file`
        `close-input-port`
        `close-output-port`
        `open-input-string`
        `open-output-string`
        `get-output-string`
        `open-input-bytevector`
        `open-output-bytevector`
        `get-output-bytevector`

6.13.2 Input
    the following input procedures are implemented:
        `eof-object`
        `eof-object?`

    the following procedures are pending implementation:
        `read-string`
        `read-line`
        `read-char`
        `peek-char`
        `char-ready?`

    the following procedures are not yet implemented:
        `read`
        `read-u8`
        `peek-u8`
        `u8-ready?`
        `read-bytevector`
        `read-bytevector!`

6.13.3 Output
    the following output procedures are implemented:
        `display`
        `newline`
        `write-char`
        `write-string`

    the following procedures are not yet implemented:
        `write`
        `write-shared`
        `write-simple`
        `write-u8`
        `write-bytevector`
        `flush-output-port`

6.14 System interface
    plot has both `exit` and `emergency-exit`, but as plot currently
    lacks dynamic-wind they are both the same.

    `(exit)`,`(exit #t)` and `(exit 0)` will exit 0 (success)
    any other value will exit 1 (failure)

    the following procedures are not yet implemented:
        `load`
        `file-exists?`
        `delete-file`
        `command-line`
        `get-environmental-variable`
        `get-environmental-variables`
        `current-second`
        `current-jiffy`
        `jiffies-per-second`
        `features`


