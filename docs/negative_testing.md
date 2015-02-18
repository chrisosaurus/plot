negative testing:
----------------

created directory t/integration/negative for negative unit tests

need infrastructure to run those tests and confirm they error and what kind
of error they give

the idea being that

chris@Ox1b plot(master)-> cat t/integration/negative/lambda_too_many.scm
(display ((lambda (x) (+ x 5)) 11 12 13))

meaning

    ./plot t/integration/negative/lambda_too_many.scm 

should always issue

    Error encountered in 'plot_eval_form LAMBDA':
        error message: 'too many args supplied to lambda'
        error type: 'runtime error in source program'


we should have testing to confirm that this message is triggered correctly in this case

that is to say test both the negative and positive expectations

currently only testing positive

