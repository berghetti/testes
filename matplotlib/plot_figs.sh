#!/bin/bash

                            # col, save_name, title, ylabel

python3 policys_interval.py "latency_short 99.9%" latency_99_short.png "Requisições curtas (1 us)" "Latência 99,9% (us)"
python3 policys_interval.py "latency_short 99.9%" latency_99_short.pdf "" "Latência 99,9% (us)"
python3 policys_interval.py "latency_long 99.9%" latency_99_long.png "Requisições longas (100 us)" "Latência 99,9% (us)"
python3 policys_interval.py "latency_long 99.9%" latency_99_long.pdf "" "Latência 99,9% (us)"

python3 policys_interval.py "50% latency_short" latency_50_short.png "Requisições curtas (1 us)" "Latência 50% (us)"
python3 policys_interval.py "50% latency_short" latency_50_short.pdf  "" "Latência 50% (us)"
python3 policys_interval.py " 50% latency_long" latency_50_long.png  "Requisições longas (100 us)" "Latência 50% (us)"
python3 policys_interval.py " 50% latency_long" latency_50_long.pdf  "" "Latência 50% (us)"
