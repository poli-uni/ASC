# Crowdsensing  (Romanian language)



	Arhitectura sistemelor de calcul
	
	Tema 1 - Crowdsensing
	
	Badila Gabriel Alin
	333 CA
	
	
	============================================================================
	
	Arhiva temei contine trei fisiere: device.py, barrier.py si fisierul README. 

	In fisierul barrier.py este implementata o bariera reentranta cu semafoare 
	(implementare preluata din laboratorul 3).

	In fisierul device.py are loc instantierea device-urilor.
	Pentru sincronizarea device-urilor si a thread-urilor fiecarui device am 
	folosit bariera din fisierul barrier.py si un lock. Toate elementele de 
	sincronizare au fost instantiate in device-ul cu id-ul egal cu 0 si apoi 
	transmise si celorlalte device-uri. Lock-ul a fost implementat sub 
	forma unui dictionar pentru a sti daca o locatie are sau nu un lock creat.

	Din clasa "DeviceThread", am luat partea codului in care sunt preluate 
	datele de la device-uri, este rulat scriptul si rezultatele sunt transmise 
	inapoi device-urilor si am mutat-o intr-o noua clasa, numita "MyThread" 
	care simuleaza un thread al unui device. Astfel, in "DeviceThread", adaug si 
	device-ul curent la lista de vecini (pentru a diminua numarul de operatii al 
	unui thread). Apoi, folosind un for, impart numarul de scripturi in mod egal 
	thread-urilor, instantiez clasa "MyThread" de un numar de ori egal cu 
	numarul de thread-uri al device-ului curent si trimit ca parametrii: datele 
	device-ului curent, un script, o locatie si lista de vecini. La sfarsitul 
	for-ului apelez wait pe bariera pentru a sincroniza toate device-urile.

	In clasa "MyThread", fiecare thread al unui device preia datele care ii sunt 
	specificate de script de la device-urile vecine, apeleaza algoritmul asupra 
	lor si transmite noile date la device-urile implicate. In aceasta clasa este 
	utilizat dictionarul de lock-uri. Se verifica intai daca exista un lock 
	pentru locatia respectiva (in caz contrar se creeaza), se da acquire pentru 
	a bloca accesul la date al celorlalte thread-uri, se executa operatiile 
	mentionate putin mai sus si apoi se elibereaza lock-ul cu release.

	============================================================================