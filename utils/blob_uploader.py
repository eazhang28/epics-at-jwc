import sqlite3
import os

db_path = 'fontdch.db'
folder_path = './apps/astctl/assets/fonts/default/'

conn = sqlite3.connect(db_path)
cursor = conn.cursor()

for filename in os.listdir(folder_path):
    if filename.endswith(".gcode"):
        font_name = os.path.splitext(filename)[0]
        file_path = os.path.join(folder_path, filename)

        with open(file_path, 'rb') as f:
            new_blob = f.read()
        print(new_blob)
        # Update the data where the font name matches the filename
        cursor.execute(
            "UPDATE FCLOOKUP_NEW SET data = ? WHERE id = ?",
            (new_blob, font_name)
        )

conn.commit()
print(f"Update finished. Rows affected: {conn.total_changes}")
conn.close()
