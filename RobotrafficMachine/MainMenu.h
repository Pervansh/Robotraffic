#pragma once

#include "MenuWindow.h"

class MenuWindow;

class MainMenu : public MenuWindow {
public:
    MainMenu(System* system, AbstractWindow* prev = nullptr);
};
