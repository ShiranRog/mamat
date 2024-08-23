#!/bin/bash

# section 1:
number_of_arguments=$#

if [[ "$number_of_arguments" -ne 1 ]]; then
    echo "Wrong number of arguments" >&2
    exit 1
fi

course_number="$1" # First input is supposed to be the name of grade file

if [[ ! -f "$course_number" ]]; then
    echo "Course not found" >&2
    exit 1
fi

# section 2:
folder_name="${course_number}_stat"
if [[ -d "$folder_name" ]]; then # Erase directory if exists
    rm  -r "$folder_name"
fi
mkdir "$folder_name"

# section 3
cat "${course_number}" | ./hist.exe - -nbins 10 > "$folder_name"/histogram.txt

# section 4

mean=$(cat "${course_number}" | ./mean.exe)
median=$(cat "${course_number}" | ./median.exe)
min=$(cat "${course_number}" | ./min.exe)
max=$(cat "${course_number}" | "./max.exe")

output="$mean\t$median\t$min\t$max"
echo -e $output > "${folder_name}/statistics.txt"
cd "$folder_name"
