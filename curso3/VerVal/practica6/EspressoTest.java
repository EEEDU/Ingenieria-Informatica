package es.unizar.eina.notepad;

import static androidx.test.espresso.Espresso.onView;
import static androidx.test.espresso.Espresso.openActionBarOverflowOrOptionsMenu;
import static androidx.test.espresso.Espresso.pressBack;
import static androidx.test.espresso.action.ViewActions.click;
import static androidx.test.espresso.action.ViewActions.closeSoftKeyboard;
import static androidx.test.espresso.action.ViewActions.longClick;
import static androidx.test.espresso.action.ViewActions.replaceText;
import static androidx.test.espresso.assertion.ViewAssertions.matches;
import static androidx.test.espresso.contrib.RecyclerViewActions.scrollTo;
import static androidx.test.espresso.matcher.ViewMatchers.hasDescendant;
import static androidx.test.espresso.matcher.ViewMatchers.isDisplayed;
import static androidx.test.espresso.matcher.ViewMatchers.withId;
import static androidx.test.espresso.matcher.ViewMatchers.withText;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.equalTo;
import static org.hamcrest.Matchers.is;

import androidx.test.core.app.ApplicationProvider;
import androidx.test.ext.junit.rules.ActivityScenarioRule;

import org.junit.After;
import org.junit.Rule;
import org.junit.Test;

import es.unizar.eina.notepad.R;
import es.unizar.eina.notepad.database.Note;
import es.unizar.eina.notepad.ui.Notepad;

public class EspressoTest {
    private static final String TEST_NAME  = "Espresso";
    private static final String TEST_TITLE = TEST_NAME + " Note Title";
    private static final String TEST_UPDATED_TITLE = TEST_NAME + " Updated Note Title";
    private static final String TEST_BODY = TEST_NAME + " Note Body";

    private static final int TEST_NUMBER_OF_NOTES = 4;

    @Rule
    public ActivityScenarioRule<Notepad> scenarioRule = new ActivityScenarioRule<>(Notepad.class);

    @Test
    public void testAddAndUpdateNotes() {
        for (int i = 0; i < TEST_NUMBER_OF_NOTES; i++) {
            // En la primera mitad de las notas, creación por menú; en la otra, por clic en FloatingActionButton
            if (i < TEST_NUMBER_OF_NOTES / 2 ) {
                // Hace clic en la opción de menú para insertar una nota (se asegura de que existe la opción)
                openActionBarOverflowOrOptionsMenu(ApplicationProvider.getApplicationContext());
                onView(withText(R.string.add_note)).check(matches(isDisplayed())).perform(click());
            } else {
                onView(withId(R.id.fab)).perform(click());
            }

            // Se asegura de que la actividad actual es NoteEdit comprobando que hay un EditText para el título
            onView(withId(R.id.title)).check(matches(isDisplayed()));

            // En el título inserta "Espresso Note Title <i>"
            final String title = TEST_TITLE + " " + i;
            // Utilizo «replaceText» en lugar de «typeText» para evitar modificaciones por parte del autocorrector
            onView(withId(R.id.title)).perform(replaceText(title), closeSoftKeyboard());

            // En el cuerpo inserta "Espresso Note Body <i>"
            final String body = TEST_BODY + " " + i;
            // Utilizo «replaceText» en lugar de «typeText» para evitar modificaciones por parte del autocorrector
            onView(withId(R.id.body)).perform(replaceText(body), closeSoftKeyboard());
            goBackToNotesList(i, false);

            // Aserción: comprobación de que la nota se visualiza en el listado
            onView(withId(R.id.recyclerview)).perform(scrollTo(hasDescendant(withText(title))));
            onView(withText(title)).check(matches(isDisplayed()));

            // Aserción: comprobación de que la nota está en la base de datos
            scenarioRule.getScenario().onActivity(activity -> {
                Note actualNote = activity.getNoteRepository().getNoteByTitle(title);
                assertNoteEquals(title, body, actualNote);
            });

            // Actualización
            onView(withId(R.id.recyclerview)).perform(scrollTo(hasDescendant(withText(title))));
            onView(withText(title)).perform(longClick());
            onView(withText(R.string.menu_edit)).perform(click());

            // Se asegura de que la actividad actual es NoteEdit comprobando que hay un EditText para el título
            onView(withId(R.id.title)).check(matches(isDisplayed()));

            // Cambia el título por "Espresso Updated Note Title <i>"
            final String updatedTitle = TEST_UPDATED_TITLE + " " + i;
            // Utilizo «replaceText» en lugar de «typeText» para evitar modificaciones por parte del autocorrector
            onView(withId(R.id.title)).perform(replaceText(updatedTitle), closeSoftKeyboard());
            goBackToNotesList(i, false);

            // Aserción: comprobación de que la nota se visualiza en el listado
            onView(withId(R.id.recyclerview)).perform(scrollTo(hasDescendant(withText(updatedTitle))));
            onView(withText(updatedTitle)).check(matches(isDisplayed()));

            // Aserción: comprobación de que la nota está en la base de datos
            scenarioRule.getScenario().onActivity(activity -> {
                Note actualNote = activity.getNoteRepository().getNoteByTitle(updatedTitle);
                assertNoteEquals(updatedTitle, body, actualNote);
            });
        }
    }

    private void goBackToNotesList(int noteNumber, boolean errorIntroduction) {

        if (errorIntroduction) {
            // En una de cada dos notas, confirma; en otra cancela
            if (noteNumber % 2 == 0) {
                // Confirma y vuelve a la actividad anterior
                onView(withId(R.id.button_save)).perform(click());
            } else {
                // Vuelve a la actividad anterior sin confirmar
                pressBack();
            }
        } else {
            onView(withId(R.id.button_save)).perform(click());
        }

    }

    @After
    public void borrarNotas() {
        for (int i = 0; i < TEST_NUMBER_OF_NOTES; i++) {
            // Busca y borra la nota con titulo "Espresso Note Title <i>"
            String title = TEST_UPDATED_TITLE + " " + i;
            onView(withId(R.id.recyclerview)).perform(scrollTo(hasDescendant(withText(title))));
            onView(withText(title)).perform(longClick());
            onView(withText(R.string.menu_delete)).perform(click());
        }
    }

    private void assertNoteEquals(String title, String body, Note note) {
        assertThat(note.getTitle(), is(equalTo(title)));
        assertThat(note.getBody(), is(equalTo(body)));
    }
}
