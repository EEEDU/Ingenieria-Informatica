package es.unizar.eina.M114_quads.ui.RelacionQuadsReservas; // (O donde lo vayas a poner)

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import es.unizar.eina.M114_quads.R;
import es.unizar.eina.M114_quads.database.QuadConCascos;

/**
 * ViewHolder que representa un item individual en la lista de Quads de una Reserva.
 * Muestra la información de un Quad y el número de cascos asignados.
 */
class RelacionQuadsReservasViewHolder extends RecyclerView.ViewHolder {

    // Vistas del layout (reserva_quad_item.xml)
    private final TextView mMatriculaView;
    private final TextView mTipoView;
    private final TextView mNumCascosView;

    private RelacionQuadsReservasViewHolder(@NonNull View itemView) {
        super(itemView);
        mMatriculaView = itemView.findViewById(R.id.item_matricula);
        mTipoView = itemView.findViewById(R.id.item_tipo);
        mNumCascosView = itemView.findViewById(R.id.item_cascos);

        // Aquí puedes añadir un OnCreateContextMenuListener
        // si quieres poder borrar quads de la reserva con una pulsación larga
        // itemView.setOnCreateContextMenuListener(this);
    }

    /**
     * Rellena las vistas del item con los datos del objeto QuadConCascos.
     * @param quadConCascos El objeto que contiene el Quad y el número de cascos.
     */
    public void bind(QuadConCascos quadConCascos) {
        if (quadConCascos != null) {
            // Rellena los datos del quad
            mMatriculaView.setText(quadConCascos.quad.getMatriQuad());
            mTipoView.setText(quadConCascos.quad.getTipo());

            // Formatea el string de cascos para que sea descriptivo
            String cascosStr = quadConCascos.numCascos + " cascos";
            mNumCascosView.setText(cascosStr);
        } else {
            // Muestra un estado de carga o error si el objeto es nulo
            mMatriculaView.setText("Cargando...");
            mTipoView.setText("");
            mNumCascosView.setText("");
        }
    }

    /**
     * Método estático para inflar el layout y crear el ViewHolder.
     * @param parent El ViewGroup padre.
     * @return Una nueva instancia de RelacionQuadsReservasViewHolder.
     */
    static RelacionQuadsReservasViewHolder create(ViewGroup parent) {
        // Infla el layout XML (reserva_quad_item.xml)
        View view = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.reserva_quad_item, parent, false);
        return new RelacionQuadsReservasViewHolder(view);
    }

    // (Opcional) Si añades el ContextMenuListener, implementa este método
    /*
    @Override
    public void onCreateContextMenu(ContextMenu menu, View v, ContextMenu.ContextMenuInfo menuInfo) {
        // Añade opciones como "Eliminar de la reserva" o "Cambiar cascos"
        menu.add(Menu.NONE, ID_ELIMINAR_RELACION, Menu.NONE, "Quitar de la reserva");
    }
    */
}