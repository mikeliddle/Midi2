#!/bin/sh

# if mac, run scripts;/init-mac.sh
if [ "$(uname)" == "Darwin" ]; then
    scripts/init-mac.sh
fi

if [ "$(uname)" == "Linux" ]; then
    scripts/init-linux.sh
fi