package es.unizar.eina.M114_quads.ui.RelacionQuadsReservas; // (O donde lo vayas a poner)

import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.DiffUtil;
import androidx.recyclerview.widget.ListAdapter;

import es.unizar.eina.M114_quads.database.QuadConCascos;

/**
 * Adapter para el RecyclerView que muestra la lista de Quads (con cascos)
 * asociados a una reserva.
 * Utiliza ListAdapter para un manejo eficiente de las actualizaciones de la lista.
 */
public class RelacionQuadsReservasListAdapter extends ListAdapter<QuadConCascos, RelacionQuadsReservasViewHolder> {

    /**
     * Constructor. Pasa el callback DiffUtil a la superclase.
     */
    public RelacionQuadsReservasListAdapter(@NonNull DiffUtil.ItemCallback<QuadConCascos> diffCallback) {
        super(diffCallback);
    }

    @NonNull
    @Override
    public RelacionQuadsReservasViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        // Llama al método estático create del ViewHolder
        return RelacionQuadsReservasViewHolder.create(parent);
    }

    @Override
    public void onBindViewHolder(@NonNull RelacionQuadsReservasViewHolder holder, int position) {
        QuadConCascos current = getItem(position);
        // Pasa el objeto QuadConCascos al ViewHolder para que lo "pinte"
        holder.bind(current);
    }

    /**
     * Clase interna estática para calcular las diferencias entre dos listas.
     * Esto permite al ListAdapter realizar animaciones eficientes (ej. fades)
     * cuando un quad se añade, elimina o cambia en la lista.
     */
    static class QuadConCascosDiff extends DiffUtil.ItemCallback<QuadConCascos> {

        @Override
        public boolean areItemsTheSame(@NonNull QuadConCascos oldItem, @NonNull QuadConCascos newItem) {
            // Un item es el mismo si su identificador único (PK del quad) es el mismo.
            return oldItem.quad.getMatriQuad().equals(newItem.quad.getMatriQuad());
        }

        @Override
        public boolean areContentsTheSame(@NonNull QuadConCascos oldItem, @NonNull QuadConCascos newItem) {
            // El contenido es el mismo si los datos que mostramos (tipo y cascos) son iguales.
            return oldItem.quad.getTipo().equals(newItem.quad.getTipo()) &&
                    oldItem.numCascos == newItem.numCascos &&
                    oldItem.quad.getDescripcion().equals(newItem.quad.getDescripcion());
        }
    }
}