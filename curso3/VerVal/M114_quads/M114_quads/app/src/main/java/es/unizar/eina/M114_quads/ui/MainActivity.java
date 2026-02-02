package es.unizar.eina.M114_quads.ui;

import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import androidx.appcompat.app.AppCompatActivity;
import com.google.android.material.appbar.MaterialToolbar;
import androidx.appcompat.app.AppCompatDelegate;
import es.unizar.eina.M114_quads.R;
import es.unizar.eina.M114_quads.ui.quads.QuadActivity;
import es.unizar.eina.M114_quads.ui.reservas.ReservaActivity;
import es.unizar.eina.M114_quads.ui.TestActivity;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO);

        // --- TOOLBAR ---
        // En el menú principal, la flecha atrás servirá para "Cerrar Sesión" (volver al Login)
        MaterialToolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setDisplayShowTitleEnabled(false);
        }
        // ---------------

        setupButtons();
    }

    @Override
    public boolean onSupportNavigateUp() {
        // Volver al Login
        onBackPressed();
        return true;
    }

    private void setupButtons() {
        Button btnQuads = findViewById(R.id.btn_quads);
        Button btnReservas = findViewById(R.id.btn_reservas);
        Button btnTests = findViewById(R.id.btn_tests); // Botón nuevo

        // Navegación a gestión de Quads
        btnQuads.setOnClickListener(v -> startActivity(new Intent(this, QuadActivity.class)));

        // Navegación a gestión de Reservas
        btnReservas.setOnClickListener(v -> startActivity(new Intent(this, ReservaActivity.class)));

        // Navegación al Panel de Pruebas
        btnTests.setOnClickListener(v -> startActivity(new Intent(this, TestActivity.class)));
    }
}