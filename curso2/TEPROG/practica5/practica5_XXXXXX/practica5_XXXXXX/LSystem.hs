module LSystem where

import Turtle
import SVG
import Data.Char (isUpper, isLower)


-- ============================================================
-- TAREA 1: tplot (ya implementada)
-- ============================================================

-- Convierte un carácter en un Move
charToMove :: Char -> Maybe Move
charToMove '>' = Just Forward
charToMove '+' = Just TurnRight
charToMove '-' = Just TurnLeft
charToMove _   = Nothing

-- Extrae la posición de la tortuga
getPosition :: Turtle -> Position
getPosition (_,_,pos,_) = pos

-- Genera la lista de posiciones a partir de la tortuga y una secuencia de comandos
tplot :: Turtle -> String -> [Position]
tplot t []     = [getPosition t]
tplot t (c:cs) = case charToMove c of
    Just move -> getPosition t : tplot (moveTurtle t move) cs
    Nothing   -> tplot t cs  -- ignora caracteres desconocidos


-- ============================================================
-- TAREA 2: L-Systems
-- ============================================================

-- Aplica las reglas a un único carácter
applyRules :: (Char -> String) -> Char -> String
applyRules rules c = rules c

-- Aplica las reglas a toda la cadena (una iteración)
step :: (Char -> String) -> String -> String
step rules = concatMap (applyRules rules)

-- Aplica las reglas n veces partiendo del axioma
lsystem :: (Char -> String) -> String -> Int -> String
lsystem rules axiom 0 = axiom
lsystem rules axiom n = lsystem rules (step rules axiom) (n-1)