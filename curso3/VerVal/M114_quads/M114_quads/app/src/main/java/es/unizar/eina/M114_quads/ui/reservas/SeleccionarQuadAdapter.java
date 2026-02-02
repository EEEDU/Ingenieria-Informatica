package es.unizar.eina.M114_quads.ui.reservas;

import android.graphics.Color;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;
import android.widget.EditText;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import es.unizar.eina.M114_quads.R;
import es.unizar.eina.M114_quads.database.QuadDisponibilidad;
import es.unizar.eina.M114_quads.database.RelacionQuadsReservas;

public class SeleccionarQuadAdapter extends RecyclerView.Adapter<SeleccionarQuadAdapter.ViewHolder> {

    private List<QuadDisponibilidad> mQuads = new ArrayList<>();

    // Mapas para mantener el estado de la selección
    private final Map<String, Boolean> mSelectedState = new HashMap<>();
    private final Map<String, String> mCascosState = new HashMap<>();

    public void setQuads(List<QuadDisponibilidad> quads) {
        this.mQuads = quads;
        notifyDataSetChanged();
    }

    public void setEstadoInicial(List<RelacionQuadsReservas> relaciones) {
        if (relaciones == null) return;
        for (RelacionQuadsReservas r : relaciones) {
            mSelectedState.put(r.getMatricula(), true);
            mCascosState.put(r.getMatricula(), String.valueOf(r.getNumCascos()));
        }
        notifyDataSetChanged();
    }

    // --- ESTE ES EL MÉTODO QUE TE FALTABA ---
    public double getSumaPrecioDiaSeleccionados() {
        double suma = 0;
        for (QuadDisponibilidad item : mQuads) {
            if (item.esOcupado) continue;

            String mat = item.quad.getMatriQuad();
            // Si está seleccionado en el mapa
            if (mSelectedState.containsKey(mat) && Boolean.TRUE.equals(mSelectedState.get(mat))) {
                suma += item.quad.getPrecioDia();
            }
        }
        return suma;
    }
    // -----------------------------------------

    public Map<String, Integer> getSelectedQuads() {
        Map<String, Integer> result = new HashMap<>();
        for (QuadDisponibilidad item : mQuads) {
            // Si está ocupado por OTRA reserva, no lo guardamos
            if (item.esOcupado) continue;

            String mat = item.quad.getMatriQuad();
            if (mSelectedState.containsKey(mat) && Boolean.TRUE.equals(mSelectedState.get(mat))) {
                String cascosStr = mCascosState.get(mat);
                int cascos = 0;
                try {
                    cascos = (cascosStr != null && !cascosStr.isEmpty()) ? Integer.parseInt(cascosStr) : 0;
                } catch (NumberFormatException e) { cascos = 0; }
                result.put(mat, cascos);
            }
        }
        return result;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.item_seleccionar_quad, parent, false);
        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        QuadDisponibilidad item = mQuads.get(position);
        String matricula = item.quad.getMatriQuad();

        // Determinar el máximo de cascos según el tipo
        // Si es "Biplaza" -> max 2, si es "Monoplaza" u otro -> max 1 (o 2 si quieres permitirlo también)
        boolean esBiplaza = "Biplaza".equalsIgnoreCase(item.quad.getTipo());
        int maxCascos = esBiplaza ? 2 : 1; // Ajusta este 1 a 2 si un monoplaza puede llevar 2 cascos (ej. mochila)

        // Lógica visual: Si está ocupado
        if (item.esOcupado) {
            holder.checkBox.setText(matricula + " (OCUPADO)");
            holder.checkBox.setTextColor(Color.GRAY);
            holder.checkBox.setEnabled(false);
            holder.checkBox.setChecked(false);

            holder.editCascos.setEnabled(false);
            holder.editCascos.setAlpha(0.3f);
            holder.itemView.setAlpha(0.5f);
        } else {
            holder.checkBox.setText(matricula + " (" + item.quad.getTipo() + ") - " + item.quad.getPrecioDia() + "€/día");
            holder.checkBox.setTextColor(Color.BLACK);
            holder.checkBox.setEnabled(true);

            holder.editCascos.setEnabled(true);
            holder.editCascos.setAlpha(1.0f);
            holder.itemView.setAlpha(1.0f);
            holder.editCascos.setHint("0-" + maxCascos); // Ayuda visual

            holder.checkBox.setOnCheckedChangeListener(null);
            holder.checkBox.setChecked(mSelectedState.containsKey(matricula) && mSelectedState.get(matricula));

            holder.checkBox.setOnCheckedChangeListener((buttonView, isChecked) -> {
                mSelectedState.put(matricula, isChecked);
            });
        }

        // Restaurar valor previo
        holder.editCascos.removeTextChangedListener(holder.textWatcher);
        String cascosVal = mCascosState.get(matricula);
        holder.editCascos.setText(cascosVal != null ? cascosVal : "0");

        // Nuevo TextWatcher con validación
        holder.textWatcher = new TextWatcher() {
            @Override public void beforeTextChanged(CharSequence s, int start, int count, int after) {}
            @Override public void onTextChanged(CharSequence s, int start, int before, int count) {}

            @Override
            public void afterTextChanged(Editable s) {
                // Validación de rango
                try {
                    String input = s.toString();
                    if (!input.isEmpty()) {
                        int val = Integer.parseInt(input);
                        if (val > maxCascos) {
                            // Si supera el máximo, lo corregimos automáticamente
                            s.replace(0, s.length(), String.valueOf(maxCascos));
                        } else if (val < 0) {
                            s.replace(0, s.length(), "0");
                        }
                    }
                } catch (NumberFormatException e) {
                    // Si no es número, limpiar
                    if (s.length() > 0) s.clear();
                }
                mCascosState.put(matricula, s.toString());
            }
        };
        holder.editCascos.addTextChangedListener(holder.textWatcher);
    }

    @Override
    public int getItemCount() { return mQuads != null ? mQuads.size() : 0; }

    static class ViewHolder extends RecyclerView.ViewHolder {
        CheckBox checkBox;
        EditText editCascos;
        TextWatcher textWatcher;

        ViewHolder(View itemView) {
            super(itemView);
            checkBox = itemView.findViewById(R.id.checkbox_quad);
            editCascos = itemView.findViewById(R.id.edit_text_num_cascos);
        }
    }
}