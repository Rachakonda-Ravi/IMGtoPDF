import os
"""Rename folders or files based on prefix."""

def rename_items(main_folder, item_type):
    
    if not os.path.exists(main_folder):
        print("\n❌ Folder does not exist:")
        print(main_folder)
        return

    old_prefix = input("\nEnter prefix to replace:~\t").strip()
    new_prefix = input("Enter new prefix:~\t").strip()

    count = 0

    for item_name in os.listdir(main_folder):
        old_path = os.path.join(main_folder, item_name)

        if item_type == "folder" and os.path.isdir(old_path):
            match = item_name.startswith(old_prefix)

        elif item_type == "file" and os.path.isfile(old_path):
            match = item_name.startswith(old_prefix)

        else:
            match = False

        if match:
            new_name = new_prefix + item_name[len(old_prefix):]
            new_path = os.path.join(main_folder, new_name)

            os.rename(old_path, new_path)

            print(f"✅ {item_name}  ➜  {new_name}")
            count += 1

    print(f"\nCompleted. {count} item(s) renamed.\n")


def main():
    print("\n" + "=" * 45)
    print("        PREFIX RENAME TOOL")
    print("=" * 45)
    
    main_folder = input("\nEnter main folder path:\n> ").strip()
    while True:
        print("\n"+ "=" * 45)
        
        print("""MAIN MENU
                1. Rename Folders
                2. Rename Files
                3. Change Main Folder
                4. Exit
                """)

        choice = input("Choose option :~\t").strip()

        if choice == "1":
            rename_items(main_folder, "folder")
            
        elif choice == "2":
                rename_items(main_folder, "file")
                
        elif choice == "3":
            continue
        elif choice == "4":
            break

        else:
            print("\n❌ Invalid choice. Try again.")


    print("\n" + "=" * 45)
    print("          Program Over")
    print("=" * 45)


if __name__ == "__main__":
    main()
