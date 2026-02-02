package es.unizar.eina.M114_quads.database;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.PrimaryKey;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Locale;

@Entity(tableName = "Reservas")
public class Reserva {

    @PrimaryKey(autoGenerate = true)
    @ColumnInfo(name = "IDReserva")
    private int idReserva;

    @NonNull
    @ColumnInfo(name = "NombreCliente")
    private String nombreCliente;

    @ColumnInfo(name = "telfCliente")
    private String telfCliente;

    @NonNull
    @ColumnInfo(name = "fechaDeRecogida")
    private String fechaDeRecogida;

    @NonNull
    @ColumnInfo(name = "fechaDeDevolucion")
    private String fechaDeDevolucion;

    @ColumnInfo(name = "precioTotal")
    private double precioTotal;

    public Reserva(@NonNull String nombreCliente, String telfCliente, @NonNull String fechaDeRecogida, @NonNull String fechaDeDevolucion, double precioTotal) {
        // --- VALIDACIONES DE LÓGICA DE NEGOCIO ---

        if (nombreCliente == null || nombreCliente.trim().isEmpty()) {
            throw new IllegalArgumentException("El nombre no puede estar vacío");
        }

        if (telfCliente == null || telfCliente.trim().isEmpty()) {
            throw new IllegalArgumentException("El teléfono no puede estar vacío");
        }

        // 1. Validar que no sean nulas
        if (fechaDeRecogida == null || fechaDeDevolucion == null) {
            throw new IllegalArgumentException("Las fechas no pueden ser nulas");
        }

        // 2. NUEVO: Validar formato y existencia real (ej. no existe 30 de Febrero)
        validarFormatoFecha(fechaDeRecogida);
        validarFormatoFecha(fechaDeDevolucion);

        // 3. Coherencia (Devolución >= Recogida)
        if (fechaDeDevolucion.compareTo(fechaDeRecogida) < 0) {
            throw new IllegalArgumentException("La fecha de devolución no puede ser anterior a la de recogida");
        }

        if (precioTotal < 0) {
            throw new IllegalArgumentException("El precio total no puede ser negativo");
        }
        // -----------------------------------------

        this.nombreCliente = nombreCliente;
        this.telfCliente = telfCliente;
        this.fechaDeRecogida = fechaDeRecogida;
        this.fechaDeDevolucion = fechaDeDevolucion;
        this.precioTotal = precioTotal;
    }

    // --- NUEVO MÉTODO DE VALIDACIÓN ESTRICTA ---
    private void validarFormatoFecha(String fecha) {
        // Formato esperado: Año-Mes-Dia (ej: 2024-12-31)
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
        sdf.setLenient(false); // IMPORTANTE: Modo estricto (rechaza 30-Feb, mes 13, etc.)
        try {
            sdf.parse(fecha);
        } catch (ParseException e) {
            throw new IllegalArgumentException("Formato de fecha inválido o fecha inexistente (" + fecha + "). Use yyyy-MM-dd.");
        }
    }

    // --- Getters y Setters ---
    public int getIdReserva() { return this.idReserva; }
    public void setIdReserva(int idReserva) { this.idReserva = idReserva; }
    @NonNull public String getNombreCliente() { return this.nombreCliente; }
    public String getTelfCliente() { return this.telfCliente; }
    @NonNull public String getFechaDeRecogida() { return this.fechaDeRecogida; }
    @NonNull public String getFechaDeDevolucion() { return this.fechaDeDevolucion; }
    public double getPrecioTotal() { return this.precioTotal; }
    public void setPrecioTotal(double precioTotal) { this.precioTotal = precioTotal; }
}