PUNCTAJ PE CHECKERUL LOCAL:

=========================================
         -= TEMA 1 SDA 2026 =-
=========================================

test01.in: 8/8
  Valgrind PASSED
test02.in: 7/7
  Valgrind PASSED
test03.in: 10/10
  Valgrind PASSED
test04.in: 10/10
  Valgrind PASSED
test05.in: 5/5
  Valgrind PASSED
test06.in: 5/5
  Valgrind PASSED
test07.in: 5/5
  Valgrind PASSED
test08.in: 8/8
  Valgrind PASSED
test09.in: 7/7
  Valgrind PASSED
test10.in: 5/5
  Valgrind PASSED
test11.in: 5/5
  Valgrind PASSED
test12.in: 10/10
  Valgrind PASSED
test13.in: 2/2
  Valgrind PASSED
test14.in: 1/1
  Valgrind PASSED
test15.in: 1/1
  Valgrind PASSED
test16.in: 1/1
  Valgrind PASSED
test17.in: 1/1
  Valgrind PASSED
test18.in: 2/2
  Valgrind PASSED
test19.in: 1/1
  Valgrind PASSED
test20.in: 1/1
  Valgrind PASSED

-----------------------------------------
  Total teste:  95/95
  Valgrind:     20/20
  README: 5/5
-----------------------------------------
  Total tema:   120/120
=========================================
  Felicitari! Punctaj maxim!
=========================================

DESCRIERE COD:

Am adaugat niste functii ajutatoare pentru a simplifica codul. Precum:

- remove_nod_unitati = extrage si returneaza prima unitate din coada de echipaje 
disponibile; 

- remove_nod_incidente = extrage si returneaza primul incident din coada de prioritati
specifica;

- add_incidente = aloca memorie pt un nod nou; adauga un incident la finalul cozii de 
prioritati;

- add_unitati = aloca memorie pt un nod nou; adauga o unitate la finalul cozii de 
disponibilitate;

- ADD_INCIDENT = aloca memorie dinamic pt descriere; insereaza un nod nou la finalul 
listei de incident si adauga un pointer catre acest nod in coada de prioritate 
corespunzatoare;

- CHECK_UNITS_AVAILABILITY = parcurge elementele din coada de echipaje disponibile,
le numara si afiseaza totalul;

- DISPATCH = verifica daca coada de echipaje disponibile este nevida; extrage un incident
din coada cu gravitatea cea mai mare care nu este goala; creeaza o interventie noua care
leaga incidentul de echipaj si il insereaza la finalul listei de interventii; totodata se
adauga un pointer catre aceasta interventie si in stiva de istoric;

- UNDO_LAST_DISPATCH = extrage un element din stiva pana gaseste prima interventie 
nerezolvata; adauga echipajul la finalul cozii de disponibilitate si incidentul 
la inceputul cozii sale de prioritate; sterge nodul interventiei anulate din lista de interventii;

- SOLVED_INCIDENT = parcurge lista de interventii, gaseste incidentul, ii schimba 
statusul si adauga echipajul eliberat la finalul cozii de disponibilitate;

- SHOW_UNIT = cauta id-ul echipajului in vector si ii afiseaza disponibilitatea;

- SHOW_INCIDENT = parcurge lista de incidente pt a gasi id-ul si a afisa detaliile 
incidentului;

- SHOW_INTERVENTIONS = verifica daca lista de interventii contine doar santinela, daca nu
parcurge lista si afiseaza fiecare pereche incident-echipaj;

- free_all = parcurge si elibereaza complet memoria, luand fiecare nod in parte din stiva, 
cozi si liste;

- main = deschidere/inchidere fisiere; initializarea vectorului si a santinelelor pt
liste, a capetelor pt cozi si varful pt stiva; comenzile se executa printr-un for;

