#!/bin/bash

gnome-terminal -- bash -c "echo 'Main'; './Main/build/Application/Main' ";
gnome-terminal -- bash -c "echo 'GUI'; './GUIApplication/build/Application/GUI'"