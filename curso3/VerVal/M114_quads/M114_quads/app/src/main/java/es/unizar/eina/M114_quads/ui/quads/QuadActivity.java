package es.unizar.eina.M114_quads.ui.quads;

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
import es.unizar.eina.M114_quads.database.Quad;
import es.unizar.eina.M114_quads.R;
import java.util.List;

public class QuadActivity extends AppCompatActivity {

    static final int DELETE_ID = Menu.FIRST;
    static final int EDIT_ID = Menu.FIRST + 1;

    private QuadViewModel mQuadViewModel;
    private RecyclerView mRecyclerView;
    private QuadListAdapter mAdapter;

    private Button btnSortMatricula;
    private Button btnSortTipo;
    private Button btnSortPrecio;

    private SearchView mSearchView;

    // Variable para alternar el orden del precio
    private boolean isPriceAsc = true;

    private LiveData<List<Quad>> mCurrentQuadsLiveData;
    private final Observer<List<Quad>> mQuadsObserver = quads -> mAdapter.submitList(quads);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_quad);

        MaterialToolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setDisplayShowTitleEnabled(false);
        }

        mRecyclerView = findViewById(R.id.recyclerview);
        mAdapter = new QuadListAdapter(new QuadListAdapter.QuadDiff(), new QuadListAdapter.OnQuadActionListener() {
            @Override
            public void onEdit(Quad quad) { editQuad(quad); }
            @Override
            public void onDelete(Quad quad) { confirmarBorrado(quad); }
        });

        mRecyclerView.setAdapter(mAdapter);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        mQuadViewModel = new ViewModelProvider(this).get(QuadViewModel.class);

        btnSortMatricula = findViewById(R.id.btn_sort_matricula);
        btnSortTipo = findViewById(R.id.btn_sort_tipo);
        btnSortPrecio = findViewById(R.id.btn_sort_precio);

        mSearchView = findViewById(R.id.search_view);
        setupSearchView();

        // Carga inicial
        sortByMatricula();

        btnSortMatricula.setOnClickListener(v -> sortByMatricula());
        btnSortTipo.setOnClickListener(v -> sortByTipo());

        // Listener con toggle
        btnSortPrecio.setOnClickListener(v -> toggleSortPrecio());

        FloatingActionButton fab = findViewById(R.id.fab);
        fab.setOnClickListener(view -> createQuad());
        registerForContextMenu(mRecyclerView);
    }

    private void setupSearchView() {
        mSearchView.clearFocus();
        mSearchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String query) { return false; }

            @Override
            public boolean onQueryTextChange(String newText) {
                if (newText != null && !newText.trim().isEmpty()) {
                    // --- CAMBIO AQUÍ: AÑADIR "%" ---
                    subscribeToQuads(mQuadViewModel.searchQuads("%" + newText + "%"));
                    // -------------------------------
                    highlightButton(null);
                } else {
                    sortByMatricula(); // O tu orden por defecto
                }
                return true;
            }
        });
    }

    private void subscribeToQuads(LiveData<List<Quad>> newLiveData) {
        if (mCurrentQuadsLiveData != null) {
            mCurrentQuadsLiveData.removeObserver(mQuadsObserver);
        }
        mCurrentQuadsLiveData = newLiveData;
        mCurrentQuadsLiveData.observe(this, mQuadsObserver);
    }

    @Override
    public boolean onSupportNavigateUp() {
        onBackPressed();
        return true;
    }

    private void confirmarBorrado(Quad quad) {
        new AlertDialog.Builder(this)
                .setTitle("Eliminar Quad")
                .setMessage("¿Borrar " + quad.getMatriQuad() + "?")
                .setPositiveButton("Eliminar", (dialog, which) -> mQuadViewModel.delete(quad))
                .setNegativeButton("Cancelar", null)
                .show();
    }

    private void sortByMatricula() {
        subscribeToQuads(mQuadViewModel.getQuadsSortedByMatricula());
        highlightButton(btnSortMatricula);
        btnSortPrecio.setText("Precio"); // Reset texto
    }

    private void sortByTipo() {
        subscribeToQuads(mQuadViewModel.getQuadsSortedByTipo());
        highlightButton(btnSortTipo);
        btnSortPrecio.setText("Precio"); // Reset texto
    }

    // --- Lógica del interruptor de precio ---
    private void toggleSortPrecio() {
        if (isPriceAsc) {
            subscribeToQuads(mQuadViewModel.getQuadsSortedByPrecioAsc());
            btnSortPrecio.setText("Precio ↑");
        } else {
            subscribeToQuads(mQuadViewModel.getQuadsSortedByPrecioDesc());
            btnSortPrecio.setText("Precio ↓");
        }
        isPriceAsc = !isPriceAsc; // Invertir para la próxima
        highlightButton(btnSortPrecio);
    }

    private void highlightButton(Button activeButton) {
        int transparent = Color.TRANSPARENT;
        int activeColor = Color.parseColor("#FFEBEE");

        btnSortMatricula.setBackgroundColor(transparent);
        btnSortTipo.setBackgroundColor(transparent);
        btnSortPrecio.setBackgroundColor(transparent);

        if (activeButton != null) {
            activeButton.setBackgroundColor(activeColor);
        }
    }

    @Override
    public boolean onContextItemSelected(MenuItem item) {
        Quad current = mAdapter.getCurrent();
        if (current == null) return super.onContextItemSelected(item);
        switch (item.getItemId()) {
            case DELETE_ID: confirmarBorrado(current); return true;
            case EDIT_ID: editQuad(current); return true;
        }
        return super.onContextItemSelected(item);
    }

    private void createQuad() {
        Intent intent = new Intent(this, QuadEdit.class);
        mStartCreateQuad.launch(intent);
    }

    private void editQuad(Quad current) {
        Intent intent = new Intent(this, QuadEdit.class);
        intent.putExtra(QuadEdit.QUAD_MATRICULA, current.getMatriQuad());
        intent.putExtra(QuadEdit.QUAD_TIPO, current.getTipo());
        intent.putExtra(QuadEdit.QUAD_DESCRIPCION, current.getDescripcion());
        intent.putExtra(QuadEdit.QUAD_PRECIO, current.getPrecioDia());
        mStartUpdateQuad.launch(intent);
    }

    ActivityResultLauncher<Intent> mStartCreateQuad = registerForActivityResult(
            new ActivityResultContracts.StartActivityForResult(),
            result -> {
                if (result.getResultCode() == RESULT_OK) {
                    Bundle extras = result.getData().getExtras();
                    if (extras != null) {
                        String matricula = extras.getString(QuadEdit.QUAD_MATRICULA);
                        String tipo = extras.getString(QuadEdit.QUAD_TIPO);
                        String descripcion = extras.getString(QuadEdit.QUAD_DESCRIPCION);
                        double precio = extras.getDouble(QuadEdit.QUAD_PRECIO, 0.0);
                        Quad quad = new Quad(matricula, tipo, descripcion, precio);
                        mQuadViewModel.insert(quad);
                    }
                }
            });

    ActivityResultLauncher<Intent> mStartUpdateQuad = registerForActivityResult(
            new ActivityResultContracts.StartActivityForResult(),
            result -> {
                if (result.getResultCode() == RESULT_OK) {
                    Bundle extras = result.getData().getExtras();
                    if (extras != null) {
                        String matricula = extras.getString(QuadEdit.QUAD_MATRICULA);
                        String tipo = extras.getString(QuadEdit.QUAD_TIPO);
                        String descripcion = extras.getString(QuadEdit.QUAD_DESCRIPCION);
                        double precio = extras.getDouble(QuadEdit.QUAD_PRECIO, 0.0);
                        Quad quad = new Quad(matricula, tipo, descripcion, precio);
                        mQuadViewModel.update(quad);
                    }
                }
            });
}