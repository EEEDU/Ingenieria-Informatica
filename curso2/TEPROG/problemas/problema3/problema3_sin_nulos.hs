-- Opcion 2
-- Almacenar sólo los coeficientes del polinomio no nulos, sabiendo a qué término corresponden

newtype Polynomial = Polynomial [(Double, Int)]
    deriving (Show)

-- peval :: Polynomial −> Double −> Double
peval (Polynomial cs) x = sum [c * x^e | (c, e) <- cs]

-- pderv :: Polynomial -> Polynomial
pderv (Polynomial cs) = Polynomial [(c * fromIntegral e, e-1) | (c, e) <- cs, e > 0]


-- padd :: Polynomial -> Polynomial -> Polynomial
padd (Polynomial cs1) (Polynomial cs2) = Polynomial (filter nonZero (addCoefs cs1 cs2))
    where
        nonZero (c, _) = c /= 0.0

        addCoefs [] ys = ys
        addCoefs xs [] = xs
        addCoefs ((c1,e1):xs) ((c2,e2):ys)
            | e1 == e2  = (c1+c2, e1) : addCoefs xs ys
            | e1 <  e2  = (c1,e1)     : addCoefs xs ((c2,e2):ys)
            | otherwise = (c2,e2)     : addCoefs ((c1,e1):xs) ys