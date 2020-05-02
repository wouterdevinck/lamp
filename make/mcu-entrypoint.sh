#!/bin/bash
set -e

. $IDF_PATH/export.sh > /dev/null

exec "$@"