#include "qom_network_data.h"

void qom_network_data::build_network(graph &graph)
{
    int terminal = graph.add_station("terminal_mosaferbari_qom", 34.69455, 50.87186);
    int ghaleh = graph.add_station("ghaleh_kamkar", 34.68400, 50.87300);
    int keshavarz = graph.add_station("meydan_keshavarz", 34.67200, 50.87400);
    int motahari = graph.add_station("meydan_motahari", 34.65200, 50.88300);
    int nekouei = graph.add_station("bimarestan_nekouei", 34.64600, 50.87200);
    int baghiyatollah = graph.add_station("meydan_baghiyatollah", 34.63200, 50.87100);
    int jamkaran = graph.add_station("masjed_jamkaran", 34.58390, 50.91470);
    int shrine = graph.add_station("harame_motahar_masoumeh", 34.64190, 50.88552);
    int arge = graph.add_station("arge_salariyeh", 34.61876, 50.84570);
    int darvazeh = graph.add_station("darvazeh_ray", 34.64300, 50.90200);
    int bajak = graph.add_station("bajak", 34.65224, 50.89483);
    int niroogah = graph.add_station("niroogah", 34.64200, 50.85200);
    int sadeghiyeh = graph.add_station("sadeghiyeh", 34.64600, 50.83600);
    int chehel = graph.add_station("chehel_derakht", 34.64800, 50.81800);
    int aminabad = graph.add_station("aminabad", 34.64400, 50.80600);
    int pardisan = graph.add_station("pardisan", 34.55846, 50.81080);
    int daneshgah = graph.add_station("daneshgah_qom", 34.55500, 50.82900);
    int ghadir = graph.add_station("boostan_jangali_ghadir", 34.56244, 50.77855);
    int fadak = graph.add_station("boostan_fadak", 34.65400, 50.91050);
    int rail = graph.add_station("rahahan_qom", 34.67800, 50.84400);

    graph.add_route(terminal, ghaleh, 1.2, 3);
    graph.add_route(ghaleh, keshavarz, 2.5, 5);
    graph.add_route(keshavarz, motahari, 6, 10);
    graph.add_route(motahari, nekouei, 3, 5);
    graph.add_route(nekouei, baghiyatollah, 2, 4);
    graph.add_route(baghiyatollah, jamkaran, 3.8, 6);
    graph.add_route(motahari, shrine, 1.5, 4);
    graph.add_route(shrine, arge, 1, 3);
    graph.add_route(shrine, darvazeh, 1.8, 4);
    graph.add_route(darvazeh, bajak, 1.3, 3);
    graph.add_route(motahari, niroogah, 2.8, 5);
    graph.add_route(niroogah, sadeghiyeh, 1.5, 3);
    graph.add_route(sadeghiyeh, chehel, 1.7, 3);
    graph.add_route(chehel, aminabad, 1.4, 3);
    graph.add_route(niroogah, aminabad, 2, 4);
    graph.add_route(baghiyatollah, pardisan, 5, 8);
    graph.add_route(pardisan, daneshgah, 2.2, 4);
    graph.add_route(pardisan, ghadir, 3.5, 6);
    graph.add_route(shrine, fadak, 2.4, 4);
    graph.add_route(ghaleh, rail, 3, 5);
    graph.add_route(daneshgah, jamkaran, 4.5, 7);
}