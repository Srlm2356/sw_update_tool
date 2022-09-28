# sw_update_tool
1. Имя config задается через команду ./xmlSetting [имя config], например ./xmlSetting NameFile  
	1.1. После чего в директории, где расположен xmlSetting, появится файл с *.xml расширением с автоматически созданным шаблоном параметров.  
	1.2. Содержимое полей:  
		<Computers>  
		<Media>  
		<Getversion>  
		<Getmedia>  
		<Update>  
		где name - наименование параметров  
		
		В Getversion - $hostname соответствует параметру Computers <hostname>  
		В Getmedia - $medianame соответствует параметру Media <medianame>  
		Update - как для Getverson и Getmedia  
	1.3. После редактирования полей, файл необходимо поместить в папку к основной программе update.  
2. Запуск программы осуществляется через команду ./update.  
   На вход может быть обьявлено несколько аргументов:  
		1) ./update [путь к конф файлу]   
		2) ./update [--x-pos размер] [--y-pos размер]  
		3) ./update [путь к конф файлу] [--x-pos размер] [--y-pos размер]  
		Например, ./update /export/home/eshestakova/config/update --x-pos 1000 --y-pos 300  
		Либо указав напрямую файл: ./update /export/home/eshestakova/config/config.xml --x-pos 1000 --y-pos 300  
	2.1. После запуска появится окно приложения:  
	
		Список "Выбор компьютера" - параметры из config <Computers>  
			Кнопка "Запрос текущей версии" - команда из config <Getversion>  
	    Список "Выбор носителя инофрмации" - параметры из config <Media>  
			"Кнопка Опрос носителя" - команда из config <Getmedia>  
		Кнопка "Обновить" - команда из config <Update>  
		По умолчанию все кнопки недоступны, пока не будут выбраны параметры.  
		Вся информация (о версии и КС выбранного компьютера) выводится в окно программы.  
