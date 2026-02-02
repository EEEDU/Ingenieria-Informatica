package es.unizar.eina.M114_quads.ui.reservas;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProvider;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import androidx.appcompat.widget.SearchView;

import com.google.android.material.appbar.MaterialToolbar;
import com.google.android.material.floatingactionbutton.FloatingActionButton;

import es.unizar.eina.M114_quads.database.Reserva;
import es.unizar.eina.M114_quads.R;
import es.unizar.eina.M114_quads.send.SendAbstraction;
import es.unizar.eina.M114_quads.send.SendAbstractionImpl;
import java.util.List;

public class ReservaActivity extends AppCompatActivity {

    static final int DELETE_ID = Menu.FIRST;
    static final int EDIT_ID = Menu.FIRST + 1;
    static final int SEND_ID = Menu.FIRST + 2;

    private ReservaViewModel mReservaViewModel;
    private RecyclerView mRecyclerView;
    private ReservaListAdapter mAdapter;

    // Botones
    private Button btnSortNombre;
    private Button btnSortTelf;
    private Button btnSortRecogida;   // Nuevo
    private Button btnSortDevolucion; // Nuevo
    private Button btnSortPrecio;

    private boolean isPriceSortAsc = true;
    private SearchView mSearchView;
    private LiveData<List<Reserva>> mCurrentReservasLiveData;
    private final Observer<List<Reserva>> mReservasObserver = reservas -> mAdapter.submitList(reservas);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_reserva);

        MaterialToolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setDisplayShowTitleEnabled(false);
        }

        mRecyclerView = findViewById(R.id.recyclerview);
        mAdapter = new ReservaListAdapter(new ReservaListAdapter.ReservaDiff(), new ReservaListAdapter.OnReservaActionListener() {
            @Override
            public void onEdit(Reserva reserva) { editReserva(reserva); }
            @Override
            public void onDelete(Reserva reserva) { confirmarBorrado(reserva); }
            @Override
            public void onSend(Reserva reserva) { mostrarSelectorEnvio(reserva); }
        });

        mRecyclerView.setAdapter(mAdapter);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));

        mReservaViewModel = new ViewModelProvider(this).get(ReservaViewModel.class);

        // Enlazar botones
        btnSortNombre = findViewById(R.id.btn_sort_nombre);
        btnSortTelf = findViewById(R.id.btn_sort_telf);
        btnSortRecogida = findViewById(R.id.btn_sort_recogida);     // Enlace
        btnSortDevolucion = findViewById(R.id.btn_sort_devolucion); // Enlace
        btnSortPrecio = findViewById(R.id.btn_sort_precio);

        mSearchView = findViewById(R.id.search_view);
        setupSearchView();

        // Carga inicial: Por Recogida (lo más común para ver qué entra hoy)
        sortByRecogida();

        // Listeners
        btnSortNombre.setOnClickListener(v -> sortByNombre());
        btnSortTelf.setOnClickListener(v -> sortByTelf());
        btnSortRecogida.setOnClickListener(v -> sortByRecogida());       // Listener
        btnSortDevolucion.setOnClickListener(v -> sortByDevolucion());   // Listener
        btnSortPrecio.setOnClickListener(v -> toggleSortPrecio());

        FloatingActionButton fab = findViewById(R.id.fab);
        fab.setOnClickListener(view -> createReserva());

        registerForContextMenu(mRecyclerView);
    }

    private void setupSearchView() {
        mSearchView.clearFocus();
        mSearchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override public boolean onQueryTextSubmit(String query) { return false; }
            @Override public boolean onQueryTextChange(String newText) {
                if (newText != null && !newText.trim().isEmpty()) {
                    // --- CAMBIO AQUÍ: AÑADIR "%" ---
                    subscribeToReservas(mReservaViewModel.searchReservas("%" + newText + "%"));
                    // -------------------------------
                    highlightButton(null);
                } else {
                    sortByRecogida();
                }
                return true;
            }
        });
    }

    private void subscribeToReservas(LiveData<List<Reserva>> newLiveData) {
        if (mCurrentReservasLiveData != null) mCurrentReservasLiveData.removeObserver(mReservasObserver);
        mCurrentReservasLiveData = newLiveData;
        mCurrentReservasLiveData.observe(this, mReservasObserver);
    }

    @Override
    public boolean onSupportNavigateUp() { onBackPressed(); return true; }

    // --- MÉTODOS DE ORDENACIÓN ---

    private void sortByNombre() {
        subscribeToReservas(mReservaViewModel.getReservasSortedByNombre());
        highlightButton(btnSortNombre);
        btnSortPrecio.setText("Precio");
    }

    private void sortByTelf() {
        subscribeToReservas(mReservaViewModel.getReservasSortedByTelefono());
        highlightButton(btnSortTelf);
        btnSortPrecio.setText("Precio");
    }

    private void sortByRecogida() {
        subscribeToReservas(mReservaViewModel.getReservasSortedByRecogida());
        highlightButton(btnSortRecogida);
        btnSortPrecio.setText("Precio");
    }

    private void sortByDevolucion() {
        subscribeToReservas(mReservaViewModel.getReservasSortedByDevolucion());
        highlightButton(btnSortDevolucion);
        btnSortPrecio.setText("Precio");
    }

    private void toggleSortPrecio() {
        if (isPriceSortAsc) {
            subscribeToReservas(mReservaViewModel.getReservasSortedByPrecioAsc());
            btnSortPrecio.setText("Precio ↑");
        } else {
            subscribeToReservas(mReservaViewModel.getReservasSortedByPrecioDesc());
            btnSortPrecio.setText("Precio ↓");
        }
        isPriceSortAsc = !isPriceSortAsc;
        highlightButton(btnSortPrecio);
    }

    private void highlightButton(Button activeButton) {
        int transparent = Color.TRANSPARENT;
        int activeColor = Color.parseColor("#FFEBEE");
        btnSortNombre.setBackgroundColor(transparent);
        btnSortTelf.setBackgroundColor(transparent);
        btnSortRecogida.setBackgroundColor(transparent);
        btnSortDevolucion.setBackgroundColor(transparent);
        btnSortPrecio.setBackgroundColor(transparent);

        if (activeButton != null) activeButton.setBackgroundColor(activeColor);
    }

    // ... (Resto de métodos: mostrarSelectorEnvio, enviarReserva, confirmarBorrado, contextMenu, create/editReserva y Launchers igual que antes) ...

    private void mostrarSelectorEnvio(Reserva reserva) {
        String[] opciones = {"WhatsApp", "SMS"};
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Enviar detalles de la reserva por:");
        builder.setItems(opciones, (dialog, which) -> {
            // CORRECCIÓN: Si es 0 (WhatsApp) asignamos "Whatsapp", si no "SMS"
            String metodo = (which == 0) ? "Whatsapp" : "SMS";
            enviarReserva(reserva, metodo);
        });
        builder.show();
    }

    private void enviarReserva(Reserva reserva, String metodo) {
        String mensaje = "Hola " + reserva.getNombreCliente() + ",\n" +
                "Su reserva está confirmada:\n" +
                "Desde: " + reserva.getFechaDeRecogida() + "\n" +
                "Hasta: " + reserva.getFechaDeDevolucion() + "\n" +
                "Precio Total: " + reserva.getPrecioTotal() + "€\n" +
                "Gracias por confiar en QuadShop.";
        SendAbstraction sender = new SendAbstractionImpl(this, metodo);
        sender.send(reserva.getTelfCliente(), mensaje);
    }

    private void confirmarBorrado(Reserva reserva) {
        new AlertDialog.Builder(this)
                .setTitle("Eliminar Reserva")
                .setMessage("¿Seguro que quieres borrar la reserva de " + reserva.getNombreCliente() + "?")
                .setPositiveButton("Eliminar", (dialog, which) -> {
                    mReservaViewModel.delete(reserva);
                })
                .setNegativeButton("Cancelar", null)
                .show();
    }

    @Override
    public boolean onContextItemSelected(MenuItem item) {
        Reserva current = mAdapter.getCurrent();
        if (current == null) return super.onContextItemSelected(item);
        switch (item.getItemId()) {
            case DELETE_ID: confirmarBorrado(current); return true;
            case EDIT_ID: editReserva(current); return true;
        }
        return super.onContextItemSelected(item);
    }

    private void createReserva() {
        Intent intent = new Intent(this, ReservaEdit.class);
        mStartCreateReserva.launch(intent);
    }

    private void editReserva(Reserva current) {
        Intent intent = new Intent(this, ReservaEdit.class);
        intent.putExtra(ReservaEdit.RESERVA_ID, current.getIdReserva());
        intent.putExtra(ReservaEdit.RESERVA_NOMBRE_CLIENTE, current.getNombreCliente());
        intent.putExtra(ReservaEdit.RESERVA_TELF_CLIENTE, current.getTelfCliente());
        intent.putExtra(ReservaEdit.RESERVA_FECHA_RECOGIDA, current.getFechaDeRecogida());
        intent.putExtra(ReservaEdit.RESERVA_FECHA_DEVOLUCION, current.getFechaDeDevolucion());
        intent.putExtra(ReservaEdit.RESERVA_PRECIO_TOTAL, current.getPrecioTotal());
        mStartUpdateReserva.launch(intent);
    }

    ActivityResultLauncher<Intent> mStartCreateReserva = registerForActivityResult(
            new ActivityResultContracts.StartActivityForResult(), result -> {
                if (result.getResultCode() == RESULT_OK && result.getData() != null) {
                    Bundle extras = result.getData().getExtras();
                    Reserva r = new Reserva(
                            extras.getString(ReservaEdit.RESERVA_NOMBRE_CLIENTE),
                            extras.getString(ReservaEdit.RESERVA_TELF_CLIENTE),
                            extras.getString(ReservaEdit.RESERVA_FECHA_RECOGIDA),
                            extras.getString(ReservaEdit.RESERVA_FECHA_DEVOLUCION),
                            extras.getDouble(ReservaEdit.RESERVA_PRECIO_TOTAL));
                    mReservaViewModel.insert(r);
                }
            });

    ActivityResultLauncher<Intent> mStartUpdateReserva = registerForActivityResult(
            new ActivityResultContracts.StartActivityForResult(), result -> {
                if (result.getResultCode() == RESULT_OK && result.getData() != null) {
                    Bundle extras = result.getData().getExtras();
                    Reserva r = new Reserva(
                            extras.getString(ReservaEdit.RESERVA_NOMBRE_CLIENTE),
                            extras.getString(ReservaEdit.RESERVA_TELF_CLIENTE),
                            extras.getString(ReservaEdit.RESERVA_FECHA_RECOGIDA),
                            extras.getString(ReservaEdit.RESERVA_FECHA_DEVOLUCION),
                            extras.getDouble(ReservaEdit.RESERVA_PRECIO_TOTAL));
                    r.setIdReserva(extras.getInt(ReservaEdit.RESERVA_ID));
                    mReservaViewModel.update(r);
                }
            });
}