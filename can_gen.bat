@echo off
setlocal enabledelayedexpansion

vent-common\DBC\dbccangen.exe vent-common\DBC\vent.dbc VALVES 0

move "can_node_valves_bus0.h" "Core\Inc\can_node_valves_bus0.h"
move "can_node_valves_bus0.c" "Core\Src\can_node_valves_bus0.c"