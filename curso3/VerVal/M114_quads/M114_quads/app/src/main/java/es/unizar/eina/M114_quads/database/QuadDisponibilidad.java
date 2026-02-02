package es.unizar.eina.M114_quads.database;

import androidx.room.Embedded;

public class QuadDisponibilidad {
    // "Embedded" hace que los campos de Quad se aplanen aquí dentro
    @Embedded
    public Quad quad;

    // Campo calculado: true si está ocupado en las fechas, false si está libre
    public boolean esOcupado;
}