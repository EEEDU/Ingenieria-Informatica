package es.unizar.eina.M114_quads.ui.RelacionQuadsReservas;

import android.app.Application;
import androidx.annotation.NonNull;
import androidx.lifecycle.AndroidViewModel;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.Transformations;
import java.util.List;
import es.unizar.eina.M114_quads.database.Quad;
import es.unizar.eina.M114_quads.database.QuadConCascos;
import es.unizar.eina.M114_quads.database.RelacionQuadsReservas;
import es.unizar.eina.M114_quads.database.TiendaQuadsRepository;

public class RelacionQuadsReservasViewModel extends AndroidViewModel {

    private TiendaQuadsRepository mRepository;
    private final MutableLiveData<Integer> mIdReservaTrigger = new MutableLiveData<>();
    private final LiveData<List<QuadConCascos>> mQuadsDeLaReserva;

    public RelacionQuadsReservasViewModel(@NonNull Application application) {
        super(application);
        mRepository = new TiendaQuadsRepository(application);
        mQuadsDeLaReserva = Transformations.switchMap(mIdReservaTrigger, id -> {
            if (id == null || id == 0) return new MutableLiveData<>(null);
            return mRepository.getQuadsConCascosParaReserva(id);
        });
    }

    public void cargarQuadsParaReserva(int idReserva) {
        mIdReservaTrigger.setValue(idReserva);
    }

    public LiveData<List<QuadConCascos>> getQuadsDeLaReserva() {
        return mQuadsDeLaReserva;
    }

    // --- MÉTODO QUE FALTABA Y CAUSABA EL ERROR ---
    public LiveData<List<RelacionQuadsReservas>> getRelacionesParaReserva(int idReserva) {
        return mRepository.getRelacionesParaReserva(idReserva);
    }
    // ---------------------------------------------

    public LiveData<List<Quad>> getAllQuads() {
        return mRepository.getQuadsSortedByMatricula();
    }

    public LiveData<List<String>> getMatriculasOcupadas(int idReserva, String f1, String f2) {
        return mRepository.getMatriculasOcupadas(idReserva, f1, f2);
    }

    // --- Escritura ---
    public long insertRelacion(RelacionQuadsReservas relacion) { return mRepository.insertRelacion(relacion); }
    public int updateRelacion(RelacionQuadsReservas relacion) { return mRepository.updateRelacion(relacion); }
    public int deleteRelacion(RelacionQuadsReservas relacion) { return mRepository.deleteRelacion(relacion); }
}