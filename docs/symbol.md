Symbols
=======

Literals
--------
A plot symbol can be made up from the following characters:
    a-z (97 - 122)
    A-Z (65 - 90)
    0-9 (48 - 57)
    !   (33)
    $   (36)
    %   (37)
    &   (38)
    *   (42)
    +   (43)
    -   (45)
    .   (46)
    /   (47)
    :   (58)
    <   (60)
    =   (61)
    >   (62)
    ?   (63)
    @   (64)
    ^   (94)
    _   (95)
    ~   (126)

A plot symbol must start with a prefix that distinguishes it from any literal values.

All symbols and forms in plot are case sensitive.

Some examples of valid identifiers (subset of those presented in r7rs section 2.1):
    ...
    +soup+
    <=?
    ->string
    the-word-recusion-has-many-meanings
    V17a


Hashing
-------
Internally plot hashes symbols at various times and stores it in the plot_symbol (.hash).

This hash is used when looking up symbols to avoid expensive strcmps.

These hashes are usually attached at parse time, however if a symbol is lacking a hash at runtime
it will be hashed automatically, a `hash` value of `0` means this symbol has not yet been hashed.

