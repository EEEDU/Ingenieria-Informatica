package es.unizar.eina.M114_quads.ui.reservas;

import android.view.ContextMenu;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.TextView;
import android.graphics.Color;

import androidx.recyclerview.widget.RecyclerView;

import es.unizar.eina.M114_quads.R;
import es.unizar.eina.M114_quads.database.Reserva;

import java.util.Locale;

class ReservaViewHolder extends RecyclerView.ViewHolder implements View.OnCreateContextMenuListener {
    private final TextView mNombreClienteView;
    private final TextView mTelefonoView;
    private final TextView mFechaRecogidaView;
    private final TextView mFechaDevolucionView;
    private final TextView mPrecioView;

    public final ImageButton mSendButton;
    public final ImageButton mEditButton;
    public final ImageButton mDeleteButton;

    private ReservaViewHolder(View itemView) {
        super(itemView);
        mNombreClienteView = itemView.findViewById(R.id.tv_nombre_cliente);
        mTelefonoView = itemView.findViewById(R.id.tv_telefono);
        mFechaRecogidaView = itemView.findViewById(R.id.tv_fecha_recogida);
        mFechaDevolucionView = itemView.findViewById(R.id.tv_fecha_devolucion);
        mPrecioView = itemView.findViewById(R.id.tv_precio);

        mSendButton = itemView.findViewById(R.id.btn_send_reserva);
        mEditButton = itemView.findViewById(R.id.btn_edit_reserva);
        mDeleteButton = itemView.findViewById(R.id.btn_delete_reserva);

        itemView.setOnCreateContextMenuListener(this);
    }

    public void bind(Reserva reserva) {
        if (reserva != null) {
            mNombreClienteView.setText(reserva.getNombreCliente());
            // Ahora es String directo, sin String.valueOf
            mTelefonoView.setText(reserva.getTelfCliente());
            mFechaRecogidaView.setText(reserva.getFechaDeRecogida());
            mFechaDevolucionView.setText(reserva.getFechaDeDevolucion());

            if (reserva.getPrecioTotal() > 0) {
                mPrecioView.setText(String.format(Locale.getDefault(), "%.2f€", reserva.getPrecioTotal()));
                mPrecioView.setTextColor(Color.parseColor("#C62828"));
            } else {
                mPrecioView.setText("Sin Quads");
                mPrecioView.setTextColor(Color.GRAY);
                mPrecioView.setTextSize(12);
            }

        } else {
            mNombreClienteView.setText("Reserva no disponible");
        }
    }

    static ReservaViewHolder create(ViewGroup parent) {
        View view = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.reserva_item, parent, false);
        return new ReservaViewHolder(view);
    }

    @Override
    public void onCreateContextMenu(ContextMenu menu, View v, ContextMenu.ContextMenuInfo menuInfo) {
        menu.add(Menu.NONE, ReservaActivity.DELETE_ID, 0, R.string.menu_delete);
        menu.add(Menu.NONE, ReservaActivity.EDIT_ID, 1, R.string.menu_edit);
    }
}