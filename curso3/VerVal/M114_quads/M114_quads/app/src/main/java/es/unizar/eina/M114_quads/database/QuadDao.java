package es.unizar.eina.M114_quads.database;

import androidx.lifecycle.LiveData;
import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.OnConflictStrategy;
import androidx.room.Query;
import androidx.room.Update;

import java.util.List;

@Dao
public interface QuadDao {

    @Insert(onConflict = OnConflictStrategy.IGNORE)
    long insert(Quad quad);

    @Update
    int update(Quad quad);

    @Delete
    int delete(Quad quad);

    @Query("DELETE FROM Quads")
    void deleteAll();

    // --- MÉTODOS DE CONSULTA ---

    @Query("SELECT * FROM Quads ORDER BY MatriQuad ASC")
    LiveData<List<Quad>> getQuadsSortedByMatricula();

    @Query("SELECT * FROM Quads ORDER BY Tipo ASC")
    LiveData<List<Quad>> getQuadsSortedByTipo();

    // --- ORDENACIÓN POR PRECIO (ASC y DESC) ---
    @Query("SELECT * FROM Quads ORDER BY PrecioDia ASC")
    LiveData<List<Quad>> getQuadsSortedByPrecioAsc();

    @Query("SELECT * FROM Quads ORDER BY PrecioDia DESC")
    LiveData<List<Quad>> getQuadsSortedByPrecioDesc();

    @Query("SELECT * FROM Quads WHERE MatriQuad = :matricula")
    LiveData<Quad> getQuadByMatricula(String matricula);

    @Query("SELECT * FROM Quads ORDER BY MatriQuad ASC")
    LiveData<List<Quad>> getOrderedQuads();

    // --- DISPONIBILIDAD ---
    @Query("SELECT Q.*, " +
            "(SELECT COUNT(*) FROM RelacionQuadsReservas R " +
            " JOIN Reservas RES ON R.IDReserva = RES.IDReserva " +
            " WHERE R.Matricula = Q.MatriQuad " +
            "   AND RES.IDReserva != :currentReservaId " +
            "   AND (:fechaInicio <= RES.fechaDeDevolucion AND :fechaFin >= RES.fechaDeRecogida) " +
            ") > 0 AS esOcupado " +
            "FROM Quads Q " +
            "ORDER BY esOcupado ASC, Q.MatriQuad ASC")
    LiveData<List<QuadDisponibilidad>> getQuadsConDisponibilidad(String fechaInicio, String fechaFin, int currentReservaId);

    // --- BÚSQUEDA ---
    @Query("SELECT * FROM Quads WHERE MatriQuad LIKE :query ORDER BY MatriQuad ASC")
    LiveData<List<Quad>> searchQuads(String query);
}