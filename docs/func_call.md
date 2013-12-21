a call to a function looks like

    (func arg1 args ...)

where func is one of:

* lambda - scheme function
* form - c function
* syntactic form - c function which takes args pre-evaluation

all 3 of these will shortly be handled by `(apply proc args)` which will not perform
further evaluation on it's `args`, therefore for either `lambda` or `form` the caller
must first evaluate the args before calling `apply`.

