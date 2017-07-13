# Extract counts, skipping headers.
data/cholera_counts.txt: data/Cholera_Deaths.csv
	tail -n +2 $< | cut -d, -f2 > $@
