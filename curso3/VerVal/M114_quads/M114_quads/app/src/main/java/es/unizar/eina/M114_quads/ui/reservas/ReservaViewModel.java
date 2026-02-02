package es.unizar.eina.M114_quads.ui.reservas;

import android.app.Application;
import androidx.lifecycle.AndroidViewModel;
import androidx.lifecycle.LiveData;
import java.util.List;
import es.unizar.eina.M114_quads.database.Reserva;
import es.unizar.eina.M114_quads.database.RelacionQuadsReservas;
import es.unizar.eina.M114_quads.database.TiendaQuadsRepository;

public class ReservaViewModel extends AndroidViewModel {

    private TiendaQuadsRepository mRepository;

    public ReservaViewModel(Application application) {
        super(application);
        mRepository = new TiendaQuadsRepository(application);
    }

    public LiveData<List<Reserva>> getReservasSortedByRecogida() { return mRepository.getReservasSortedByRecogida(); }
    public LiveData<List<Reserva>> getReservasSortedByNombre() { return mRepository.getReservasSortedByNombre(); }
    public LiveData<List<Reserva>> getReservasSortedByTelefono() { return mRepository.getReservasSortedByTelefono(); }
    public LiveData<List<Reserva>> getReservasSortedByDevolucion() { return mRepository.getReservasSortedByDevolucion(); }
    public LiveData<List<Reserva>> getReservasSortedByPrecioAsc() { return mRepository.getReservasSortedByPrecioAsc(); }
    public LiveData<List<Reserva>> getReservasSortedByPrecioDesc() { return mRepository.getReservasSortedByPrecioDesc(); }

    public LiveData<Reserva> getReservaById(int id) { return mRepository.getReservaById(id); }

    public LiveData<List<Reserva>> searchReservas(String query) {
        return mRepository.searchReservas(query);
    }

    public long insert(Reserva reserva) { return mRepository.insert(reserva); }
    public int update(Reserva reserva) { return mRepository.update(reserva); }
    public void updatePrecio(int id, double precio) { mRepository.updatePrecioReserva(id, precio); }
    public int delete(Reserva reserva) { return mRepository.delete(reserva); }

    // NUEVO: Método transaccional
    public void actualizarReservaCompleta(int idReserva, List<RelacionQuadsReservas> nuevasRelaciones, double precioTotal) {
        mRepository.actualizarReservaCompleta(idReserva, nuevasRelaciones, precioTotal);
    }
}