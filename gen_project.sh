#!/usr/bin/env bash

set -ex

project_name=$1
project_location="projects/$project_name"

echo "add_subdirectory($project_name)" >> projects/CMakeLists.txt
mkdir $project_location
cp -r framework/* $project_location
echo "# $project_name" > "$project_location/README.md"
