(define port (open-output-file "test"))

(display "hello")
(newline)
(display "hello" port)
(newline port)

(write-char #\w)
(newline)
(write-char #\w port)
(newline port)

(write-string "orld")
(newline)
(write-string "or" port)
(write-string "ldAAA" port 0 2)
(newline port)

(close-port port)
