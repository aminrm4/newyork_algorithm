#include "levenshtein_search.h"

levenshtein_search::levenshtein_search(interface_graph &graph):graph(graph)
{
   
}

station levenshtein_search::find_closest(const std::string &name)
{
    auto &stations = graph.get_all_stations();
    station ans;
    int min_dict = std::numeric_limits<int>::max();
    for( auto st : stations){
        int dict = levenshtein_distance(name, st.get_name());
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