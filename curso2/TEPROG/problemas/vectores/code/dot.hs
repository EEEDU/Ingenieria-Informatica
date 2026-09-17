module Main where

import Data.List

-- Solucion 1: recursividad
dot :: Num a => [a] -> [a] -> a
dot [] [] = 0
dot (x:xs) (y:ys) = (x * y) + dot xs ys
dot _ _ = error "Vectors of different size"

-- Solucion 2: sum, map y zip
dot' :: Num a => [a] -> [a] -> a
dot' xs ys = sum (map (\(x,y) -> x*y) (zip xs ys) )

-- Solucion 3: sum y zipWith
dot'' :: Num a => [a] -> [a] -> a
dot'' xs ys = sum (zipWith (*) xs ys)

-- Solucion 4: list comprension y sum
dot''' :: Num a => [a] -> [a] -> a
dot''' xs ys = sum [x*y | (x,y) <- zip xs ys]


-- Solucion 5: sum, zipWith, composicion
dot'''' :: Num a => [a] -> [a] -> a
dot'''' = curry (sum . (uncurry (zipWith (*))))

-- Matriz por vector
mulmv  :: Num a => [[a]] -> [a] -> [a]
mulmv m v = map (dot v) m

-- Matriz por matriz
mulmm  :: Num a => [[a]] -> [[a]] -> [[a]]
mulmm m1 m2 = transpose (map (mulmv m1) (transpose m2))

main = do
  print $ dot     [1,2,3] [4,5,6]
  print $ dot'    [1,2,3] [4,5,6]
  print $ dot''   [1,2,3] [4,5,6]
  print $ dot'''  [1,2,3] [4,5,6]
  print $ dot'''' [1,2,3] [4,5,6]
