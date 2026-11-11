
#!/bin/bash

file="addressbook.txt"

while true
do
    echo ""
    echo "----- ADDRESS BOOK -----"
    echo "1. Create"
    echo "2. View"
    echo "3. Insert"
    echo "4. Delete"
    echo "5. Modify"
    echo "6. Exit"
    read -p "Enter choice: " ch

    case $ch in
        1)
            > "$file"
            echo "Address book created."
            ;;

        2)
            if [ -f "$file" ]; then
                echo "Name | Phone | Address"
                cat "$file"
            else
                echo "Address book not found."
            fi
            ;;

        3)
            read -p "Enter name: " name
            read -p "Enter phone: " phone
            read -p "Enter address: " address
            echo "$name|$phone|$address" >> "$file"
            echo "Contact inserted."
            ;;

        4)
            read -p "Enter name to delete: " name
            if [ -f "$file" ]; then
                grep -v "^$name|" "$file" > temp.txt
                mv temp.txt "$file"
                echo "Contact deleted if it existed."
            else
                echo "Address book not found."
            fi
            ;;

        5)
            read -p "Enter name to modify: " name
            if [ -f "$file" ]; then
                grep -v "^$name|" "$file" > temp.txt
                read -p "Enter new phone: " phone
                read -p "Enter new address: " address
                echo "$name|$phone|$address" >> temp.txt
                mv temp.txt "$file"
                echo "Contact modified."
            else
                echo "Address book not found."
            fi
            ;;

        6)
            echo "Exiting..."
            break
            ;;

        *)
            echo "Invalid choice."
            ;;
    esac
done
