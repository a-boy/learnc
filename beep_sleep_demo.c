#include <stdio.h>
#include <windows.h>

// A simple tune using Beep() and Sleep()
int main() {
    printf("Playing a simple tune using Beep() and Sleep()...\n");

    // Define the notes (frequency in Hz)
    int C4 = 261;
    int D4 = 293;
    int E4 = 329;
    int F4 = 349;
    int G4 = 392;
    
    // Define duration (in milliseconds)
    int quarter_note = 400;
    int half_note = 800;
    int pause = 100;

    // Play "Twinkle Twinkle Little Star"
    Beep(C4, quarter_note); Sleep(pause);
    Beep(C4, quarter_note); Sleep(pause);
    Beep(G4, quarter_note); Sleep(pause);
    Beep(G4, quarter_note); Sleep(pause);
    // Beep(A4, quarter_note); Sleep(pause); // A4 is a bit high, let's skip it
    // Beep(A4, quarter_note); Sleep(pause);
    Beep(G4, half_note);   Sleep(pause);

    Beep(F4, quarter_note); Sleep(pause);
    Beep(F4, quarter_note); Sleep(pause);
    Beep(E4, quarter_note); Sleep(pause);
    Beep(E4, quarter_note); Sleep(pause);
    Beep(D4, quarter_note); Sleep(pause);
    Beep(D4, quarter_note); Sleep(pause);
    Beep(C4, half_note);   Sleep(pause);

    printf("Playback finished.\n");

    return 0;
}
