package es.unizar.eina.M114_quads.database;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.PrimaryKey;

@Entity(tableName = "Quads")
public class Quad {

    @PrimaryKey
    @NonNull
    @ColumnInfo(name = "MatriQuad")
    private String matriQuad;

    @NonNull
    @ColumnInfo(name = "Tipo")
    private String tipo;

    @NonNull
    @ColumnInfo(name = "Descripcion")
    private String descripcion;

    @ColumnInfo(name = "PrecioDia")
    private double precioDia;

    public Quad(@NonNull String matriQuad, @NonNull String tipo, @NonNull String descripcion, double precioDia) {
        // --- VALIDACIONES (Para que el Test de Caja Negra funcione) ---
        if (matriQuad == null || matriQuad.trim().isEmpty()) {
            throw new IllegalArgumentException("La matrícula no puede estar vacía");
        }
        if (tipo == null || tipo.trim().isEmpty()) {
            throw new IllegalArgumentException("El tipo no puede estar vacío");
        }
        if (precioDia < 0) {
            throw new IllegalArgumentException("El precio no puede ser negativo");
        }
        // -------------------------------------------------------------

        this.matriQuad = matriQuad;
        this.tipo = tipo;
        this.descripcion = descripcion;
        this.precioDia = precioDia;
    }

    @NonNull
    public String getMatriQuad(){ return this.matriQuad; }

    @NonNull
    public String getTipo(){ return this.tipo; }

    @NonNull
    public String getDescripcion(){ return this.descripcion; }

    public double getPrecioDia() { return this.precioDia; }
}