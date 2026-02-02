package es.unizar.eina.M114_quads.ui.quads;

import android.view.ContextMenu;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;

import es.unizar.eina.M114_quads.R;
import es.unizar.eina.M114_quads.database.Quad;

import java.util.Locale;

class QuadViewHolder extends RecyclerView.ViewHolder implements View.OnCreateContextMenuListener {
    private final TextView mMatriculaView;
    private final TextView mTipoBadgeView;
    private final TextView mDescripcionView;
    private final TextView mPrecioView; // NUEVO CAMPO

    public final ImageButton mEditButton;
    public final ImageButton mDeleteButton;

    private QuadViewHolder(View itemView) {
        super(itemView);
        mMatriculaView = itemView.findViewById(R.id.tv_matricula);
        mTipoBadgeView = itemView.findViewById(R.id.tv_tipo_badge);
        mDescripcionView = itemView.findViewById(R.id.tv_descripcion);
        mPrecioView = itemView.findViewById(R.id.tv_precio_quad); // Enlazar vista

        mEditButton = itemView.findViewById(R.id.btn_edit);
        mDeleteButton = itemView.findViewById(R.id.btn_delete);

        itemView.setOnCreateContextMenuListener(this);
    }

    public void bind(Quad quad) {
        if (quad != null) {
            mMatriculaView.setText(quad.getMatriQuad());
            mTipoBadgeView.setText(quad.getTipo().toUpperCase());

            // Mostrar precio formateado
            mPrecioView.setText(String.format(Locale.getDefault(), "%.2f€/día", quad.getPrecioDia()));

            if (quad.getDescripcion() != null && !quad.getDescripcion().isEmpty()) {
                mDescripcionView.setText(quad.getDescripcion());
            } else {
                mDescripcionView.setText("Sin descripción disponible");
            }
        } else {
            mMatriculaView.setText("Quad no disponible");
        }
    }

    static QuadViewHolder create(ViewGroup parent) {
        View view = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.recyclerview_item, parent, false);
        return new QuadViewHolder(view);
    }

    @Override
    public void onCreateContextMenu(ContextMenu menu, View v, ContextMenu.ContextMenuInfo menuInfo) {
        menu.add(Menu.NONE, QuadActivity.DELETE_ID, 0, R.string.menu_delete);
        menu.add(Menu.NONE, QuadActivity.EDIT_ID, 1, R.string.menu_edit);
    }
}