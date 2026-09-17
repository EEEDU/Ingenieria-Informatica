module Main where

-- Solucion 1: función auxiliar

pnorm   :: Floating a => Int -> [a] -> a
pnorm   p = powrev (1.0/fp) . sum . map (powrev fp . abs)
  where
    fp = fromIntegral p
    powrev e b = b**e

-- Solucion 2: ** + flip
pnorm'  :: Floating a => Int -> [a] -> a
pnorm'  p = ((flip (**)) (1.0/fp)) . sum . map ((flip (**)) fp . abs)
  where
    fp = fromIntegral p

-- Solucion 3: sección (** n)
pnorm'' :: Floating a => Int -> [a] -> a
pnorm'' p = (** (1.0/fp)) . sum . map ((** (fp)) . abs)
  where
    fp = fromIntegral p

mydata = [1.0,2.0,3.0,4.0,5.0]

main = do
  print (pnorm   2 mydata)
  print (pnorm'  2 mydata)
  print (pnorm'' 2 mydata)
  print (pnorm   3 mydata)
  print (pnorm'  3 mydata)
  print (pnorm'' 3 mydata)
