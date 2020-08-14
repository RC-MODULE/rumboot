#!/usr/bin/python3
import sys
from parse import parse

labels = []

for f in sys.argv[1:]:
    with open(f,"r") as fl:
        for line in fl.readlines(3):
            ret = parse("{}-*- rumboot-test-labels: {} -*-{}", line)
            if ret:
                for label in ret[1].split(" "):
                    if len(label)>0:
                        labels.append(label)

print(";".join(labels))
sys.exit(0)