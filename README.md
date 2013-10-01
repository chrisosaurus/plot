plot
====
Interpreter for a small subset of scheme - not to be taken seriously

I am writing plot mostly for the sake of playing around with implementing a language,
I am specifically interesting in garbage collection and memory allocation.

Please note that I will most likely not be accepting pull requests as this project is primarily for my learning.
I will also be force pushing here often, so no getting upset.

naming
-------

scheme - Noun; A large-scale systematic plan or arrangement for attaining some particular object or putting a particular idea into effect.

plot - Noun; A plan made in secret by a group of people to do something illegal or harmful

work
----
The initial plan was to try to define a very minimal core language (in c) and implement the rest in plot itself.

planned:
* ref counting
* define
* lambda
* quote
* strings and basic ops
* numbers and basic ops
* eq? and equal?
* if and cond

dependencies
------------
* libc
* lib check is used for unit testing, in debian derived system this is simply 'check'

installing
----------
The closest we have to 'installing' is running `make test`, that is to say that there is currently no front-end.

license
---------
Plot is released under the terms of the MIT License

