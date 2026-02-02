package es.unizar.eina.M114_quads.database;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.ForeignKey;
import androidx.room.Index;

/**
 * Entidad que representa la relación muchos-a-muchos entre Quads y Reservas.
 * Almacena el atributo adicional 'numCascos' para esta relación.
 * * Corresponde a la tabla 'RelacionQuadsReservas'.
 */
@Entity(tableName = "RelacionQuadsReservas",
        // Clave primaria compuesta por las dos claves foráneas
        primaryKeys = {"Matricula", "IDReserva"},

        // Índices para optimizar las consultas sobre las claves foráneas
        indices = {@Index(value = "Matricula"), @Index(value = "IDReserva")},

        // Definición de las claves foráneas
        foreignKeys = {
                @ForeignKey(entity = Quad.class,
                        parentColumns = "MatriQuad",
                        childColumns = "Matricula",
                        // Si se borra un Quad, se borra la relación
                        onDelete = ForeignKey.CASCADE),
                @ForeignKey(entity = Reserva.class,
                        parentColumns = "IDReserva",
                        childColumns = "IDReserva",
                        // Si se borra una Reserva, se borra la relación
                        onDelete = ForeignKey.CASCADE)
        })
public class RelacionQuadsReservas {

    @NonNull
    @ColumnInfo(name = "Matricula")
    private String matricula;

    @ColumnInfo(name = "IDReserva")
    private int idReserva;

    @ColumnInfo(name = "numCascos")
    private int numCascos;

    /**
     * Constructor de la relación.
     * @param matricula Matrícula del Quad (FK).
     * @param idReserva ID de la Reserva (FK).
     * @param numCascos Número de cascos para este quad en esta reserva (0, 1, o 2).
     */
    public RelacionQuadsReservas(@NonNull String matricula, int idReserva, int numCascos) {
        this.matricula = matricula;
        this.idReserva = idReserva;
        this.numCascos = numCascos;
    }

    // --- Getters ---

    @NonNull
    public String getMatricula() {
        return matricula;
    }

    public int getIdReserva() {
        return idReserva;
    }

    public int getNumCascos() {
        return numCascos;
    }
}