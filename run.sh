mkdir -p dump field image interface  # (Your run.sh already does this)
qcc -O2 sessile.c -fopenmp -o drop -lm
./drop > output.log 2>&1  # Log stdout/stderr
