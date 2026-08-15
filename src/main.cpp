#include "metro_system.h"
#include "metro_tui.h"
int main(){
    metro_system metro;
    metro_tui ui(metro);
    ui.run();

    return 0;
}