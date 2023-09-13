#!/bin/bash

# Requested packages 
required_packages=("build-essential" "xserver-xorg" "xbindkeys" "libx11-dev" "x11-utils" "libxkbcommon-x11-dev" "libxkbcommon-dev" "libicu-dev")

# Requested files, can be configured yourself
required_files=("$HOME/.xbindkeysrc" "$HOME/.xinitrc")

# Activate OpenSwitcher using ctr+alt
defaultCombination="
# Activate OpenSwitcher
# To read about how to customise read \`man xbindkeys\`
\"xsel --primary -o > ~/Programs/Project1/tmp && xsel --clear\"
  Control + Alt_L
"

# Check requested packages 
for package in "${required_packages[@]}"; do
    if ! dpkg -s "$package" >/dev/null; then
        echo "Необходимый пакет $package не установлен. Установка..."
        if ! sudo apt install "$package"; then
            echo "Error: для работы программы необходим $package" >> /dev/stderr
            exit 1;
        fi
    else
        echo "Пакет $package уже установлен."
    fi
done

# Check requested files
for file in "${required_files[@]}"; do
    if [ ! -f "$file" ]; then
        echo "Файл $file не найден."

        if [ "$file" = "$HOME/.xbindkeysrc" ]; then

            read -r -p "Хотите добавить стандартный $file ? (y/n): " answer
            if [ "$answer" = "y" ] || [ "$answer" = "Y" ]; then
                echo "Установка стандартного файла $file"
                xbindkeys --defaults > "$file"
                echo "Добавляется стандартная комбинация клавиш для запуска"
                echo "$defaultCombination" >> "$HOME/.xbindkeysrc"
                echo "Установите необходимую вам конфигурацию запуска"
            else
                echo "Error: для работы программы необходим $file" >> /dev/stderr
                exit 1
            fi
            
        elif [ "$file" = "$HOME/.xinitrc" ]; then
            echo "Рекомендуется добавить автостарт xbindkeys при запуску X session"
            read -r -p "Хотите добавить автостарт xbindkeys в $file? (y/n): " answer
            if [ "$answer" = "y" ] || [ "$answer" = "Y" ]; then
                cp /etc/X11/xinit/xinitrc "$HOME"/.xinitrc
                echo "exec xbindkeys&" >> "$HOME"/.xinitrc
            else
                echo "Автостарт xbindkeys не добавлен"
            fi
        fi
    else
        echo "Файл $file найден."
    fi
done

echo "Проверка завершена."