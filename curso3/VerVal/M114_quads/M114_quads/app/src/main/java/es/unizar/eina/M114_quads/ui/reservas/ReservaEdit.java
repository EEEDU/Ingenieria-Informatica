package es.unizar.eina.M114_quads.ui.reservas;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.appcompat.app.AppCompatActivity;
import android.app.DatePickerDialog;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;
import com.google.android.material.appbar.MaterialToolbar;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;
import es.unizar.eina.M114_quads.R;

public class ReservaEdit extends AppCompatActivity {

    public static final String RESERVA_ID = "idReserva";
    public static final String RESERVA_NOMBRE_CLIENTE = "nombreCliente";
    public static final String RESERVA_TELF_CLIENTE = "telfCliente";
    public static final String RESERVA_FECHA_RECOGIDA = "fechaDeRecogida";
    public static final String RESERVA_FECHA_DEVOLUCION = "fechaDeDevolucion";
    public static final String RESERVA_PRECIO_TOTAL = "precioTotal";

    private EditText mNombreClienteText;
    private EditText mTelfClienteText;
    private Spinner mPrefijoSpinner;
    private EditText mFechaRecogidaText;
    private EditText mFechaDevolucionText;
    private EditText mPrecioTotalText;
    private TextView mToolbarTitle;

    private Button mSaveButton;
    private Button mManageQuadsButton;

    private int mOriginalId = -1;
    private double mPrecioActual = 0.0;
    private double mPrecioDia = 0.0;

    private final String[] PREFIJOS = {
            "🇪🇸 +34", "🇫🇷 +33", "🇬🇧 +44", "🇺🇸 +1", "🇩🇪 +49", "🇮🇹 +39", "🇵🇹 +351"
    };

