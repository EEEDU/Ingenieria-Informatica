
package es.unizar.eina.M114_quads.database;

import androidx.lifecycle.LiveData;
import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.OnConflictStrategy;
import androidx.room.Query;
import androidx.room.Update;

import java.util.List;

/**
 * DAO para gestionar la relación entre Quads y Reservas.
 */
@Dao
public interface RelacionQuadsReservasDao {

    @Insert(onConflict = OnConflictStrategy.IGNORE)
    long insert(RelacionQuadsReservas relacion);

    @Update
    int update(RelacionQuadsReservas relacion);

    @Delete
    int delete(RelacionQuadsReservas relacion);

    @Query("DELETE FROM RelacionQuadsReservas WHERE IDReserva = :idReserva")
    int deleteRelacionesDeReserva(int idReserva);

    @Query("SELECT * FROM RelacionQuadsReservas WHERE IDReserva = :idReserva")
    LiveData<List<RelacionQuadsReservas>> getRelacionesParaReserva(int idReserva);

    @Query("SELECT Quads.*, RelacionQuadsReservas.numCascos " +
            "FROM Quads " +
            "INNER JOIN RelacionQuadsReservas ON Quads.MatriQuad = RelacionQuadsReservas.Matricula " +
            "WHERE RelacionQuadsReservas.IDReserva = :idReserva")
    LiveData<List<QuadConCascos>> getQuadsConCascosParaReserva(int idReserva);

    // --- MÉTODO QUE FALTABA Y CAUSABA EL ERROR ---
    /**
     * Devuelve las matrículas de los quads que están en reservas que se solapan con las fechas dadas.
     * Excluye la reserva actual (idReserva) para permitir editarla.
     */
    @Query("SELECT RelacionQuadsReservas.Matricula FROM RelacionQuadsReservas " +
            "INNER JOIN Reservas ON RelacionQuadsReservas.IDReserva = Reservas.IDReserva " +
            "WHERE Reservas.IDReserva != :idReserva " +
            "AND (:fechaInicio <= Reservas.fechaDeDevolucion AND :fechaFin >= Reservas.fechaDeRecogida)")
    LiveData<List<String>> getMatriculasOcupadas(int idReserva, String fechaInicio, String fechaFin);
}