# convert parquet format file to csv 20190620
import os
import sys
import pandas as pd
import pyarrow as pa
import pyarrow.csv as csv
import pyarrow.parquet as pq

def err(msg):
    print("Error: " + str(msg))
    sys.exit(1)

if len(sys.argv) < 2:
    err("Usage: parquet_to_csv [input file name.parquet]")

fn = sys.argv[1]
if not os.path.exists(fn):
    err("could not file file: " + fn)

if not fn.split(".")[-1] == "parquet":
    err(".parquet file expected")

open(fn + '.csv', 'wb').write(pq.read_table(fn).to_pandas().to_csv().encode())