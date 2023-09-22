#!/bin/bash

source preinstall.sh

program=/usr/bin/openswitcher


if [ ! -f "$program" ]; then
    echo "Файл $program не найден."
    echo ""
    if [ -f "./openswitcher" ]; then
        echo "Копирование программы в /usr/bin/"
        echo ""
        sudo cp ./openswitcher /usr/bin/openswitcher
    else
        echo "Программа не скомпилированна в данной дериктории, установка не возможна"
        exit 1
    fi
else
    echo "Файл $program найден."
    echo ""
fi

echo "Пожалуйста добавьте правило для работы OpenSwitcher в ручную, для вашей безопасности"
echo "скопируйте и вставьте его при помощи комбинаций ctr+shift+c ctr+shift+v"
echo "Правило:"
echo "
# rule for correctly working OpenSwitcher programm 
$USER ALL = (ALL) ALL
$USER ALL = (root) NOPASSWD: /usr/bin/openswitcher
"

read -r -p "Хотите добавить правило для работы OpenSwitcher в файл /etc/sudoers? (y/n): " answer
if [ "$answer" = "y" ] || [ "$answer" = "Y" ]; then
    sudo visudo
else
    echo "Правило для работы OpenSwitcher не дабавлено, завершение установки"
    exit 1
fi

echo ""
echo "Добавте вручную команду привязанную к комбинации клавиш, которой вам будет удобно"
echo "Команда:"
echo ""
echo "sh -c "/usr/bin/xsel --primary -o | sudo /usr/bin/openswitcher""