    private ActivityResultLauncher<Intent> mManageQuadsLauncher;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_reserva_edit);

        MaterialToolbar toolbar = findViewById(R.id.toolbar);
        mToolbarTitle = findViewById(R.id.toolbar_title);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setDisplayShowTitleEnabled(false);
        }

        mNombreClienteText = findViewById(R.id.edit_nombre_cliente);
        mTelfClienteText = findViewById(R.id.edit_telf_cliente);
        mPrefijoSpinner = findViewById(R.id.spinner_prefijo_pais);

        ArrayAdapter<String> adapter = new ArrayAdapter<>(this,
                android.R.layout.simple_spinner_item, PREFIJOS);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mPrefijoSpinner.setAdapter(adapter);

        mFechaRecogidaText = findViewById(R.id.edit_fecha_recogida);
        mFechaDevolucionText = findViewById(R.id.edit_fecha_devolucion);

        mFechaRecogidaText.setOnClickListener(v -> showDatePickerDialog(mFechaRecogidaText));
        mFechaDevolucionText.setOnClickListener(v -> showDatePickerDialog(mFechaDevolucionText));

        mPrecioTotalText = findViewById(R.id.edit_precio_total);

        mManageQuadsLauncher = registerForActivityResult(
                new ActivityResultContracts.StartActivityForResult(),
                result -> {
                    if (result.getResultCode() == RESULT_OK && result.getData() != null) {
                        mPrecioActual = result.getData().getDoubleExtra(SeleccionarQuadsActivity.EXTRA_PRECIO_CALCULADO, mPrecioActual);
                        mPrecioDia = result.getData().getDoubleExtra(SeleccionarQuadsActivity.EXTRA_PRECIO_DIA, 0.0);
                        mPrecioTotalText.setText(String.format(Locale.getDefault(), "%.2f", mPrecioActual));
                        recalcularTotal();
                    }
                });

        mManageQuadsButton = findViewById(R.id.button_manage_quads);
        mManageQuadsButton.setOnClickListener(v -> {
            if (mOriginalId != -1) {
                if (!validarFechas()) return;

                Intent intent = new Intent(ReservaEdit.this, SeleccionarQuadsActivity.class);
                intent.putExtra(SeleccionarQuadsActivity.EXTRA_ID_RESERVA, mOriginalId);
                intent.putExtra("extra_fecha_inicio", mFechaRecogidaText.getText().toString());
                intent.putExtra("extra_fecha_fin", mFechaDevolucionText.getText().toString());

                mManageQuadsLauncher.launch(intent);
            } else {
                Toast.makeText(this, "Guarda la reserva antes de asignar quads", Toast.LENGTH_SHORT).show();
            }
        });

        mSaveButton = findViewById(R.id.button_save);
        mSaveButton.setOnClickListener(view -> {
            Intent replyIntent = new Intent();
            String telefonoCompleto = obtenerTelefonoCompleto();
            if (telefonoCompleto == null) return;

            if (!validarFechas()) return;

            if (mOriginalId != -1) {
                replyIntent.putExtra(RESERVA_ID, mOriginalId);
            }
            String nombreCliente = mNombreClienteText.getText().toString().trim();
            if (nombreCliente.isEmpty()) nombreCliente = "Sin nombre";

            replyIntent.putExtra(RESERVA_NOMBRE_CLIENTE, nombreCliente);
            replyIntent.putExtra(RESERVA_TELF_CLIENTE, telefonoCompleto);
            replyIntent.putExtra(RESERVA_FECHA_RECOGIDA, mFechaRecogidaText.getText().toString());
            replyIntent.putExtra(RESERVA_FECHA_DEVOLUCION, mFechaDevolucionText.getText().toString());

            replyIntent.putExtra(RESERVA_PRECIO_TOTAL, mPrecioActual);

            setResult(RESULT_OK, replyIntent);
            finish();
        });

        populateFields();
    }

    // --- CORRECCIÓN CLAVE: CANCELAR AL VOLVER ATRÁS ---
    @Override
    public boolean onSupportNavigateUp() {
        // Al pulsar la flecha de la barra superior, indicamos explícitamente CANCELAR
        setResult(RESULT_CANCELED);
        finish();
        return true;
    }

    // Opcional: También cubrimos el botón físico "Atrás" del móvil
    @Override
    public void onBackPressed() {
        setResult(RESULT_CANCELED);
        super.onBackPressed();
    }
    // --------------------------------------------------

    private void showDatePickerDialog(final EditText targetEditText) {
        final Calendar c = Calendar.getInstance();
        int year = c.get(Calendar.YEAR);
        int month = c.get(Calendar.MONTH);
        int day = c.get(Calendar.DAY_OF_MONTH);

        String currentText = targetEditText.getText().toString();
        if (!currentText.isEmpty()) {
            try {
                SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
                Date date = sdf.parse(currentText);
                if (date != null) {
                    c.setTime(date);
                    year = c.get(Calendar.YEAR);
                    month = c.get(Calendar.MONTH);
                    day = c.get(Calendar.DAY_OF_MONTH);
                }
            } catch (ParseException e) {}
        }

        DatePickerDialog datePickerDialog = new DatePickerDialog(this,
                (view, selectedYear, selectedMonth, selectedDay) -> {
                    String formattedDate = String.format(Locale.getDefault(), "%04d-%02d-%02d", selectedYear, selectedMonth + 1, selectedDay);
                    targetEditText.setText(formattedDate);
                    recalcularTotal();
                }, year, month, day);

        datePickerDialog.show();
    }

    private void recalcularTotal() {
        String f1 = mFechaRecogidaText.getText().toString();
        String f2 = mFechaDevolucionText.getText().toString();

        if (f1.isEmpty() || f2.isEmpty()) return;

        long dias = calcularDias(f1, f2);

        if (dias > 0 && mPrecioDia > 0) {
            mPrecioActual = dias * mPrecioDia;
            mPrecioTotalText.setText(String.format(Locale.getDefault(), "%.2f", mPrecioActual));
        }
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

    private String obtenerTelefonoCompleto() {
        String seleccionSpinner = mPrefijoSpinner.getSelectedItem().toString();
        String[] partes = seleccionSpinner.split(" ");
        String prefijo = partes.length > 1 ? partes[1] : "+34";
        String numeroUsuario = mTelfClienteText.getText().toString().trim();

        if (numeroUsuario.isEmpty()) {
            mostrarError("El teléfono es obligatorio");
            return null;
        }
        String telefonoFinal = prefijo + numeroUsuario;
        if (!telefonoFinal.matches("^\\+[0-9]{7,15}$")) {
            mostrarError("Teléfono inválido");
            return null;
        }
        return telefonoFinal;
    }

    private boolean validarFechas() {
        String f1 = mFechaRecogidaText.getText().toString().trim();
        String f2 = mFechaDevolucionText.getText().toString().trim();

        if (!f1.matches("\\d{4}-\\d{2}-\\d{2}") || !f2.matches("\\d{4}-\\d{2}-\\d{2}")) {
            mostrarError("Fecha obligatoria");
            return false;
        }

        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
        try {
            Date d1 = sdf.parse(f1);
            Date d2 = sdf.parse(f2);
            if (!d2.after(d1) && !d2.equals(d1)) {
                mostrarError("La devolución debe ser posterior o igual a la recogida");
                return false;
            }
        } catch (ParseException e) { return false; }
        return true;
    }

    private void mostrarError(String msg) {
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }

    private void populateFields() {
        Bundle extras = getIntent().getExtras();
        if (extras != null && extras.containsKey(RESERVA_ID)) {
            mOriginalId = extras.getInt(RESERVA_ID);
            mNombreClienteText.setText(extras.getString(RESERVA_NOMBRE_CLIENTE));

            String telfCompleto = extras.getString(RESERVA_TELF_CLIENTE);
            if (telfCompleto != null && telfCompleto.startsWith("+")) {
                boolean encontrado = false;
                for (int i = 0; i < PREFIJOS.length; i++) {
                    String codigo = PREFIJOS[i].split(" ")[1];
                    if (telfCompleto.startsWith(codigo)) {
                        mPrefijoSpinner.setSelection(i);
                        String restoNumero = telfCompleto.substring(codigo.length());
                        mTelfClienteText.setText(restoNumero);
                        encontrado = true;
                        break;
                    }
                }
                if (!encontrado) mTelfClienteText.setText(telfCompleto);
            } else {
                mTelfClienteText.setText(telfCompleto);
            }

            mFechaRecogidaText.setText(extras.getString(RESERVA_FECHA_RECOGIDA));
            mFechaDevolucionText.setText(extras.getString(RESERVA_FECHA_DEVOLUCION));

            mPrecioActual = extras.getDouble(RESERVA_PRECIO_TOTAL);

            long dias = calcularDias(mFechaRecogidaText.getText().toString(), mFechaDevolucionText.getText().toString());
            if (dias > 0 && mPrecioActual > 0) {
                mPrecioDia = mPrecioActual / dias;
            } else {
                mPrecioDia = 0.0;
            }

            if (mPrecioActual > 0) {
                mPrecioTotalText.setText(String.format(Locale.getDefault(), "%.2f", mPrecioActual));
            } else {
                mPrecioTotalText.setText("0.00 (Sin Quads)");
            }

            if(mToolbarTitle != null) mToolbarTitle.setText("Editar Reserva");
            mManageQuadsButton.setVisibility(View.VISIBLE);
        } else {
            mPrecioTotalText.setText("0.00 (Guardar para calcular)");
            mPrecioActual = 0.0;
            mPrecioDia = 0.0;
            if(mToolbarTitle != null) mToolbarTitle.setText("Crear Reserva");
            mManageQuadsButton.setVisibility(View.GONE);
        }
    }
}