#!/bin/bash

add_standart_combination() 
{
    if [ "$1" -eq 1 ]; then
        if ! grep -q "$default_combination" "$file"; then
			echo "Установка стандартной комбинации клавиш для запуска."
			echo "$default_combination" >> "$file"
			echo "Установите необходимую вам конфигурацию запуска"
        else
            echo "Стандартная комбинация клавиш найдена"
        fi
    else
		echo "Установка стандартного файла $file"
		mkdir "$XDG_CONFIG_HOME/actkbd"
		touch "$file"
		echo "Установка стандартной комбинации клавиш для запуска."
		echo "$default_combination" >> "$file"
		echo "Установите необходимую вам конфигурацию запуска"
    fi
    
}

add_standart_autostart()
{   
    if [ "$1" -eq 1 ]; then
        if ! grep -q "$default_autostart_actkbd" "$file"; then
			echo "Установка стандартного автостарта."
			echo "$default_autostart_actkbd" >> "$file"
        else
            echo "Стандартный автостарт найден"
        fi
    else
		echo "Копирование стандартного /etc/X11/xinit/xinitrc в $file."
		cp /etc/X11/xinit/xinitrc "$file"
		echo "Установка стандартного автостарта."
		echo "$default_autostart_actkbd" >> "$file"
    fi
    
}

# Check requested files
for file in "${required_files[@]}"; do

	if [ "$file" = "$XDG_CONFIG_HOME/actkbd/actkbd.conf" ]; then
		if [ ! -f "$file" ]; then
			echo "Файл $file не найден."
			add_standart_combination 0
		else
			echo "Файл $file найден."
			add_standart_combination 1
		fi

	elif [ "$file" = "$HOME_USER/.xinitrc" ]; then
		if [ ! -f "$file" ]; then
			echo "Файл $file не найден."
			add_standart_autostart 0
		else
			echo "Файл $file найден."
			add_standart_autostart 1
		fi
	
	fi

done

echo "Проверка завершена.
"
