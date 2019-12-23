#include "System.h"
#include "EmptyBehavior.h"
#include "MainMenu.h"

namespace pervansh {
    System* system;
}

void setup() {
    pervansh::system = new System(new EmptyBehavior(10, 11), new MainMenu(pervansh::system));
}

void loop() {
    pervansh::system->execute();
}
