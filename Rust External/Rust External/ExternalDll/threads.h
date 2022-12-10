#pragma once

#include "GuiEngine.h"
#include "DriverRW.h"
#include "loops.h"
#include "vars.h"
#include "RustSDK.h"
#include "aim.h"

#include "socket.h"

void BN_thread();
void ESP_thread();
void aim_thread();
void menu_thread();
void GOM_thread();
void socket_checker();