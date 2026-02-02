package es.unizar.eina.M114_quads.database;

import android.content.Context;

import androidx.annotation.NonNull;
import androidx.room.Database;
import androidx.room.Room;
import androidx.room.RoomDatabase;
import androidx.sqlite.db.SupportSQLiteDatabase;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

// CAMBIO: Versión subida a 4
@Database(entities = {Quad.class, Reserva.class, RelacionQuadsReservas.class},
        version = 4,
        exportSchema = false)
public abstract class TiendaQuadsRoomDatabase extends RoomDatabase {

    public abstract QuadDao quadDao();
    public abstract ReservaDao reservaDao();
    public abstract RelacionQuadsReservasDao relacionDao();

    private static volatile TiendaQuadsRoomDatabase INSTANCE;
    private static final int NUMBER_OF_THREADS = 4;
    static final ExecutorService databaseWriteExecutor =
            Executors.newFixedThreadPool(NUMBER_OF_THREADS);

    static TiendaQuadsRoomDatabase getDatabase(final Context context) {
        if (INSTANCE == null) {
            synchronized (TiendaQuadsRoomDatabase.class) {
                if (INSTANCE == null) {
                    INSTANCE = Room.databaseBuilder(context.getApplicationContext(),
                                    TiendaQuadsRoomDatabase.class, "tienda_quads_database")
                            .addCallback(sRoomDatabaseCallback)
                            // IMPORTANTE: Esto permite destruir la BD vieja si la versión cambia
                            .fallbackToDestructiveMigration()
                            .build();
                }
            }
        }
        return INSTANCE;
    }

    private static RoomDatabase.Callback sRoomDatabaseCallback = new RoomDatabase.Callback() {
        @Override
        public void onCreate(@NonNull SupportSQLiteDatabase db) {
            super.onCreate(db);
            databaseWriteExecutor.execute(() -> {
                QuadDao dao = INSTANCE.quadDao();
                dao.deleteAll();

                // Datos de prueba
                Quad quad = new Quad("1234ABC", "monoplaza", "Quad rojo modelo X", 50.0);
                dao.insert(quad);
                quad = new Quad("5678XYZ", "biplaza", "Quad azul modelo Y", 75.0);
                dao.insert(quad);

                ReservaDao reservaDao = INSTANCE.reservaDao();
                reservaDao.deleteAll();
            });
        }
    };
}