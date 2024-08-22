#!/bin/bash 

# the adress was given as a part of the exercise:
main_adress="https://www.ynetnews.com/category/3082"
data=$(wget --no-check-certificate -qO - "$main_adress")
# both the specific article adresses and the number of the articles are needed:
article_urls=$(echo "$data" | grep -oP "https://(www.)?ynetnews.com/article/[a-zA-Z0-9]*" | sort | uniq)
num_of_lines=$(echo "$article_urls" | wc -l)

for (( i=1; i<=$num_of_lines; ++i )); do
    sub_adress="$(echo "$article_urls" | head -n $i | tail -n 1)"
    article_data=$(wget --no-check-certificate -qO - "$sub_adress")
    Netanyahu_count=$(echo "$article_data" | grep -oP "Netanyahu" | wc -l)
    Gantz_count=$(echo "$article_data" | grep -oP "Gantz" | wc -l)
    # seperating between cases without and with mentions:
    if [[ $Netanyahu_count -eq 0 ]] && [[ $Gantz_count -eq 0 ]]; then
        echo "$sub_adress, -"     
    else
        echo "$sub_adress, Netanyahu, $Netanyahu_count, Gantz, $Gantz_count"
    fi  
done