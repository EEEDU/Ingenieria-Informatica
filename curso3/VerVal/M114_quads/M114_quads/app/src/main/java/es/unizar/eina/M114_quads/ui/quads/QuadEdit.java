package es.unizar.eina.M114_quads.ui.quads;

import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProvider;
import androidx.lifecycle.LiveData;
import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import com.google.android.material.appbar.MaterialToolbar;
import es.unizar.eina.M114_quads.R;
import es.unizar.eina.M114_quads.database.Quad;

public class QuadEdit extends AppCompatActivity {

    public static final String QUAD_MATRICULA = "matricula";
    public static final String QUAD_TIPO = "tipo";
    public static final String QUAD_DESCRIPCION = "descripcion";
    public static final String QUAD_PRECIO = "precio";

    private static final String REGEX_MATRICULA = "^\\d{4}[A-Z]{3}$";

    private EditText mMatriculaText;
    private Spinner mTipoSpinner;
    private EditText mDescripcionText;
    private EditText mPrecioText;
    private Button mSaveButton;
    private TextView mToolbarTitle;

    private QuadViewModel mQuadViewModel;
    private String mOriginalMatricula;
    private final String[] TIPOS_QUAD = {"Monoplaza", "Biplaza"};

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_quad_edit);

        // --- CONFIGURACIÓN TOOLBAR ---
        MaterialToolbar toolbar = findViewById(R.id.toolbar);
        mToolbarTitle = findViewById(R.id.toolbar_title);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setDisplayShowTitleEnabled(false);
        }
        // -----------------------------

        mQuadViewModel = new ViewModelProvider(this).get(QuadViewModel.class);

        mMatriculaText = findViewById(R.id.edit_matricula);
        mTipoSpinner = findViewById(R.id.spinner_tipo);
        mDescripcionText = findViewById(R.id.edit_descripcion);
        mPrecioText = findViewById(R.id.edit_precio_dia);
        mSaveButton = findViewById(R.id.button_save);

        ArrayAdapter<String> adapter = new ArrayAdapter<>(this,
                android.R.layout.simple_spinner_item, TIPOS_QUAD);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mTipoSpinner.setAdapter(adapter);

        mSaveButton.setOnClickListener(view -> {
            String matricula = mMatriculaText.getText().toString().trim().toUpperCase();
            String tipo = mTipoSpinner.getSelectedItem().toString();
            String descripcion = mDescripcionText.getText().toString().trim();
            String precioStr = mPrecioText.getText().toString().trim();

            if (TextUtils.isEmpty(matricula)) {
                mMatriculaText.setError("La matrícula es obligatoria");
                return;
            }
            if (!matricula.matches(REGEX_MATRICULA)) {
                mMatriculaText.setError("Formato inválido. Debe ser 1111AAA");
                return;
            }

            double precio = 0.0;
            try {
                precio = Double.parseDouble(precioStr);
                if (precio <= 0) {
                    mPrecioText.setError("Precio debe ser positivo");
                    return;
                }
            } catch (NumberFormatException e) {
                mPrecioText.setError("Precio inválido");
                return;
            }

            if (mOriginalMatricula == null) {
                checkDuplicateAndSave(matricula, tipo, descripcion, precio);
            } else {
                finishWithResult(matricula, tipo, descripcion, precio);
            }
        });

        populateFields();
    }

    // --- FLECHA ATRÁS ---
    @Override
    public boolean onSupportNavigateUp() {
        onBackPressed();
        return true;
    }

    private void checkDuplicateAndSave(String matricula, String tipo, String descripcion, double precio) {
        LiveData<Quad> quadLiveData = mQuadViewModel.getQuadByMatricula(matricula);
        quadLiveData.observe(this, new Observer<Quad>() {
            @Override
            public void onChanged(Quad quad) {
                quadLiveData.removeObserver(this);
                if (quad != null) {
                    mMatriculaText.setError("Esta matrícula ya existe");
                } else {
                    finishWithResult(matricula, tipo, descripcion, precio);
                }
            }
        });
    }

    private void finishWithResult(String matricula, String tipo, String descripcion, double precio) {
        Intent replyIntent = new Intent();
        replyIntent.putExtra(QUAD_MATRICULA, matricula);
        replyIntent.putExtra(QUAD_TIPO, tipo);
        replyIntent.putExtra(QUAD_DESCRIPCION, descripcion);
        replyIntent.putExtra(QUAD_PRECIO, precio);
        setResult(RESULT_OK, replyIntent);
        finish();
    }

    private void populateFields() {
        Bundle extras = getIntent().getExtras();
        if (extras != null && extras.containsKey(QUAD_MATRICULA)) {
            // --- MODO EDICIÓN ---
            mOriginalMatricula = extras.getString(QUAD_MATRICULA);
            mMatriculaText.setText(mOriginalMatricula);
            mDescripcionText.setText(extras.getString(QUAD_DESCRIPCION));
            mPrecioText.setText(String.valueOf(extras.getDouble(QUAD_PRECIO)));

            String currentTipo = extras.getString(QUAD_TIPO);
            if (currentTipo != null && currentTipo.equalsIgnoreCase("Biplaza")) {
                mTipoSpinner.setSelection(1);
            } else {
                mTipoSpinner.setSelection(0);
            }

            // AQUÍ ESTÁN LOS CAMBIOS PARA BLOQUEAR LA EDICIÓN:
            mMatriculaText.setEnabled(false); // Bloquea Matrícula
            mPrecioText.setEnabled(false);    // Bloquea Precio (NUEVO)

            if (mToolbarTitle != null) mToolbarTitle.setText("Editar Quad");
        } else {
            // --- MODO CREACIÓN ---
            // Aseguramos que estén habilitados por si acaso
            mMatriculaText.setEnabled(true);
            mPrecioText.setEnabled(true);

            if (mToolbarTitle != null) mToolbarTitle.setText("Crear Quad");
        }
    }
}