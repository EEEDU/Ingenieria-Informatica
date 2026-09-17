import LSystem
import SVG
import Turtle

-- CUADRADO: paso=1, giro=90°
cuadrado :: [Position]
cuadrado = tplot (1, 90, (0,0), 90) ">+>+>+>+"

-- TRIÁNGULO EQUILÁTERO: paso=1, giro=120°
triangulo :: [Position]
triangulo = tplot (1, 120, (0,0), 90) ">+>+>+"

-- CÍRCULO aproximado: 360 pasos de 1°
circulo :: [Position]
circulo = tplot (0.01745, 1, (0,0), 90) (concat (replicate 360 ">+"))

-- ============================================================
-- REGLAS DE REESCRITURA
-- ============================================================

-- Sierpinsky Arrowhead
rulesArrowhead :: Char -> String
rulesArrowhead 'F' = "G+F+G"
rulesArrowhead 'G' = "F-G-F"
rulesArrowhead  c  = [c]       -- cualquier otro símbolo se conserva

-- Copo de nieve de Koch
rulesKoch :: Char -> String
rulesKoch 'F' = "F+F--F+F"
rulesKoch  c  = [c]

-- Curva de Dragon
rulesDragon :: Char -> String
rulesDragon 'F' = "F+G"
rulesDragon 'G' = "F-G"
rulesDragon  c  = [c]

-- Cuadrado de Koch
rulesKochSquare :: Char -> String
rulesKochSquare 'F' = "F+F-F-F+F"
rulesKochSquare  c  = [c]

-- ============================================================
-- GENERACIÓN DE FIGURAS
-- ============================================================

arrowhead :: [Position]
arrowhead = tplot (1, 60, (0,0), 0) (lsystem rulesArrowhead "F" 6)

koch :: [Position]
koch = tplot (1, 60, (0,0), 0) (lsystem rulesKoch "F--F--F" 4)

dragon :: [Position]
dragon = tplot (5, 90, (0,0), 0) (lsystem rulesDragon "F" 12)

kochSquare :: [Position]
kochSquare = tplot (1, 90, (0,0), 0) (lsystem rulesKochSquare "F" 4)



main :: IO ()
main = do
    savesvg "cuadrado"  cuadrado
    savesvg "triangulo" triangulo
    savesvg "circulo"   circulo
    savesvg "arrowhead"  arrowhead
    savesvg "koch"       koch
    savesvg "dragon"     dragon
    savesvg "kochSquare" kochSquare    
    putStrLn "Figuras generadas!"


