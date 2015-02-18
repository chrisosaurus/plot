
;; not so much a bug as a missing feature
;; currently triggers unimplemented error
;; r7rs page 13 section 4.1.4 says:
;; ((lambda x x) 3 4 5 6) => (3 4 5 6)
(display ((lambda foo foo) 1 2 3))


