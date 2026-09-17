-- Opcion 1:
-- Almacenar todos los coeficientes del polinomio (ceros incluidos).

newtype Polynomial = Polynomial [Double]
    deriving (Show)

-- peval :: Polynomial −> Double −> Double
peval (Polynomial cs) x = foldr (\c acc -> c + x * acc) 0.0 cs

-- pderv :: Polynomial -> Polynomial
pderv (Polynomial cs) = Polynomial (zipWith (*) [1..] (tail cs))

-- padd :: Polynomial -> Polynomial -> Polynomial
padd (Polynomial cs1) (Polynomial cs2) = Polynomial (addCoefs cs1 cs2)
    where
        addCoefs [] ys         = ys
        addCoefs xs []         = xs
        addCoefs (x:xs) (y:ys) = (x + y) : addCoefs xs ys
