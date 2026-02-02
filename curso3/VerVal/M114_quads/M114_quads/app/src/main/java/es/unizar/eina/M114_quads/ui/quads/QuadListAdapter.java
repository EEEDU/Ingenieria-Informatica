package es.unizar.eina.M114_quads.ui.quads;

import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.DiffUtil;
import androidx.recyclerview.widget.ListAdapter;

import es.unizar.eina.M114_quads.database.Quad;

public class QuadListAdapter extends ListAdapter<Quad, QuadViewHolder> {

    // 1. Definimos la interfaz para comunicar los clics
    public interface OnQuadActionListener {
        void onEdit(Quad quad);
        void onDelete(Quad quad);
    }

    private final OnQuadActionListener mListener;
    private int position;

    public int getPosition() {
        return position;
    }

    public void setPosition(int position) {
        this.position = position;
    }

    // 2. Actualizamos el constructor para recibir el listener
    public QuadListAdapter(@NonNull DiffUtil.ItemCallback<Quad> diffCallback, OnQuadActionListener listener) {
        super(diffCallback);
        mListener = listener;
    }

    @Override
    public QuadViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        return QuadViewHolder.create(parent);
    }

    public Quad getCurrent() {
        if (getItemCount() == 0) { return null; }
        return getItem(getPosition());
    }

    @Override
    public void onBindViewHolder(QuadViewHolder holder, int position) {
        Quad current = getItem(position);
        holder.bind(current);

        // 3. Asignamos la acción a los botones
        holder.mEditButton.setOnClickListener(v -> {
            if (mListener != null) mListener.onEdit(current);
        });

        holder.mDeleteButton.setOnClickListener(v -> {
            if (mListener != null) mListener.onDelete(current);
        });

        // Mantener el long click para menú contextual (opcional)
        holder.itemView.setOnLongClickListener(v -> {
            setPosition(holder.getAdapterPosition());
            return false;
        });
    }

    static class QuadDiff extends DiffUtil.ItemCallback<Quad> {
        @Override
        public boolean areItemsTheSame(@NonNull Quad oldItem, @NonNull Quad newItem) {
            return oldItem.getMatriQuad().equals(newItem.getMatriQuad());
        }

        @Override
        public boolean areContentsTheSame(@NonNull Quad oldItem, @NonNull Quad newItem) {
            return oldItem.getTipo().equals(newItem.getTipo()) &&
                    oldItem.getDescripcion().equals(newItem.getDescripcion());
        }
    }
}