def bissecion(a,b,error,d,g):
    """
    Funcion bissecion
    Args:
        a,b : Limites iniciales de la biseción
        error: Maximo error para considerar una solución
        d: Profundidad
        g: Numero de nodos Generados
    """
    def f(b, depth, gen):
        return ((b * (1 - b ** depth) / (1 - b)) - gen)
    m = 0.0
    if f(a,d,g)*f(b,d,g) > 0:
        print("no raiz")
        return -1
    else:
        m = (a+b)/2.0
        while(abs(a-b)>=error):
            if f(a,d,g)*f(m,d,g) > 0:
                a = m
            elif f(b,d,g)*f(m,d,g) > 0:
                b = m
            m = (a+b)/2.0
        return m