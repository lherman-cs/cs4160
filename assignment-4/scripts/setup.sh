#!/bin/bash

/tmp/scripts/install-ports.sh &
/tmp/scripts/install-expat.sh &
wait

echo "Setup completed!"