#include "levenshtein_search.h"

levenshtein_search::levenshtein_search(interface_graph &graph):graph(graph)
{
   
}

std::string levenshtein_search::to_lower(const std::string &text){
    std::string result = text;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return result;
}

station levenshtein_search::find_closest(const std::string &name)
{
    const auto &stations = graph.get_all_stations();

    if(stations.empty())
        throw std::runtime_error("no station available");


    std::string normalized_query = to_lower(name);

    station ans;
    int min_dict = std::numeric_limits<int>::max();
    for( auto st : stations){
        const std::string& station_name = st.get_name();
        std::string normalized_station_name = to_lower(station_name);
        // Exact match
        if(normalized_query == normalized_station_name) return st;

        // Prefix match
        if(normalized_station_name.rfind(normalized_query, 0) == 0) return st;

        int dict = levenshtein_distance(normalized_query, normalized_station_name);
        if(min_dict > dict){
            ans = st;
            min_dict = dict;
        }
    }

    return ans;
}

int levenshtein_search::levenshtein_distance(
    const std::string &first,
    const std::string &second
)
{
    std::vector<std::vector<int>> dp(
        first.size() + 1,
        std::vector<int>(second.size() + 1)
    );

    for (int i = 0; i <= static_cast<int>(first.size()); i++)dp[i][0] = i;
    

    for (int j = 0; j <= static_cast<int>(second.size()); j++)dp[0][j] = j;

    
    

    for (int i = 1; i <= static_cast<int>(first.size()); i++){
        for (int j = 1; j <= static_cast<int>(second.size()); j++){
        
            if (first[i - 1] == second[j - 1]){
                dp[i][j] = dp[i - 1][j - 1];
            }
            else{
                dp[i][j] =
                    1 + std::min({
                        dp[i - 1][j],
                        dp[i][j - 1],
                        dp[i - 1][j - 1]
                    });
            }
        }
    }

    return dp[first.size()][second.size()];
}