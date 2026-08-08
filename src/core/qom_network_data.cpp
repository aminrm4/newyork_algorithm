#include "qom_network_data.h"

void qom_network_data::build_network(graph &graph)
{
    graph.add_route(graph.add_station("terminal_mosaferbari_qom"), graph.add_station("ghaleh_kamkar"), 1.2, 3);

    graph.add_route(graph.add_station("ghaleh_kamkar"), graph.add_station("meydan_keshavarz"), 2.5, 5);

    graph.add_route(graph.add_station("meydan_keshavarz"), graph.add_station("meydan_motahari"), 6, 10);

    graph.add_route(graph.add_station("meydan_motahari"), graph.add_station("bimarestan_nekouei"), 3, 5);

    graph.add_route(graph.add_station("bimarestan_nekouei"), graph.add_station("meydan_baghiyatollah"), 2, 4);

    graph.add_route(graph.add_station("meydan_baghiyatollah"), graph.add_station("masjed_jamkaran"), 3.8, 6);

    graph.add_route(graph.add_station("meydan_motahari"), graph.add_station("harame_motahar_masoumeh"), 1.5, 4);

    graph.add_route(graph.add_station("harame_motahar_masoumeh"), graph.add_station("arge_salariyeh"), 1, 3);

    graph.add_route(graph.add_station("harame_motahar_masoumeh"), graph.add_station("darvazeh_ray"), 1.8, 4);

    graph.add_route(graph.add_station("darvazeh_ray"), graph.add_station("bajak"), 1.3, 3);

    graph.add_route(graph.add_station("meydan_motahari"), graph.add_station("niroogah"), 2.8, 5);

    graph.add_route(graph.add_station("niroogah"), graph.add_station("sadeghiyeh"), 1.5, 3);

    graph.add_route(graph.add_station("sadeghiyeh"), graph.add_station("chehel_derakht"), 1.7, 3);

    graph.add_route(graph.add_station("chehel_derakht"), graph.add_station("aminabad"), 1.4, 3);

    graph.add_route(graph.add_station("niroogah"), graph.add_station("aminabad"), 2, 4);

    graph.add_route(graph.add_station("meydan_baghiyatollah"), graph.add_station("pardisan"), 5, 8);

    graph.add_route(graph.add_station("pardisan"), graph.add_station("daneshgah_qom"), 2.2, 4);

    graph.add_route(graph.add_station("pardisan"), graph.add_station("boostan_jangali_ghadir"), 3.5, 6);

    graph.add_route(graph.add_station("harame_motahar_masoumeh"), graph.add_station("boostan_fadak"), 2.4, 4);

    graph.add_route(graph.add_station("ghaleh_kamkar"), graph.add_station("rahahan_qom"), 3, 5);

    graph.add_route(graph.add_station("daneshgah_qom"), graph.add_station("masjed_jamkaran"), 4.5, 7);
}