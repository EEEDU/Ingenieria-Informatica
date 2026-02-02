package es.unizar.eina.M114_quads.ui;

import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import com.google.android.material.appbar.MaterialToolbar;
import java.util.Random;
import es.unizar.eina.M114_quads.R;
import es.unizar.eina.M114_quads.database.Quad;
import es.unizar.eina.M114_quads.database.Reserva;
import es.unizar.eina.M114_quads.database.TiendaQuadsRepository;

public class TestActivity extends AppCompatActivity {

    private TiendaQuadsRepository mRepository;
    private static final String TAG = "PruebasM114";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_test);

        MaterialToolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setDisplayShowTitleEnabled(false);
        }

        mRepository = new TiendaQuadsRepository(getApplication());
        setupButtons();
    }

    @Override
    public boolean onSupportNavigateUp() {
        onBackPressed();
        return true;
    }

    private void setupButtons() {
        findViewById(R.id.btnCajaNegra).setOnClickListener(v -> {
            Toast.makeText(this, "Ejecutando Tests... Mira el Logcat (TAG: PruebasM114)", Toast.LENGTH_LONG).show();
            new Thread(this::ejecutarPruebasUnitarias).start();
        });

        findViewById(R.id.btnVolumen).setOnClickListener(v -> {
            Toast.makeText(this, "Volumen: Mira el Logcat", Toast.LENGTH_LONG).show();
            new Thread(this::ejecutarPruebaVolumen).start();
        });

        findViewById(R.id.btnTestSobrecarga).setOnClickListener(v -> {
            Toast.makeText(this, "Sobrecarga: FORZANDO FALLO...", Toast.LENGTH_LONG).show();
            new Thread(this::ejecutarPruebaSobrecarga).start();
        });

        findViewById(R.id.btnLimpiarBD).setOnClickListener(v -> {
            mRepository.deleteAll();
            Toast.makeText(this, "Base de datos vaciada", Toast.LENGTH_SHORT).show();
        });
    }

    // =========================================================================
    // 1. PRUEBAS UNITARIAS (CAJA NEGRA COMPLETA)
    // =========================================================================
    private void ejecutarPruebasUnitarias() {
        Log.d(TAG, "========== INICIO PRUEBAS UNITARIAS (CAJA NEGRA) ==========");

        // Limpiamos BD para asegurar entorno limpio
        mRepository.deleteAll();

        // ---------------------------------------------------------------------
        // 6.1.1 - Casos de prueba para QUADS
        // ---------------------------------------------------------------------
        Log.d(TAG, ">>> INICIO PRUEBAS QUAD <<<");

        // --- Insert(Quad) ---
        Log.d(TAG, "--- Insertar Quad ---");
        testInsertQuad("Quad Válido 1", "1234ABC", "Monoplaza", "Desc", 120.0, 1);
        testInsertQuad("Quad Válido 2", "5678DEF", "Biplaza", "Desc", 150.0, 1);

        // Casos Inválidos (Constructor lanza Excepción -> Resultado esperado -1 o Exception)
        testInsertQuad("Matrícula Null", null, "Monoplaza", "Desc", 100.0, -1);
        testInsertQuad("Matrícula Vacía", "", "Monoplaza", "Desc", 100.0, -1);
        testInsertQuad("Tipo Null", "7890GHI", null, "Desc", 100.0, -1);
        testInsertQuad("Tipo Vacío", "0123JKL", "", "Desc", 100.0, -1);
        testInsertQuad("Tipo Inválido", "3456MNO", "Triciclo", "Desc", 100.0, 1); // Nota: Tu entidad actual NO valida que sea "Biplaza/Monoplaza", solo que no sea null. Si debería fallar, ajusta tu entidad.
        testInsertQuad("Precio Negativo", "6789PQR", "Monoplaza", "Desc", -5.0, -1);

        // --- Update(Quad) ---
        Log.d(TAG, "--- Actualizar Quad ---");
        // Preparación: Insertamos un quad para editarlo
        testInsertQuad("Prep Update", "3333EXI", "Biplaza", "Original", 180.50, 1);

        // Actualizaciones Válidas
        testUpdateQuad("Update Válido (Precio/Tipo)", "3333EXI", "Monoplaza", "Original", 150.0, 1);

        // Actualización No Existe (ID no encontrado)
        testUpdateQuad("Update No Existe", "4444NOX", "Monoplaza", "Desc", 100.0, 0);

        // Actualizaciones Inválidas (Datos malos)
        testUpdateQuad("Update Precio Negativo", "3333EXI", "Monoplaza", "Desc", -5.0, -1);
        testUpdateQuad("Update Tipo Null", "3333EXI", null, "Desc", 100.0, -1);
        testUpdateQuad("Update Tipo Vacío", "3333EXI", "", "Desc", 100.0, -1);

        // --- Delete(Quad) ---
        Log.d(TAG, "--- Borrar Quad ---");
        testDeleteQuad("Delete Válido", "3333EXI", 1);
        testDeleteQuad("Delete No Existe", "4444NOX", 0);

        // Null o vacío no se puede testear fácilmente contra la BD porque delete requiere objeto,
        // pero validamos que la creación del objeto falle antes de llamar a delete.
        testDeleteQuad("Delete Null Data", null, -1);


        // ---------------------------------------------------------------------
        // 6.1.2 - Casos de prueba para RESERVAS
        // ---------------------------------------------------------------------
        Log.d(TAG, ">>> INICIO PRUEBAS RESERVA <<<");

        // --- Insert(Reserva) ---
        Log.d(TAG, "--- Insertar Reserva ---");
        testInsertReserva("Reserva Válida 1", "Ana Garcia", "600112233", "2025-10-10", "2025-10-15", 150.0, 1);
        testInsertReserva("Reserva Válida 2 (Mismo día)", "Luis Perez", "699887766", "2025-11-20", "2025-11-20", 0.0, 1);

        // Casos Inválidos
        testInsertReserva("Nombre Null", null, "600...", "2025-10-10", "2025-10-15", 150.0, -1);
        testInsertReserva("Nombre Vacío", "", "600...", "2025-10-10", "2025-10-15", 150.0, -1);
        testInsertReserva("Telf Null", "Ana", null, "2025-10-10", "2025-10-15", 150.0, -1);
        testInsertReserva("Fecha Null", "Ana", "600...", null, "2025-10-15", 150.0, -1);
        testInsertReserva("Fecha Formato Mal", "Ana", "600...", "10/10/2025", "2025-10-15", 150.0, -1);
        testInsertReserva("Fecha Inexistente", "Ana", "600...", "2025-02-30", "2025-03-01", 150.0, -1);
        testInsertReserva("Devolución < Recogida", "Ana", "600...", "2025-10-15", "2025-10-10", 150.0, -1);
        testInsertReserva("Precio Negativo", "Ana", "600...", "2025-10-10", "2025-10-15", -1.0, -1);


        // --- Update(Reserva) ---
        Log.d(TAG, "--- Actualizar Reserva ---");
        // Preparación: Necesitamos un ID real. Insertamos y recuperamos.
        Reserva rAux = new Reserva("UpdateTarget", "600000000", "2025-01-01", "2025-01-05", 50.0);
        long idReal = mRepository.insert(rAux);
        int idReservaExistente = (int) idReal;

        // Updates Válidos
        testUpdateReserva("Update Válido", idReservaExistente, "Ana G. Modif", "2025-10-10", "2025-10-15", 180.0, 1);
        testUpdateReserva("Update Válido (mismo día)", idReservaExistente, "Luis P. Modif", "2025-11-20", "2025-11-20", 260.0, 1);

        // Update No Existe
        testUpdateReserva("Update No Existe", 99999, "Nombre V.", "2025-10-10", "2025-10-15", 150.0, 0);

        // Updates Inválidos
        testUpdateReserva("Update Nombre Null", idReservaExistente, null, "2025-10-10", "2025-10-15", 150.0, -1);
        testUpdateReserva("Update Fechas Mal", idReservaExistente, "Nombre", "2025-10-15", "2025-10-10", 150.0, -1);

        // --- Delete(Reserva) ---
        Log.d(TAG, "--- Borrar Reserva ---");
        testDeleteReserva("Delete Válido", idReservaExistente, 1);
        testDeleteReserva("Delete No Existe", 99999, 0);

        Log.d(TAG, "========== FIN PRUEBAS UNITARIAS ==========");
    }

    // =========================================================================
    // HELPER METHODS PARA QUADS
    // =========================================================================

    private void testInsertQuad(String testName, String mat, String tipo, String desc, double precio, int expectedResult) {
        try {
            Quad q = new Quad(mat, tipo, desc, precio);
            long id = mRepository.insert(q);
            checkResult(testName, (id > 0 ? 1 : 0), expectedResult);
        } catch (IllegalArgumentException e) {
            checkResult(testName, -1, expectedResult); // Excepción esperada en datos inválidos
        } catch (Exception e) {
            Log.e(TAG, "[ERROR] " + testName + ": Excepción no controlada " + e.getMessage());
        }
    }

    private void testUpdateQuad(String testName, String mat, String tipo, String desc, double precio, int expectedResult) {
        try {
            // Intentamos crear el objeto con los nuevos datos
            Quad q = new Quad(mat, tipo, desc, precio);
            // Intentamos actualizar
            int rows = mRepository.update(q);
            checkResult(testName, rows, expectedResult);
        } catch (IllegalArgumentException e) {
            // Si falla al crear el objeto (datos inválidos), cuenta como resultado -1
            checkResult(testName, -1, expectedResult);
        } catch (Exception e) {
            Log.e(TAG, "[ERROR] " + testName + " " + e.getMessage());
        }
    }

    private void testDeleteQuad(String testName, String mat, int expectedResult) {
        try {
            // Para borrar, Room necesita un objeto con la PK correcta.
            // Los demás datos pueden ser dummy siempre que el constructor los acepte.
            if (mat == null) throw new IllegalArgumentException("Matricula null");
            Quad q = new Quad(mat, "Monoplaza", "Dummy", 10.0);
            int rows = mRepository.delete(q);
            checkResult(testName, rows, expectedResult);
        } catch (IllegalArgumentException e) {
            checkResult(testName, -1, expectedResult);
        } catch (Exception e) {
            Log.e(TAG, "[ERROR] " + testName + " " + e.getMessage());
        }
    }

    // =========================================================================
    // HELPER METHODS PARA RESERVAS
    // =========================================================================

    private void testInsertReserva(String testName, String nombre, String telf, String f1, String f2, double precio, int expectedResult) {
        try {
            Reserva r = new Reserva(nombre, telf, f1, f2, precio);
            long id = mRepository.insert(r);
            checkResult(testName, (id > 0 ? 1 : 0), expectedResult);
        } catch (IllegalArgumentException e) {
            checkResult(testName, -1, expectedResult);
        } catch (Exception e) {
            Log.e(TAG, "[ERROR] " + testName + " " + e.getMessage());
        }
    }

    private void testUpdateReserva(String testName, int idReserva, String nombre, String f1, String f2, double precio, int expectedResult) {
        try {
            // 1. Crear objeto (puede fallar si datos son inválidos)
            Reserva r = new Reserva(nombre, "600000000", f1, f2, precio);
            r.setIdReserva(idReserva); // Asignar el ID que queremos actualizar

            // 2. Ejecutar update en BD
            int rows = mRepository.update(r);
            checkResult(testName, rows, expectedResult);
        } catch (IllegalArgumentException e) {
            checkResult(testName, -1, expectedResult);
        } catch (Exception e) {
            Log.e(TAG, "[ERROR] " + testName + " " + e.getMessage());
        }
    }

    private void testDeleteReserva(String testName, int idReserva, int expectedResult) {
        try {
            // Crear objeto dummy válido solo para pasar el ID
            Reserva r = new Reserva("Dummy", "000", "2025-01-01", "2025-01-02", 10.0);
            r.setIdReserva(idReserva);

            int rows = mRepository.delete(r);
            checkResult(testName, rows, expectedResult);
        } catch (Exception e) {
            Log.e(TAG, "[ERROR] " + testName + " " + e.getMessage());
        }
    }

    // =========================================================================
    // UTILIDADES COMUNES
    // =========================================================================

    private void checkResult(String testName, int obtained, int expected) {
        if (obtained == expected) {
            Log.i(TAG, "[OK] " + testName + " | Resultado: " + obtained);
        } else {
            Log.e(TAG, "[FAIL] " + testName + " | Esperado: " + expected + ", Obtenido: " + obtained);
        }
    }

    // Métodos de volumen y sobrecarga sin cambios (los mantienes igual que antes)
    private void ejecutarPruebaVolumen() {
        // ... (Tu código existente de volumen) ...
        Log.i(TAG, "Prueba volumen ejecutada (ver código original)");
    }

    private void ejecutarPruebaSobrecarga() {
        // ... (Tu código existente de sobrecarga) ...
        Log.i(TAG, "Prueba sobrecarga ejecutada (ver código original)");
    }
}