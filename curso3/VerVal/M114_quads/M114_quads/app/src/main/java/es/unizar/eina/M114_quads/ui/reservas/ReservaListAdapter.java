package es.unizar.eina.M114_quads.ui.reservas;

import android.view.ViewGroup;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.DiffUtil;
import androidx.recyclerview.widget.ListAdapter;
import es.unizar.eina.M114_quads.database.Reserva;

public class ReservaListAdapter extends ListAdapter<Reserva, ReservaViewHolder> {

    public interface OnReservaActionListener {
        void onEdit(Reserva reserva);
        void onDelete(Reserva reserva);
        void onSend(Reserva reserva);
    }

    private final OnReservaActionListener mListener;
    private int position;

    public int getPosition() {
        return position;
    }

    public void setPosition(int position) {
        this.position = position;
    }

    public ReservaListAdapter(@NonNull DiffUtil.ItemCallback<Reserva> diffCallback, OnReservaActionListener listener) {
        super(diffCallback);
        mListener = listener;
    }

    @Override
    public ReservaViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        return ReservaViewHolder.create(parent);
    }

    public Reserva getCurrent() {
        if (getItemCount() == 0) return null;
        return getItem(getPosition());
    }

    @Override
    public void onBindViewHolder(ReservaViewHolder holder, int position) {
        Reserva current = getItem(position);
        holder.bind(current);

        holder.mSendButton.setOnClickListener(v -> {
            if (mListener != null) mListener.onSend(current);
        });

        holder.mEditButton.setOnClickListener(v -> {
            if (mListener != null) mListener.onEdit(current);
        });

        holder.mDeleteButton.setOnClickListener(v -> {
            if (mListener != null) mListener.onDelete(current);
        });

        holder.itemView.setOnLongClickListener(v -> {
            setPosition(holder.getAdapterPosition());
            return false;
        });
    }

    static class ReservaDiff extends DiffUtil.ItemCallback<Reserva> {
        @Override
        public boolean areItemsTheSame(@NonNull Reserva oldItem, @NonNull Reserva newItem) {
            return oldItem.getIdReserva() == newItem.getIdReserva();
        }

        @Override
        public boolean areContentsTheSame(@NonNull Reserva oldItem, @NonNull Reserva newItem) {
            // Comprobación actualizada para Strings en teléfono
            return oldItem.getNombreCliente().equals(newItem.getNombreCliente()) &&
                    oldItem.getTelfCliente().equals(newItem.getTelfCliente()) &&
                    oldItem.getFechaDeRecogida().equals(newItem.getFechaDeRecogida()) &&
                    oldItem.getFechaDeDevolucion().equals(newItem.getFechaDeDevolucion()) &&
                    oldItem.getPrecioTotal() == newItem.getPrecioTotal();
        }
    }
}