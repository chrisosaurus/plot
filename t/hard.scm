;; some basic unit testing for plot in plot

;; I do love my new lines
(define println
  (lambda (s) ;; an inline comment :o
    (display s)
    (newline)))


;; test define, if and simple comparison functions
(define one
  (if (< 4 5)
    1
    0))

(if (= one 1)
  (println "pass one")
  (println "fail one"))


;; test define, if and more complex comparison functions
(define two
  (if (>= 10 8 6 4 2 0)
    (if (<= 0 2 4 6 8 10)
      1
      0)
    0))

(if (= two 1)
  (println "pass two")
  (println "fail two"))


;; testing nested lambda form
(define three
  (lambda ()
    (lambda ()
      5)))

(if (procedure? three)
  (if (procedure? (three))
    (if (number? ((three)))
      (if (= 5 ((three)))
        (println "pass three")
        (println "fail three: case 4"))
      (println "fail three: case 3"))
    (println "fail three: case 2"))
  (println "fail three: case 1"))


;; test value testing functions
(if (string? "I am a string")
  (println "pass four")
  (println "fail four"))

(if (number? 24)
  (println "pass five")
  (println "fail five"))

(if (procedure? display)
  (println "pass six")
  (println "fail six"))

(if (boolean? #f)
  (println "pass seven")
  (println "fail seven"))


;; testing define and scope
(define eight 1)
(define eight-f
  (lambda ()
    (define eight 2)
    eight))

(if (= eight 1)
  (if (= (eight-f) 2)
    (println "pass eight")
    (println "fail eight: case 2"))
  (println "fail eight: case 1"))


(define nine 1)
(define nine-f
  (lambda ()
    nine))
(define nine 2)

(if (= nine 2)
  (if (= (nine-f) 2)
    (println "pass nine")
    (println "fail nine case 2"))
  (println "fail nine case 1"))


