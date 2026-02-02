package es.unizar.eina.M114_quads.ui.quads;

import android.app.Application;
import androidx.lifecycle.AndroidViewModel;
import androidx.lifecycle.LiveData;
import java.util.List;
import es.unizar.eina.M114_quads.database.Quad;
import es.unizar.eina.M114_quads.database.QuadDisponibilidad;
import es.unizar.eina.M114_quads.database.TiendaQuadsRepository;

public class QuadViewModel extends AndroidViewModel {

    private TiendaQuadsRepository mRepository;

    public QuadViewModel (Application application) {
        super(application);
        mRepository = new TiendaQuadsRepository(application);
    }

    public LiveData<List<Quad>> getQuadsSortedByMatricula() {
        return mRepository.getQuadsSortedByMatricula();
    }

    public LiveData<List<Quad>> getQuadsSortedByTipo() {
        return mRepository.getQuadsSortedByTipo();
    }

    // MÉTODOS DE PRECIO
    public LiveData<List<Quad>> getQuadsSortedByPrecioAsc() {
        return mRepository.getQuadsSortedByPrecioAsc();
    }

    public LiveData<List<Quad>> getQuadsSortedByPrecioDesc() {
        return mRepository.getQuadsSortedByPrecioDesc();
    }

    public LiveData<Quad> getQuadByMatricula(String matricula) {
        return mRepository.getQuadByMatricula(matricula);
    }

    public LiveData<List<QuadDisponibilidad>> getQuadsConDisponibilidad(String fechaInicio, String fechaFin, int currentReservaId) {
        return mRepository.getQuadsConDisponibilidad(fechaInicio, fechaFin, currentReservaId);
    }

    public LiveData<List<Quad>> searchQuads(String query) {
        return mRepository.searchQuads(query);
    }

    public long insert(Quad quad) { return mRepository.insert(quad); }
    public int update(Quad quad) { return mRepository.update(quad); }
    public int delete(Quad quad) { return mRepository.delete(quad); }
}