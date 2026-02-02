package es.unizar.eina.M114_quads.database;

import android.app.Application;
import androidx.lifecycle.LiveData;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

public class TiendaQuadsRepository {

    private final QuadDao mQuadDao;
    private final ReservaDao mReservaDao;
    private final RelacionQuadsReservasDao mRelacionDao;

    private final long TIMEOUT = 15000;

    public TiendaQuadsRepository(Application application) {
        TiendaQuadsRoomDatabase db = TiendaQuadsRoomDatabase.getDatabase(application);
        mQuadDao = db.quadDao();
        mReservaDao = db.reservaDao();
        mRelacionDao = db.relacionDao();
    }

    // --- INSERTS ---
    public long insert(Quad quad) {
        Future<Long> future = TiendaQuadsRoomDatabase.databaseWriteExecutor.submit(() -> mQuadDao.insert(quad));
        try { return future.get(TIMEOUT, TimeUnit.MILLISECONDS); } catch (Exception ex) { return -1; }
    }

    public long insert(Reserva reserva) {
        Future<Long> future = TiendaQuadsRoomDatabase.databaseWriteExecutor.submit(() -> mReservaDao.insert(reserva));
        try { return future.get(TIMEOUT, TimeUnit.MILLISECONDS); } catch (Exception ex) { return -1; }
    }

    public long insertRelacion(RelacionQuadsReservas relacion) {
        Future<Long> future = TiendaQuadsRoomDatabase.databaseWriteExecutor.submit(() -> mRelacionDao.insert(relacion));
        try { return future.get(TIMEOUT, TimeUnit.MILLISECONDS); } catch (Exception ex) { return -1; }
    }

    // --- UPDATES ---
    public int update(Quad quad) {
        Future<Integer> future = TiendaQuadsRoomDatabase.databaseWriteExecutor.submit(() -> mQuadDao.update(quad));
        try { return future.get(TIMEOUT, TimeUnit.MILLISECONDS); } catch (Exception ex) { return -1; }
    }

    public int update(Reserva reserva) {
        Future<Integer> future = TiendaQuadsRoomDatabase.databaseWriteExecutor.submit(() -> mReservaDao.update(reserva));
        try { return future.get(TIMEOUT, TimeUnit.MILLISECONDS); } catch (Exception ex) { return -1; }
    }

    public void updatePrecioReserva(int id, double precio) {
        TiendaQuadsRoomDatabase.databaseWriteExecutor.execute(() -> mReservaDao.updatePrecio(id, precio));
    }

    public int updateRelacion(RelacionQuadsReservas relacion) {
        Future<Integer> future = TiendaQuadsRoomDatabase.databaseWriteExecutor.submit(() -> mRelacionDao.update(relacion));
        try { return future.get(TIMEOUT, TimeUnit.MILLISECONDS); } catch (Exception ex) { return -1; }
    }

    // --- DELETES ---
    public int delete(Quad quad) {
        Future<Integer> future = TiendaQuadsRoomDatabase.databaseWriteExecutor.submit(() -> mQuadDao.delete(quad));
        try { return future.get(TIMEOUT, TimeUnit.MILLISECONDS); } catch (Exception ex) { return -1; }
    }

    public int delete(Reserva reserva) {
        Future<Integer> future = TiendaQuadsRoomDatabase.databaseWriteExecutor.submit(() -> mReservaDao.delete(reserva));
        try { return future.get(TIMEOUT, TimeUnit.MILLISECONDS); } catch (Exception ex) { return -1; }
    }

    public int deleteRelacion(RelacionQuadsReservas relacion) {
        Future<Integer> future = TiendaQuadsRoomDatabase.databaseWriteExecutor.submit(() -> mRelacionDao.delete(relacion));
        try { return future.get(TIMEOUT, TimeUnit.MILLISECONDS); } catch (Exception ex) { return -1; }
    }

