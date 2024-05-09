#!/bin/sh

FILES=$(python3 scripts/update_headers.py)

for FILE in $FILES; do
    git add $FILE
done

cpplint --recursive src/