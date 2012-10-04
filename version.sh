#!/bin/bash
VERSION=$(git rev-parse HEAD)
DATE=$(date '+%d.%m.%Y %H:%M')
cat << EOF > version.c
#include "version.h"
const char * build_date = "$DATE";
const char * build_git_sha = "$VERSION";
EOF

