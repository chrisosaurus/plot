List of r5rs sections and notes on implementation progress and any deviations from standard.

2.2
    `;` comments are implemented

3.5
    tail recursion is not implemented but is planned.

4.1.2
    literals are implemented for all implemented types:
        character
        string
        boolean
        number (exact positive integer)
        symbol (although there is not yet quoting)
        function (lambda form)
4.1.4
    `lambda` form is implemented

4.1.5
    both forms of `if` are implemented
    `(if cond if-expr)`
    `(if cond if-expr else-expr)`

4.1.6
    `set!` form is implemented.


4.2.1
    the following procedures are implemented:
        `and`
        `or`

4.2.2
    no derived binding construct are currently implemented.

4.3
    plot does not yet have a macro system.

5.2
    `define` form is implemented.

5.3
    plot does not yet have syntax definitions.

6.1
    `eq?` is not implemented.
    `eqv?` is not implemented.
    `equal?` is implemented.

6.2
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

    the following numerical arithmetic procedures are impelmented:
        `+`
        `-`
        `*`
        `/`
        `remainder`

6.3.1
    booleans are implemented, literals `#t` and `#f`.
    plot has the boolean-type testing procedures `boolean?`
    `not` is implemented.

6.3.2
    pairs and lists are currently not implemented.

6.3.3
    symbols are implemented along with the testing procedures `symbol?`,
    note however that quoting is not yet implemented and thus symbols
    are currently always resolved via the env so `symbol?` can never return true.

6.3.4
    this section is completely implemented.

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

6.3.5
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

6.3.6
    vectors are not currently implemented.

6.4
    `procedure?` is implemented.
    all other procedures from this section are not implemented.

6.5
    eval is not currently exposed to plot programs.

6.6
    ports are not currently implemented.

6.6.3
    the following output procedures are implemented:
        `display`
        `newline`

