AREA codigo, CODE 
EXPORT ordena 
ordena 
; SBR que ordena una tabla de enteros de 32 bits. Parametros: 
; r0 = @ de comienzo de la tabla a ordenar (tabla por referencia)  
; r1 = Numero de elementos de la tabla (num_elem por valor) 
PUSH {lr} 
<apilar registros utilizados> 
<llamar a qksort recursivo> 
<desapilar registros utilizados> 
POP {pc} 
qksort 
; SBR qksort realizada en la parte 1 (no es necesario modificarla) 
END 