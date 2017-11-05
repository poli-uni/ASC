# Inmultirea eficienta a matricelor in C/C++ (Romanian language)



	Arhitectura sistemelor de calcul

	Tema 2 - Inmultirea eficienta a matricelor in C/C++

	Badila Gabriel Alin
	333 CA


	============================================================================

	Arhiva temei contine trei fisiere: tema2.c, tema2.cfg, utils.c, utils.h,
	Makefile si README.

	In fisierul "tema2.c" are loc intreaga implementare a temei. In fisierul
	"utils.h" a fost realizata o singura modificare: matricele din structura
	'test' au fost transformate in matrice liniare.

	In fisierul "tema2.c", in main, parcurg lista de teste intr-un for. La
	fiecare iteratie salvez numele fisierelor aferente testului respectiv in
	niste variabile. Aloc apoi, in mod liniar (pentru optimizare) spatiu pentru
	cele trei matrice ce vor fi inmultite. Trimit functiei "readMatrix"
	variabilele din care urmeaza sa citesc cele trei matrice, iar in functia
	propriu-zisa citesc cele trei matrice in campurile structurii 'test', din
	"utils.h".

	Revenind in main, dupa citirea matricelor, aloc spatiu pentru a patra
	matrice (matricea rezultat). Realizez inmultirea matricelor folosind
	algoritmul BMM (Blocked Matrix Multiplication), de la sfarsitul
	laboratorului 5, cu dimensiunea blocului egala cu 8. Pentru o mai mare
	optimizare apelez si la schimbarea ordinii buclelor din cadrul algoritmului
	BMM.

	In cele din urma folosesc fprintf pentru scrierea matricei rezultat in
	fisierul de output aferent testului curent si trec la urmatorul test.

	============================================================================

	Pentru compilare am folosit comanda:
			gcc -Wall -O2 -funroll-loops -o tema2 tema2.c utils.c

	Dupa putina documentare legata de optimizarea loop-urilor am descoperit ca
	de fiecare data cand un loop ajunge la final cu executarea instructiunilor
	din cadrul lui, procesorul face un guess (ghiceste) daca loop-ul o va lua
	din nou de la capat sau se va incheia. Daca in cazul loop-urilor mici
	procesorul are o rata de guess destul de ridicata, in cazul loop-urilor mari
	(spre exemplu tema noastra: 1000 sau 4000 de iteratii) procesorul are o rata
	de guess extrem de mica, ceea ce face ca executabilul sa ruleze intr-un timp
	mai lung. De aceea flagul "-funroll-loops" realizeaza desfacerea
	loop-urilor, eliminand guess-ul procesorului si aducand o eficienta mult mai
	ridicata programului.

	Pentru a folosi flagul "-funroll-loops" a fost nevoie sa folosesc si un
	nivel O al GCC-ului. Nivelul O2 este un nivel mediu, dar sigur. Nivele
	pornesc de la O0 (fara optimizari) si ajung pana la O3 (optimizari
	puternice, uneori predispuse la erori.) Din cauza predispunerii la erori a
	nivelului O3 am ales sa folosesc nivelul O2 (din cate am gasit, dupa ce
	m-am documentat, este si cel mai folosit nivel).

	Cu nivelul O2 trece durata de timp impusa (2min 22sec). Mentionez ca am
	rulat si cu nivelul O1 si pica durata de timp impusa la limita (3min 05sec).

	============================================================================
	
	MENTIUNE
	
	Calea catre folder-ele de 'input' si 'output' a fost data pentru cazul cand 
	acestea se gasesc in acelasi folder cu fisierul sursa.
	