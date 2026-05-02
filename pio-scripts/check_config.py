import os
import shutil

# Beide Dateien liegen im Hauptverzeichnis (Root)
source_file = "config.hpp.example"
target_file = "config.hpp"

def check_config():
    if not os.path.isfile(target_file):
        if os.path.isfile(source_file):
            print(f"--- [INFO] Kopiere {source_file} -> {target_file} im Hauptverzeichnis ---")
            shutil.copyfile(source_file, target_file)
        else:
            print(f"--- [FEHLER] {source_file} fehlt! ---")
    else:
        print(f"--- [INFO] {target_file} existiert bereits. ---")

check_config()