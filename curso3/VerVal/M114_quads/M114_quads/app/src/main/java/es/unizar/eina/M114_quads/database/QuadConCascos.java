package es.unizar.eina.M114_quads.database;

import androidx.room.Embedded;
import androidx.room.ColumnInfo;

/**
 * POJO (Plain Old Java Object) que combina la entidad Quad
 * con el campo 'numCascos' de la tabla de relación.
 * Room lo usa para devolver los resultados de un JOIN.
 */
public class QuadConCascos {

    @Embedded
    public Quad quad; // Contiene MatriQuad, Tipo, Descripcion

    @ColumnInfo(name = "numCascos")
    public int numCascos; // De la tabla RelacionQuadsReservas
}