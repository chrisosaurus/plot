
;; manual currying
(define (fbuilder func . dargs)
  ;; list of args we have collected so far
  (define args dargs)

  ;; call the function with the collected args
  (define (call)
    (func args))

  ;; append some more arguments
  (define (app . moreargs)
    (set! args (append args moreargs)))

  ;; pretty error display
  (define (error ins)
    (newline)
    (display "fbuilder: unsupported command: '")
    (display ins)
    (display "'")
    (newline))

  ;; our dispatcher
  (lambda (ins)
    (cond
      [(eq? ins 'add)    app]
      [(eq? ins 'app)    app]
      [(eq? ins 'append) app]
      [(eq? ins 'call)   (call)]
      [else              (error ins)])))

;; define a toy function taking a list of args
(define (dn . args)
  (map display args))

;; build our function
(define f (fbuilder dn 1 2))
;; add some more ags
((f 'add) 3)
((f 'add) 4 5 6 7)
;; finally call our function
(f 'call)

;; test error display
(f 'illegal-command-goes-here)

