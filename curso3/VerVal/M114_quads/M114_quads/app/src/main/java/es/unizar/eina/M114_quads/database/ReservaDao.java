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
public interface ReservaDao {

    @Insert(onConflict = OnConflictStrategy.IGNORE)
    long insert(Reserva reserva);

    @Update
    int update(Reserva reserva);

    @Delete
    int delete(Reserva reserva);

    @Query("DELETE FROM Reservas")
    void deleteAll();

    // --- CONSULTAS DE ORDENACIÓN ---
    @Query("SELECT * FROM Reservas ORDER BY fechaDeRecogida ASC")
    LiveData<List<Reserva>> getReservasSortedByRecogida();

    @Query("SELECT * FROM Reservas ORDER BY NombreCliente ASC")
    LiveData<List<Reserva>> getReservasSortedByNombre();

    @Query("SELECT * FROM Reservas ORDER BY telfCliente ASC")
    LiveData<List<Reserva>> getReservasSortedByTelefono();

    @Query("SELECT * FROM Reservas ORDER BY fechaDeDevolucion DESC")
    LiveData<List<Reserva>> getReservasSortedByDevolucion();

    @Query("SELECT * FROM Reservas ORDER BY precioTotal ASC")
    LiveData<List<Reserva>> getReservasSortedByPrecioAsc();

    @Query("SELECT * FROM Reservas ORDER BY precioTotal DESC")
    LiveData<List<Reserva>> getReservasSortedByPrecioDesc();

    @Query("SELECT * FROM Reservas WHERE IDReserva = :id")
    LiveData<Reserva> getReservaById(int id);

    @Query("SELECT COUNT(*) FROM Reservas")
    int getCount();

    @Query("SELECT * FROM Reservas WHERE NombreCliente LIKE :query ORDER BY NombreCliente ASC")
    LiveData<List<Reserva>> searchReservas(String query);

    // --- NUEVO: ACTUALIZAR PRECIO ---
    @Query("UPDATE Reservas SET precioTotal = :nuevoPrecio WHERE IDReserva = :id")
    void updatePrecio(int id, double nuevoPrecio);
}