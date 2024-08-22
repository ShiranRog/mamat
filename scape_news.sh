#!/bin/bash

main_adress="https://ynetnews.com/category/3082"
data=$(wget -qO- "$main_adress" --no-check-certificate)

article_urls=$(echo "$data" | grep -oP 'https://www.ynetnews.com/article/[0-9a-zA-Z]*')
article_urls=$(echo "$article_urls" |sort | uniq)
num_of_lines=$(echo "$article_urls" | wc -l)

search_N="Netanyahu"
search_G="Gantz"
for ((i=1; i<=num_of_lines; i++)); do
	main_adress=$(echo "$article_urls" | sed -n "${i}p")
	source=$(wget -qO- "$main_adress")
	N=$(echo "$source" | grep -o Netanyahu | wc -l)
	G=$(echo "$source" | grep -o Gantz | wc -l)
	if [[ $N -eq 0 ]] && [[ $G -eq 0 ]]; then
		echo "$main_adress, -"
	else
		echo "$main_adress, Netanyahu, $N, Gantz, $G"
	fi
done
