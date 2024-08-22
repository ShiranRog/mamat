#!/bin/bash

# test
main_adress="https://ynetnews.com/category/3082"
data=$(wget -qO- "$main_adress" --no-check-certificate)
article_urls=$(echo "$data" | grep -oP 'https://www.ynetnews.com/article/[0-9a-zA-Z]*')

article_urls=$(echo "$article_urls" |sort | uniq)
num_of_lines=$(echo "$article_urls" | wc -l)

search_N="Netanyahu"
search_G="Gantz"
for ((i=1; i<=num_of_lines; i++)); do
	sub_adress=$(echo "$article_urls" | sed -n "${i}p")
	article_data=$(wget -qO- "$sub_adress")
	N=$(echo "$article_data" | grep -o Netanyahu | wc -l)
	G=$(echo "$article_data" | grep -o Gantz | wc -l)
	if [[ $N -eq 0 ]] && [[ $G -eq 0 ]]; then
		echo "$sub_adress, -"
	else
		echo "$sub_adress, Netanyahu, $N, Gantz, $G"
	fi
done
