# type-inference

A Hindley-Milner(-Damas) type inference implementation in C.

Current output:

    ((+ 2) 3) : int
    ((* 2) ((+ 2) 2)) : int
    ((> 2) 3) : bool
    ((* 5) ((< 7) 11)) : type mismatch: bool != int
    (let bar = (fun x -> (fun y -> ((+ (strlen x)) y))) in (bar "fubar")) : (int -> int)
    ((pair 11) "13") : (int * string)
    (((cond true) "yes") "no") : string
    (let sum = (fun y -> (fun x -> ((+ x) y))) in ((sum 2) 3)) : int
    (letrec factorial = (fun n -> (((cond (zero n)) 1) ((* n) (factorial (pred n))))) in (factorial 5)) : int
    (fun x -> ((pair (x 3)) (x true))) : type mismatch: bool != int
    ((pair (f 4)) (f true)) : unknown symbol 'f'
    (let f = (fun x -> x) in ((pair (f 4)) (f true))) : (int * bool)
    (fun f -> (f f)) : recursive unification
    (let g = (fun f -> 5) in (g g)) : int
    (fun g -> (let f = (fun x -> g) in ((pair (f 3)) (f true)))) : (a -> (a * a))
    (fun f -> (fun g -> (fun arg -> (g (f arg))))) : ((b -> c) -> ((c -> d) -> (b -> d)))

