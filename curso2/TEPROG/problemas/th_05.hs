-- EJERCICIOS DE CONSTRUCTORES

--------------------------------------------------
-- 2. Legal o ilegal
--------------------------------------------------

data Persona = Pers String Int

a = Pers "Ana" 20          -- 
b = Persona "Luis" 30      -- 
c = Pers 10 "Mario"        -- 

d :: Persona
d = Pers "Laura" 25        -- 

--------------------------------------------------
-- 3. Completa el código
--------------------------------------------------

data Libro = LibroConstructor ??? ???

miLibro = ??? "Haskell Fácil" 300

--------------------------------------------------
-- 4. Define las funciones
--------------------------------------------------

data Punto = Pnt Int Int

coordX :: Punto -> Int
coordX ??? = ???

coordY :: Punto -> Int
coordY ??? = ???

--------------------------------------------------
-- 5. Multiplicar fracciones
--------------------------------------------------

data Fraccion = Frac Integer Integer

producto :: Fraccion -> Fraccion -> Fraccion
producto ??? = ???

-- producto (Frac 1 2) (Frac 3 4)
-- Resultado: Frac 3 8

--------------------------------------------------
-- 6. Constructores con nombres de campos
--------------------------------------------------

-- Define los nombres de atributos necesarios
-- data Alumno = Alumno String Float


data Alumno = Alumno {
    ???,
    ???
}

aAlumno = Alumno {
    nombreAlumno = "Pepe",
    notaAlumno = 7.5
}

--------------------------------------------------
-- 9. Completa para que funcione
--------------------------------------------------

data Caja a = ???

c1 :: Caja Int
c1 = Caja 5

c2 :: Caja String
c2 = Caja "hola"


--------------------------------------------------
-- 13. Constructor incorrecto - ¿Qué falla?
--------------------------------------------------

data Animal = Perro String | Gato String

aAnimal = Animal "Toby"

--------------------------------------------------
-- 18. Piensa por qué funciona esto
--------------------------------------------------

data Punto = Punto Float Float







-----------
-----------
-- STOP FOR NOW :)
-----------
-----------




--------------------------------------------------
-- 1. Eq con restricción
--------------------------------------------------

elemEnLista :: ??? => ??? -> [a] -> Bool
elemEnLista _ [] = False
elemEnLista x (y:ys) = x == y || elemEnLista x ys


--------------------------------------------------
-- 3. Show con tipo propio
--------------------------------------------------

data Coordenada = Coord Int Int

instance Show Coordenada where
    show (Coord x y) =
        "Coord(" ++ (show x) ++ "," ++ (show y) ++ ")"



--------------------------------------------------
-- 6. Completar para comparar, ordenar, y mostrar por pantalla
--------------------------------------------------

data Prioridad = Baja | Media | Alta
    deriving (Eq, Ord, Show)



--------------------------------------------------
-- 7. Clase propia
--------------------------------------------------

class Vacio a where
    estaVacio :: a -> Bool

instance Vacio [a] where
    estaVacio [] = True
    estaVacio _  = False


--------------------------------------------------
-- 8. Clase propia para texto
--------------------------------------------------

class Resumible a where
    resumen :: a -> String

data Libro = Libro String String Int

instance Resumible Libro where
    resumen (Libro titulo autor _) =
        titulo ++ ", de " ++ autor



--------------------------------------------------
-- 10. Eq para tipo recursivo
--------------------------------------------------

data Camino a =
      Fin
    | Paso a (Camino a)

instance Eq a => Eq (Camino a) where
    Fin == Fin = True
    Paso x xs == Paso y ys = x == y && xs == ys
    _ == _ = False
