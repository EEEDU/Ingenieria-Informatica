package es.unizar.eina.M114_quads.send;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.widget.Toast;
import java.net.URLEncoder;

/** Concrete implementor utilizando la aplicación de WhatsApp. */
public class WhatsAppImplementor implements SendImplementor {

   /** actividad desde la cual se abrirá la aplicación de WhatsApp */
   private Activity sourceActivity;

   /** Constructor */
   public WhatsAppImplementor(Activity source){
      setSourceActivity(source);
   }

   /** Actualiza la actividad */
   public void setSourceActivity(Activity source) {
      sourceActivity = source;
   }

   /** Recupera la actividad */
   public Activity getSourceActivity(){
      return sourceActivity;
   }

   /**
    * Implementacion del metodo send utilizando la API URL de WhatsApp
    * @param phone teléfono (Debe incluir código de país, ejemplo: "+34666...")
    * @param message cuerpo del mensaje
    */
   @Override
   public void send(String phone, String message) {
      boolean app_installed = false;
      PackageManager pm = getSourceActivity().getPackageManager();
      try {
         pm.getPackageInfo("com.whatsapp", PackageManager.GET_ACTIVITIES);
         app_installed = true;
      } catch (PackageManager.NameNotFoundException e) {
         app_installed = false;
      }

      if (app_installed) {
         try {
            // 1. Limpiar el teléfono: WhatsApp API requiere el número SIN el simbolo '+' ni espacios,
            // pero CON el código de país. Tu base de datos guarda por ejemplo "+34 666...",
            // así que quitamos el '+' y los espacios en blanco.
            String sNum = phone.replace("+", "").replace(" ", "").trim();

            // 2. Codificar el mensaje para URL (gestiona espacios, tildes, saltos de linea)
            String url = "https://api.whatsapp.com/send?phone=" + sNum + "&text=" + URLEncoder.encode(message, "UTF-8");

            // 3. Lanzar el Intent con ACTION_VIEW
            Intent sendIntent = new Intent(Intent.ACTION_VIEW);
            sendIntent.setData(Uri.parse(url));
            sendIntent.setPackage("com.whatsapp"); // Asegura que abra la app y no el navegador
            getSourceActivity().startActivity(sendIntent);

         } catch (Exception e) {
            Toast.makeText(getSourceActivity(), "Error al preparar WhatsApp", Toast.LENGTH_SHORT).show();
            e.printStackTrace();
         }
      } else {
         Toast.makeText(getSourceActivity(), "WhatsApp no está instalado", Toast.LENGTH_SHORT).show();
      }
   }
}