package es.unizar.eina.M114_quads.ui.reservas;

import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.ViewModelProvider;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import com.google.android.material.appbar.MaterialToolbar;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import es.unizar.eina.M114_quads.R;
import es.unizar.eina.M114_quads.database.RelacionQuadsReservas;
import es.unizar.eina.M114_quads.ui.RelacionQuadsReservas.RelacionQuadsReservasViewModel;
import es.unizar.eina.M114_quads.ui.quads.QuadViewModel;

public class SeleccionarQuadsActivity extends AppCompatActivity {

    public static final String EXTRA_ID_RESERVA = "extra_id_reserva";
    public static final String EXTRA_PRECIO_CALCULADO = "extra_precio_calculado";
    public static final String EXTRA_PRECIO_DIA = "extra_precio_dia";

    private QuadViewModel mQuadViewModel;
    private RelacionQuadsReservasViewModel mRelacionViewModel;
    private ReservaViewModel mReservaViewModel;
    private SeleccionarQuadAdapter mAdapter;

    private int mIdReserva;
    private String mFechaInicio;
    private String mFechaFin;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_seleccionar_quads);

        MaterialToolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setDisplayShowTitleEnabled(false);
        }

        if (getIntent().hasExtra(EXTRA_ID_RESERVA)) {
            mIdReserva = getIntent().getIntExtra(EXTRA_ID_RESERVA, -1);
            mFechaInicio = getIntent().getStringExtra("extra_fecha_inicio");
            mFechaFin = getIntent().getStringExtra("extra_fecha_fin");
        }

        if (mIdReserva == -1 || mFechaInicio == null || mFechaFin == null) {
            Toast.makeText(this, "Datos incompletos", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }

        TextView subtitulo = findViewById(R.id.text_view_subtitle);
        subtitulo.setText("Fechas: " + mFechaInicio + " / " + mFechaFin);

        RecyclerView recyclerView = findViewById(R.id.recycler_view_seleccionar_quads);
        mAdapter = new SeleccionarQuadAdapter();
        recyclerView.setLayoutManager(new LinearLayoutManager(this));
        recyclerView.setAdapter(mAdapter);

        mQuadViewModel = new ViewModelProvider(this).get(QuadViewModel.class);
        mRelacionViewModel = new ViewModelProvider(this).get(RelacionQuadsReservasViewModel.class);
        mReservaViewModel = new ViewModelProvider(this).get(ReservaViewModel.class);

        mRelacionViewModel.getRelacionesParaReserva(mIdReserva).observe(this, relaciones -> {
            if (relaciones != null) {
                mAdapter.setEstadoInicial(relaciones);
            }
        });

        mQuadViewModel.getQuadsConDisponibilidad(mFechaInicio, mFechaFin, mIdReserva)
                .observe(this, quads -> {
                    mAdapter.setQuads(quads);
                });

        Button btnConfirmar = findViewById(R.id.button_confirmar_seleccion);
        btnConfirmar.setOnClickListener(v -> guardarSeleccion());
    }

    // --- CAMBIO: Forzar RESULT_CANCELED al pulsar la flecha de la toolbar ---
    @Override
    public boolean onSupportNavigateUp() {
        setResult(RESULT_CANCELED);
        finish();
        return true;
    }

    // --- CAMBIO: Forzar RESULT_CANCELED al pulsar el botón físico "Atrás" ---
    @Override
    public void onBackPressed() {
        setResult(RESULT_CANCELED);
        super.onBackPressed();
    }

    private void guardarSeleccion() {
        Map<String, Integer> seleccionados = mAdapter.getSelectedQuads();
        List<RelacionQuadsReservas> listaAGuardar = new ArrayList<>();

        for (Map.Entry<String, Integer> entry : seleccionados.entrySet()) {
            listaAGuardar.add(new RelacionQuadsReservas(entry.getKey(), mIdReserva, entry.getValue()));
        }

        long dias = calcularDias(mFechaInicio, mFechaFin);
        double sumaPreciosDia = mAdapter.getSumaPrecioDiaSeleccionados();
        double precioTotal = sumaPreciosDia * dias;

        // Solo aquí guardamos en la BD
        mReservaViewModel.actualizarReservaCompleta(mIdReserva, listaAGuardar, precioTotal);

        Intent data = new Intent();
        data.putExtra(EXTRA_PRECIO_CALCULADO, precioTotal);
        data.putExtra(EXTRA_PRECIO_DIA, sumaPreciosDia);
        setResult(RESULT_OK, data); // Aquí sí confirmamos el éxito

        Toast.makeText(this, "Guardado. Total: " + String.format(Locale.getDefault(), "%.2f€", precioTotal), Toast.LENGTH_LONG).show();
        finish();
    }

    private long calcularDias(String f1, String f2) {
        try {
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
            Date d1 = sdf.parse(f1);
            Date d2 = sdf.parse(f2);
            long diffMs = d2.getTime() - d1.getTime();
            long dias = Math.round((double) diffMs / (24 * 60 * 60 * 1000));
            return dias + 1;
        } catch (Exception e) {
            return 0;
        }
    }
}