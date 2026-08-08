import os

# Ask user for main folder path
folder = input("Enter the main folder path: ").strip().strip('"')

# Check if folder exists
if not os.path.isdir(folder):
    print("Invalid folder path.")
    exit()

# Output file
output_file = os.path.join(folder, "subfolders.txt")

# Get only direct subfolders
subfolders = [
    os.path.join(folder, name)
    for name in os.listdir(folder)
    if os.path.isdir(os.path.join(folder, name))
]

# Write comma-separated paths
with open(output_file, "w", encoding="utf-8") as f:
    f.write(",".join(subfolders))

print(f"\nSaved {len(subfolders)} subfolders to:")
print(output_file)