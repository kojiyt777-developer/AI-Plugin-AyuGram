name: Build Plugin

on:
  push:
  workflow_dispatch:

jobs:
  build:
    runs-on: ubuntu-latest
    
    steps:
      - uses: actions/checkout@v3
      
      - name: Install Qt
        run: |
          sudo apt-get update
          sudo apt-get install -y qtbase5-dev
      
      - name: Compile Plugin
        run: |
          g++ -shared -fPIC \
            ai_plugin.cpp \
            -o AiPlugin.plugin \
            $(pkg-config --cflags --libs Qt5Core Qt5Network)
          ls -la *.plugin
      
      - name: Upload Plugin
        uses: actions/upload-artifact@v3
        with:
          name: AiPlugin
          path: AiPlugin.plugin
