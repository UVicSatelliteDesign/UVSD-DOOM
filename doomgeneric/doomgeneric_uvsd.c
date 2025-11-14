#include "doomgeneric.h"
#include "doomkeys.h"
#include "m_argv.h"

#include <stdio.h>
#include <unistd.h>

#include <stdbool.h>

void DG_Init() {
  // Initialize your platfrom (create window, framebuffer, etc...).
  // we probably want to Initialize a connection to the ground station
  // set up the protocol to read keys and set up the screen streaming
}

void DG_DrawFrame(uint32_t ms) {
  // Frame is ready in DG_ScreenBuffer. Copy it to your platform's screen.
}

void DG_SleepMs() {
  // sleep for ms
}

uint32_t DG_GetTicksMs() {
  // The ticks passed since launch in milliseconds.
}

int DG_GetKey(int *pressed, unsigned char *doomKey) {
  // Provide keyboard events.
}