    public int deleteRelacionesDeReserva(int idReserva) {
        Future<Integer> future = TiendaQuadsRoomDatabase.databaseWriteExecutor.submit(() -> mRelacionDao.deleteRelacionesDeReserva(idReserva));
        try { return future.get(TIMEOUT, TimeUnit.MILLISECONDS); } catch (Exception ex) { return -1; }
    }

    // --- QUERIES ---

    public LiveData<List<Quad>> getQuadsSortedByMatricula() { return mQuadDao.getQuadsSortedByMatricula(); }
    public LiveData<List<Quad>> getQuadsSortedByTipo() { return mQuadDao.getQuadsSortedByTipo(); }

    // MÉTODOS DE ORDENACIÓN DE PRECIO
    public LiveData<List<Quad>> getQuadsSortedByPrecioAsc() { return mQuadDao.getQuadsSortedByPrecioAsc(); }
    public LiveData<List<Quad>> getQuadsSortedByPrecioDesc() { return mQuadDao.getQuadsSortedByPrecioDesc(); }

    public LiveData<Quad> getQuadByMatricula(String matricula) { return mQuadDao.getQuadByMatricula(matricula); }
    public LiveData<List<QuadDisponibilidad>> getQuadsConDisponibilidad(String fechaInicio, String fechaFin, int currentReservaId) {
        return mQuadDao.getQuadsConDisponibilidad(fechaInicio, fechaFin, currentReservaId);
    }

    public LiveData<List<Reserva>> getReservasSortedByNombre() { return mReservaDao.getReservasSortedByNombre(); }
    public LiveData<List<Reserva>> getReservasSortedByTelefono() { return mReservaDao.getReservasSortedByTelefono(); }
    public LiveData<List<Reserva>> getReservasSortedByRecogida() { return mReservaDao.getReservasSortedByRecogida(); }
    public LiveData<List<Reserva>> getReservasSortedByDevolucion() { return mReservaDao.getReservasSortedByDevolucion(); }
    public LiveData<List<Reserva>> getReservasSortedByPrecioAsc() { return mReservaDao.getReservasSortedByPrecioAsc(); }
    public LiveData<List<Reserva>> getReservasSortedByPrecioDesc() { return mReservaDao.getReservasSortedByPrecioDesc(); }
    public LiveData<Reserva> getReservaById(int id) { return mReservaDao.getReservaById(id); }

    public LiveData<List<RelacionQuadsReservas>> getRelacionesParaReserva(int idReserva) { return mRelacionDao.getRelacionesParaReserva(idReserva); }
    public LiveData<List<QuadConCascos>> getQuadsConCascosParaReserva(int idReserva) { return mRelacionDao.getQuadsConCascosParaReserva(idReserva); }
    public LiveData<List<String>> getMatriculasOcupadas(int idReserva, String fechaInicio, String fechaFin) {
        return mRelacionDao.getMatriculasOcupadas(idReserva, fechaInicio, fechaFin);
    }

    public LiveData<List<Quad>> searchQuads(String query) { return mQuadDao.searchQuads(query); }
    public LiveData<List<Reserva>> searchReservas(String query) { return mReservaDao.searchReservas(query); }

    public void actualizarReservaCompleta(int idReserva, List<RelacionQuadsReservas> nuevasRelaciones, double precioTotal) {
        TiendaQuadsRoomDatabase.databaseWriteExecutor.execute(() -> {
            mRelacionDao.deleteRelacionesDeReserva(idReserva);
            for (RelacionQuadsReservas rel : nuevasRelaciones) {
                mRelacionDao.insert(rel);
            }
            mReservaDao.updatePrecio(idReserva, precioTotal);
        });
    }

    // --- NUEVO: MÉTODO PARA LIMPIAR TODA LA BD (Usado en Tests) ---
    public void deleteAll() {
        TiendaQuadsRoomDatabase.databaseWriteExecutor.execute(() -> {
            // Borramos reservas y quads. Las relaciones se borran solas por el ON DELETE CASCADE
            // definido en la entidad RelacionQuadsReservas
            mReservaDao.deleteAll();
            mQuadDao.deleteAll();
        });
    }
}