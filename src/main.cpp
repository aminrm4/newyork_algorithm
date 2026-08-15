#include "metro_system.h"
#include "metro_ui.h"
int main(){
    metro_system metro;
    metro_ui ui(metro);
    ui.run();

    return 0;
}